#include "lcd/lcd.hpp"
#include "midi.hpp"
extern "C" {
#include "FLASH_SECTOR_F4.h"
#include "main.h"
}
extern UART_HandleTypeDef huart1;
extern LCD_ lcd;
namespace midi
{
    void file::note_on(uint32_t t, uint8_t channel, uint8_t note)
    {
        message& m = m_pkg.msg[m_pkg.m_size];
        m_pkg.m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_ON << 4) + channel;
        m.data_byte   = note;
        lcd.draw_stringf(0, 6, "time_stamp:%d", t);
    }

    void file::note_off(uint32_t t, uint8_t channel, uint8_t note)
    {
        message& m = m_pkg.msg[m_pkg.m_size];
        m_pkg.m_size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_OFF << 4) + channel;
        m.data_byte   = note;
        lcd.draw_stringf(20, 6, "%d", t);
    }

    void file::load(uintptr_t addr)
    {
        // TODO
        Flash_Read_Bytes(addr, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg));
    }
    void file::export_midi()
    {
        // TODO
        static uint8_t end = '\n';
        HAL_UART_Transmit(&huart1, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg), 100);
        HAL_UART_Transmit(&huart1, &end, 1, 100);
    }
    void file::save(uintptr_t addr) const
    {
        // TODO
        Flash_Write_Bytes(addr, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg));
    }

} // namespace midi