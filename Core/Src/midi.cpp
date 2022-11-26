#include "midi.hpp"


namespace midi
{
    void file::note_on(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel)
    {
        message& m = m_msg[m_size];
        m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_ON << 4) + channel;
        m.data_byte   = note;
        m.velocity    = vel;
    }

    void file::note_off(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel)
    {
        message& m = m_msg[m_size];
        m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_OFF << 4) + channel;
        m.data_byte   = note;
        m.velocity    = vel;
    }

    void file::load(uintptr_t addr)
    {
        // TODO
    }

    void file::save(uintptr_t addr) const
    {
        // TODO
    }

} // namespace midi