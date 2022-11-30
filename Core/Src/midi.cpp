#include "midi.hpp"
extern "C" {
#include "FLASH_SECTOR_F4.h"
}
namespace midi
{
    void file::note_on(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel)
    {
        message& m = m_pkg.msg[m_pkg.m_size];
        m_pkg.m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_ON << 4) + channel;
        m.data_byte   = note;
        m.velocity    = vel;
    }

    void file::note_off(uint32_t t, uint8_t channel, uint8_t note, uint8_t vel)
    {
        message& m = m_pkg.msg[m_pkg.m_size];
        m_pkg.m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_OFF << 4) + channel;
        m.data_byte   = note;
        m.velocity    = vel;
    }

    void file::load(uintptr_t addr)
    {
        // TODO
        Flash_Read_Bytes(addr, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg));
    }

    void file::save(uintptr_t addr) const
    {
        // TODO
        Flash_Write_Bytes(addr, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg));
    }

} // namespace midi