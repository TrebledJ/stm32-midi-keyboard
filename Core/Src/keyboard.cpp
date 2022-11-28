#include "keyboard.hpp"

#include "lcd/lcd.hpp"
#include "speaker.hpp"
#include "utils/notes.hpp"

#include <cstdlib>


void kb::update_midi()
{
    uint32_t time_stamp = get_ticks() - m_start_record_time;
    uint8_t channel     = 1;
    if (is_recording) {
        for (int i = 0; i < NUM_KEYBOARD_KEYS; i++) {
            if (is_key_just_pressed(i)) {
                m_midi_file.note_on(time_stamp, channel, note2midi(button2note(i)));
            } else if (is_key_just_released(i)) {
                m_midi_file.note_off(time_stamp, channel, note2midi(button2note(i)));
            }
        }
    }
}

void kb::update_speaker()
{
    extern LCD_ lcd;
    lcd.draw_stringf(0, 16, "%d %d %d %d     ", m_is_recording, m_is_playback, m_start_play_time, m_playback_index);
    lcd.draw_stringf(0, 10, "%d  ", m_midi_file.size());
    for (int i = 0; i < m_midi_file.size(); i++) {
        lcd.draw_stringf(5 + 5 * (i % 4), 10 + (i / 4), "%d  ", m_midi_file[i].time_stamp);
    }

    if (is_playback) {
        while ((get_ticks() - m_start_play_time) > m_midi_file[m_playback_index].time_stamp) {
            // TODO: are the conversion functions flipped?
            uint16_t note  = midi2note(note2button(m_midi_file[m_playback_index].data_byte));
            m_active[note] = ((m_midi_file[m_playback_index].status_byte >> 4) & 1);
            m_playback_index++;
            if (m_playback_index == m_midi_file.size()) {
                _toggle_playback();
                break;
            }
        }
    }
    speaker::default_load(m_active);
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