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
    uint8_t channel     = settings::curr().channel;
    if (m_is_recording) {
        for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
            Note note = static_cast<Note>(button2note(i) + settings::curr().transpose);
            if (is_key_just_pressed(i)) {
                for (auto note : autochord::autonotes(note, settings::curr().autochord)) {
                    m_record_file.note_on(time_stamp, channel, note2midi(note));
                }
            } else if (is_key_just_released(i)) {
                for (auto note : autochord::autonotes(note, settings::curr().autochord)) {
                    m_record_file.note_off(time_stamp, channel, note2midi(note));
                }
            }
        }
    }
}

void kb::update_speaker()
{
    if (m_is_playback) {
        while (get_ticks() - m_start_play_time >= m_song_file[m_playback_index].time_stamp) {
            Note note = midi2note(m_song_file[m_playback_index].data_byte);
            // uint8_t vel  = m_song_file[m_playback_index].velocity;
            uint8_t vel  = 127;
            bool note_on = ((m_song_file[m_playback_index].status_byte >> 4) & 1);

            if (note_on) {
                speaker::note_on(note, vel);
            } else {
                speaker::note_off(note);
            }

            m_playback_index++;
            if (m_playback_index == m_song_file.size()) {
                // Stop playback.
                _toggle_playback();
                break;
            }
        }
    }

    // Handle keypress.
    for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
        Note note = button2note(i + settings::curr().transpose);
        if (is_key_just_pressed(i)) {
            // TODO: get key press velocity.
            uint8_t vel = 90;
            for (auto note : autochord::autonotes(note, settings::curr().autochord)) {
                speaker::note_on(note, vel);
            }
        } else if (is_key_just_released(i)) {
            for (auto note : autochord::autonotes(note, settings::curr().autochord)) {
                speaker::note_off(note);
            }
        }
    }
}

void kb::_toggle_playback()
{
    m_is_playback = !m_is_playback;
    m_song_file.load(FLASH_ADDR_START);
    reset();
    m_is_recording    = 0;
    m_playback_index  = 0;
    m_start_play_time = get_ticks();
}

void kb::_toggle_record()
{
    m_is_recording = !m_is_recording;
    if (m_is_recording) {
        m_is_playback = 0;
        m_song_file.erase(settings::curr().channel);
        m_record_file.reset();
        m_start_record_time = get_ticks();
        gpio_reset(LED1);
    } else {
        m_song_file.merge(m_record_file);
        m_song_file.save(FLASH_ADDR_START);
        // m_song_file.export_midi();
        gpio_set(LED1);
    }
}