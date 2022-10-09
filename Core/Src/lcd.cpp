#include "lcd/lcd.hpp"
#include "lcd/lcd_font.h"
#include "utils/utils.hpp"

#include <cstring>
#include <span>


void spi_wait_busy(SPI_HandleTypeDef* spi)
{
    // Wait for transmit to finish.
    while (__HAL_SPI_GET_FLAG(spi, SPI_FLAG_BSY))
        ;
}

template <Orientation O>
void LCD<O>::write_cmd(uint8_t cmd)
{
    spi_wait_busy(spi);
    TFT_DC::reset();
    HAL_SPI_Transmit(spi, &cmd, 1, 10);
    TFT_DC::set();
}

template <Orientation O>
void LCD<O>::write_data(uint8_t data)
{
    HAL_SPI_Transmit(spi, &data, 1, 10);
}

template <Orientation O>
void LCD<O>::init_sequence()
{
    uint32_t i     = 0;
    std::span cmds = ili9486_init_seq<O>;

    while (cmds[++i] != TFT_EOF_MARKER) {
        write_cmd(cmds[i]);

        while (cmds[++i] != TFT_EOL_MARKER) {
            if (cmds[i] == TFT_DELAY_MARKER)
                delay(cmds[++i]);
            else
                write_data(cmds[i]);
        }
    }
}

template <Orientation O>
void LCD<O>::ready_region(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    __disable_irq();
    write_cmd(0x2A); // Column addr set.
    write_data((x >> 8) & 0xFF);
    write_data((x)&0xFF); // X START.
    write_data(((x + w) >> 8) & 0xFF);
    write_data((x + w) & 0xFF);

    write_cmd(0x2B); // Row addr set.
    write_data((y >> 8) & 0xFF);
    write_data((y)&0xFF); // Y START.
    write_data(((y + h) >> 8) & 0xFF);
    write_data((y + h) & 0xFF);

    write_cmd(0x2C);
    __enable_irq();
}

template <Orientation O>
void LCD<O>::clear()
{
    draw_rect(palette.background(), 0, 0, LCD_WIDTH, LCD_HEIGHT);
}

template <Orientation O>
void LCD<O>::draw_pixel(color_t color, uint16_t x, uint16_t y)
{
    ready_region(x, y, 0, 0);
    write_data((color >> 16) & 0xFF);
    write_data((color >> 8) & 0xFF);
    write_data(color & 0xFF);
}

template <Orientation O>
void LCD<O>::draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    draw_rect(palette.foreground(), x, y, w, h);
}

template <Orientation O>
void LCD<O>::draw_rect(color_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // TODO: try 32-bit copying implementation?
    uint32_t nbytes   = w * h * 3ul; // Total number of byte to send.
    uint32_t bufbytes = std::min(nbytes, BUF_SIZE);
    for (uint32_t i = 0; i < bufbytes; i += 3) {
        curr_buffer[i]     = (color >> 16) & 0xFF;
        curr_buffer[i + 1] = (color >> 8) & 0xFF;
        curr_buffer[i + 2] = color & 0xFF;
    }

    spi_wait_busy(spi);
    ready_region(x, y, w, h);

    do {
        TFT_DC::set();
        uint32_t send_size = std::min(nbytes, bufbytes);
        spi_wait_busy(spi);
        HAL_SPI_Transmit_DMA(spi, curr_buffer, send_size);
        nbytes -= send_size;
    } while (nbytes > 0);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_image(const uint8_t* bytes, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    spi_wait_busy(spi);
    ready_region(x, y, w - 1, h - 1);
    HAL_SPI_Transmit_DMA(spi, const_cast<uint8_t*>(bytes), w * h * 3ul);
}


template <Orientation O>
void LCD<O>::draw_char(char c, uint16_t x, uint16_t y)
{
    spi_wait_busy(spi);
    uint8_t* bufptr  = curr_buffer;
    color_t color_fg = palette.foreground();
    color_t color_bg = palette.background();

    // Consider the current chunk and populate the buffer.
    for (uint16_t iy = 0; iy < CHAR_HEIGHT; iy++) {
        const uint8_t char_ptr = (CHAR_PTR(c))[iy];
        for (int8_t ix = CHAR_WIDTH - 1; ix >= 0; ix--) {
            // for (uint16_t ix = 0; ix < CHAR_WIDTH; ix++) {
            color_t color = ((char_ptr >> ix) & 0x01) ? color_fg : color_bg;
            draw_pixel(color, x * CHAR_WIDTH + ix, y * CHAR_HEIGHT + iy);
            *bufptr++ = (color >> 16) & 0xFF;
            *bufptr++ = (color >> 8) & 0xFF;
            *bufptr++ = color & 0xFF;
        }
    }

    draw_image(curr_buffer, x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_string(const char* str, uint16_t x, uint16_t y)
{
    for (; *str; str++) {
        draw_char(*str, x, y);
        x++;
    }
}

template class LCD<LCD_ORIENTATION>;
