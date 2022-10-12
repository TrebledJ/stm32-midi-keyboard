#include "lcd/lcd.hpp"
#include "lcd/lcd_font.h"
#include "utils/utils.hpp"

#include <cstdio> // sprintf


// TODO: try built-in DMA double buffering.

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
    TFT_DC::reset();                    // Command mode.
    HAL_SPI_Transmit(spi, &cmd, 1, 10); // Send command.
    TFT_DC::set();                      // Data mode.
}

template <Orientation O>
void LCD<O>::write_data(uint8_t data)
{
    HAL_SPI_Transmit(spi, &data, 1, 10); // Send data.
}

template <Orientation O>
void LCD<O>::init_sequence()
{
    const uint8_t* cmds = ili9486_init_seq<O>;
    while (*++cmds != TFT_EOF_MARKER) {
        write_cmd(*cmds);
        while (*++cmds != TFT_EOL_MARKER) {
            if (*cmds == TFT_DELAY_MARKER)
                delay(*++cmds);
            else
                write_data(*cmds);
        }
    }
}

template <Orientation O>
void LCD<O>::ready_region(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
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
}

template <Orientation O>
void LCD<O>::clear()
{
    draw_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, palette.background());
}

template <Orientation O>
void LCD<O>::draw_pixel(uint16_t x, uint16_t y, color_t color)
{
    ready_region(x, y, 0, 0);
    write_data((color >> 16) & 0xFF);
    write_data((color >> 8) & 0xFF);
    write_data(color & 0xFF);
}

template <Orientation O>
void LCD<O>::draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    draw_rect(x, y, w, h, palette.foreground());
}

template <Orientation O>
void LCD<O>::draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color)
{
    // TODO: try 32-bit copying implementation?
    uint32_t nbytes   = w * h * 3ul; // Total number of byte to send.
    uint32_t bufbytes = std::min(nbytes, BUF_SIZE);
    for (uint32_t i = 0; i < bufbytes; i += 3) {
        curr_buffer[i + 0] = (color >> 16) & 0xFF;
        curr_buffer[i + 1] = (color >> 8) & 0xFF;
        curr_buffer[i + 2] = color & 0xFF;
    }

    spi_wait_busy(spi);
    ready_region(x, y, w, h);

    do {
        uint32_t send_size = std::min(nbytes, bufbytes);
        spi_wait_busy(spi);
        HAL_SPI_Transmit_DMA(spi, curr_buffer, send_size);
        nbytes -= send_size;
    } while (nbytes > 0);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* bytes)
{
    spi_wait_busy(spi);
    ready_region(x, y, w - 1, h - 1);
    HAL_SPI_Transmit_DMA(spi, const_cast<uint8_t*>(bytes), w * h * 3ul);
}


template <Orientation O>
void LCD<O>::draw_char(uint16_t x, uint16_t y, char c)
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
            // draw_pixel(x * CHAR_WIDTH + ix, y * CHAR_HEIGHT + iy, color);
            *bufptr++ = (color >> 16) & 0xFF;
            *bufptr++ = (color >> 8) & 0xFF;
            *bufptr++ = color & 0xFF;
        }
    }

    draw_image(x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, curr_buffer);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_string(uint16_t x, uint16_t y, const char* str)
{
    for (; *str; str++) {
        draw_char(x++, y, *str);
    }
}

template class LCD<LCD_ORIENTATION>;
