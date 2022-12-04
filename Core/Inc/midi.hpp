#pragma once

#include "defines.hpp"

#include <cstdint>
#include <cstdlib>
#include <optional>


inline constexpr size_t MIDI_MAX_MESSAGES = 1000;


namespace midi
{
    enum status {
        NOTE_OFF = 8,
        NOTE_ON,
        POLYPHONIC_AFTERTOUCH,
        CONTROL_CHANGE,
        PR0GRAM_CHANGE,
        CHANNEL_AFTERTOUCH,
        PITCH_BEND_CHANGE
    };

    struct message {
        uint32_t time_stamp;
        uint8_t status_byte;
        uint8_t data_byte;

        uint8_t channel() const { return status_byte & 0xF; }
    };

    struct package {
        uint8_t start_byte  = 101;
        uint16_t size       = 0;
        uint8_t midi_volume = 100;
        uint8_t tempo       = 120;
        message msg[MIDI_MAX_MESSAGES];
    };

    class file
    {
    public:
        void reset() { m_pkg.size = 0; }
        size_t size() const { return m_pkg.size; }

        void note_on(uint32_t t, uint8_t channel, uint8_t note);
        void note_off(uint32_t t, uint8_t channel, uint8_t note);

        // Load midi data from the given address.
        void load(uintptr_t addr);
        void export_midi();

        // Save midi data to the given address.
        void save(uintptr_t addr);

        message& operator[](size_t i) { return m_pkg.msg[i]; }
        const message& operator[](size_t i) const { return m_pkg.msg[i]; }
        // message& at(size_t ch, size_t i) { return m_pkg.msg[ch][i]; }
        // const message& at(size_t ch, size_t i) const { return m_pkg.msg[ch][i]; }

        void erase(uint8_t channel);
        void merge(const file& other);

    private:
        package m_pkg;
    };

    // class playback
    // {
    // public:
    //     playback(const file& file) : m_file(file) {}

    //     void reset() {
    //         for (int i = 0; i < NUM_CHANNELS; i++)
    //             m_playback_index[i] = 0;
    //     }

    //     void start() {
    //         reset();
    //         m_start_play_time = get_ticks();
    //     }

    //     std::optional<const message&> poll_message() const {
    //         uint8_t from_channel = m_curr_channel;
    //         if (get_ticks() - m_start_play_time >= m_file.at(m_playback_index).time_stamp) {
    //             return m_file[m_playback_index];
    //         }
    //         return std::nullopt;
    //     }

    // private:
    //     const file& m_file;
    //     uint32_t m_start_play_time = 0;
    //     uint32_t m_playback_index[NUM_CHANNELS] = {0};
    //     uint8_t m_curr_channel = 0;

    //     uint8_t next_channel(uint8_t ch) {
    //         return (ch + 1) % NUM_CHANNELS;
    //     }
    // };
} // namespace midi
