#include "keyboard.hpp"
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
    if (is_playback) {
        const midi::message* msg = &m_midi_file[m_playback_index];
        while (get_ticks() - m_start_play_time > msg->time_stamp) {
            // TODO: are the conversion functions flipped?
            m_active[midi2note(note2button(msg->data_byte))] = ((msg->status_byte >> 4) & 1);
            m_playback_index++;
            msg = &m_midi_file[m_playback_index];
        }
    }
    speaker::default_load(m_active);
}

void kb::_toggle_playback()
{
    m_is_playback = !m_is_playback;
    reset();
    if (m_is_playback) {
        m_start_play_time = get_ticks();
        m_is_recording    = 0;
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