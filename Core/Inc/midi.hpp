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
    };
    struct package {
        uint8_t start_byte = 'e';
        uint16_t m_size    = 0;
        uint8_t volume     = 100;
        uint8_t tempo      = 120;
        message msg[MIDI_MAX_MESSAGES];
    };
    union big_package {
        midi::package pkg;
        uint8_t buff[sizeof(pkg)];
    };
    class file
    {
    public:
        void reset() { m_pkg.m_size = 0; }
        size_t size() const { return m_pkg.m_size; }

        void note_on(uint32_t t, uint8_t channel, uint8_t note);
        void note_off(uint32_t t, uint8_t channel, uint8_t note);

        // Load midi data from the given address.
        void load(uintptr_t addr);
        void export_midi();

        // Save midi data to the given address.
        void save(uintptr_t addr) const;

        message& operator[](size_t i) { return m_pkg.msg[i]; }
        const message& operator[](size_t i) const { return m_pkg.msg[i]; }

    private:
        package m_pkg;
    };
} // namespace midi
