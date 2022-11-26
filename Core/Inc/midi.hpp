#pragma once

#include <cstdint>
#include <cstdlib>


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
        uint8_t velocity;
    };

    class file
    {
    public:
        void reset() { m_size = 0; }
        size_t size() const { return m_size; }

        void note_on(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel = 90);
        void note_off(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel = 90);

        // Load midi data from the given address.
        void load(uintptr_t addr);

        // Save midi data to the given address.
        void save(uintptr_t addr) const;

        message& operator[](size_t i) { return m_msg[i]; }
        const message& operator[](size_t i) const { return m_msg[i]; }

    private:
        message m_msg[MIDI_MAX_MESSAGES];
        size_t m_size = 0;
    };
} // namespace midi
