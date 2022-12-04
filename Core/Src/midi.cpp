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
        message& m = m_pkg.msg[m_pkg.size];
        m_pkg.size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_ON << 4) | (channel & 0xF);
        m.data_byte   = note;
        // lcd.draw_stringf(0, 6, "time_stamp:%d", t);
    }

    void file::note_off(uint32_t t, uint8_t channel, uint8_t note)
    {
        message& m = m_pkg.msg[m_pkg.size];
        m_pkg.size++;
        m.time_stamp  = t;
        m.status_byte = (NOTE_OFF << 4) | (channel & 0xF);
        m.data_byte   = note;
        // lcd.draw_stringf(20, 6, "%d", t);
    }

    void file::load(uintptr_t addr)
    {
        // TODO
        Flash_Read_Bytes(addr, reinterpret_cast<uint8_t*>(&m_pkg), sizeof(m_pkg));
    }

    void file::export_midi()
    {
        // TODO
        static uint8_t end = '\n';
        HAL_UART_Transmit(&huart1, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&m_pkg)), sizeof(m_pkg), 100);
        HAL_UART_Transmit(&huart1, &end, 1, 100);
    }

    void file::save(uintptr_t addr)
    {
        // TODO
        Flash_Write_Bytes(addr, reinterpret_cast<uint8_t*>(&m_pkg), sizeof(m_pkg));
    }

    void file::erase(uint8_t channel)
    {
        int write_i = 0;
        for (int i = 0; i < m_pkg.size; i++) {
            if ((m_pkg.msg[i].status_byte & 0xF) != channel)
                m_pkg.msg[write_i++] = m_pkg.msg[i];
        }
        m_pkg.size = write_i;
    }

    void file::merge(const file& other)
    {
        message msg[MIDI_MAX_MESSAGES];

        // Copy first array.
        for (int i = 0; i < m_pkg.size; i++) {
            msg[i] = m_pkg.msg[i];
        }

        uint32_t new_size = m_pkg.size + other.m_pkg.size;
        int a = 0, b = 0;
        int i = 0;
        for (; a < m_pkg.size && b < other.m_pkg.size; i++) {
            if (msg[a].time_stamp < other.m_pkg.msg[b].time_stamp) {
                m_pkg.msg[i] = msg[a++];
            } else {
                m_pkg.msg[i] = other.m_pkg.msg[b++];
            }
        }
        if (a < m_pkg.size) {
            for (; i < new_size; i++)
                m_pkg.msg[i] = msg[a++];
        } else {
            for (; i < new_size; i++)
                m_pkg.msg[i] = other.m_pkg.msg[b++];
        }
        m_pkg.size = new_size;
    }

} // namespace midi