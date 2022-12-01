#include "display/menu.hpp"
#include "keyboard.hpp"
#include "lcd/lcd.hpp"
#include "settings.hpp"
#include "speaker.hpp"
#include "utils/notes.hpp"

#include <cstdlib>


void kb::update_midi()
{
    uint32_t time_stamp = get_ticks() - m_start_record_time;
    uint8_t channel     = 1;
    if (m_is_recording) {
        for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
            if (is_key_just_pressed(i)) {
                m_midi_file.note_on(time_stamp, channel, note2midi(button2note(i)) + settings::curr().transpose);
            } else if (is_key_just_released(i)) {
                m_midi_file.note_off(time_stamp, channel, note2midi(button2note(i)) + settings::curr().transpose);
            }
        }
    }
}

void kb::update_speaker()
{
    if (m_is_playback) {
        while (get_ticks() - m_start_play_time >= m_midi_file[m_playback_index].time_stamp) {
            Note note    = midi2note(m_midi_file[m_playback_index].data_byte);
            uint8_t vel  = m_midi_file[m_playback_index].velocity;
            bool note_on = ((m_midi_file[m_playback_index].status_byte >> 4) & 1);

            // uint16_t note  = note2button(midi2note(m_midi_file[m_playback_index].data_byte));
            // m_active[note] = ((m_midi_file[m_playback_index].status_byte >> 4) & 1);
            if (note_on) {
                speaker::note_on(note, vel);
            } else {
                speaker::note_off(note);
            }

            m_playback_index++;
            if (m_playback_index == m_midi_file.size()) {
                // Stop playback.
                _toggle_playback();
                break;
            }
        }
    }
    // speaker::default_load(m_active);

    // Handle keypress.
    for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
        Note note = button2note(i + settings::curr().transpose);
        if (is_key_just_pressed(i)) {
            // TODO: get key press velocity.
            uint8_t vel = 90;
            speaker::note_on(note, vel);
        } else if (is_key_just_released(i)) {
            speaker::note_off(note);
        }
    }
}

void kb::_toggle_playback()
{
    m_is_playback = !m_is_playback;
    reset();
    if (m_is_playback) {
        m_is_recording    = 0;
        m_playback_index  = 0;
        m_start_play_time = get_ticks();
    }
}

void kb::_toggle_record()
{
    m_is_recording = !m_is_recording;
    if (m_is_recording) {
        m_is_playback = 0;
        m_midi_file.reset();
        m_start_record_time = get_ticks();
    }
}