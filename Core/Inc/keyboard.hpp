// Functions to poll for keyboard keypresses and events.
#pragma once

#include "buttons.hpp"
#include "midi.hpp"


class kb
{
public:
    static constexpr uintptr_t FLASH_ADDR_START = 0x080C0000;

    static kb& instance()
    {
        static kb k;
        return k;
    }

    static void init()
    {
        instance().m_midi_file.load(FLASH_ADDR_START);
        instance().m_midi_file.export_midi();
    }

    static void loop()
    {
        if (!is_playback()) {
            instance().poll_keys();
        }
        instance().update_midi();
        instance().update_speaker();
    }

    static bool is_playback() { return instance().m_is_playback; }
    static bool is_recording() { return instance().m_is_recording; }
    static void toggle_playback() { instance()._toggle_playback(); }
    static void toggle_record() { instance()._toggle_record(); }

private:
    bool m_active[NUM_KEYBOARD_KEYS]      = {0};
    bool m_prev_active[NUM_KEYBOARD_KEYS] = {0};

    uint8_t m_is_recording = 0;
    uint8_t m_is_playback  = 0;

    uint32_t m_start_record_time = 0;
    uint32_t m_start_play_time   = 0;
    uint32_t m_playback_index    = 0;

    midi::file m_midi_file;

    bool raw_is_key_pressed(int b) const
    {
        return buttons::is_btn_pressed(static_cast<ButtonName>(BTN_1_U + 2 * b))
               || buttons::is_btn_pressed(static_cast<ButtonName>(BTN_1_D + 2 * b));
    }

    bool is_key_pressed(int b) const { return m_active[b]; }
    bool is_key_just_pressed(int b) const { return !m_prev_active[b] && m_active[b]; }
    bool is_key_just_released(int b) const { return m_prev_active[b] && !m_active[b]; }

    void reset()
    {
        for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
            m_active[i]      = false;
            m_prev_active[i] = false;
        }
    }

    void poll_keys()
    {
        for (int b = 0; b < NUM_KEYBOARD_KEYS; b++) {
            m_prev_active[b] = m_active[b];
            m_active[b]      = raw_is_key_pressed(b);
        }
    }

    void update_midi();
    void update_speaker();

    void _toggle_playback();
    void _toggle_record();
};
