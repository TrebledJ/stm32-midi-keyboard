#include "lcd/lcd.hpp"
#include "lcd/lcd_font.h"
#include "utils/utils.hpp"

#include <cstring>
#include <span>


void spi_wait_finished(SPI_HandleTypeDef* spi)
{
    // Wait for transmit to finish.
    while (!__HAL_SPI_GET_FLAG(spi, SPI_FLAG_TXE))
        ;
    while (__HAL_SPI_GET_FLAG(spi, SPI_FLAG_BSY))
        ;
}

template <Orientation O>
void LCD<O>::write_data(uint8_t data)
{
    spi->Instance->CR1 &= ~SPI_CR1_SPE; // Enable SPI.
    spi->Instance->DR = data;           // Send data.

    spi_wait_finished(spi);
}

// void LCD::write_data(uint8_t data)
// {
//     spi->Instance->DR = data;
//     while (!__HAL_SPI_GET_FLAG(spi, SPI_FLAG_TXE))
//         ;
// }

template <Orientation O>
void LCD<O>::init_sequence()
{
    uint32_t timeout = 100;
    std::span cmds = ili9486_init_seq<O>;
    HAL_SPI_Transmit(spi, const_cast<uint8_t*>(cmds.data()), cmds.size(), timeout);
    // uint32_t i = 0;
    // while (ili9486_init_seq[++i] != TFT_EOF_MARKER) {
    //     write_command(ili9486_init_seq[i]);

    //     if (ili9486_init_seq[i] == TFT_ORIENTATION_CMD)
    //         write_data(ili9486_init_orientations[orientation]);

    //     while (ili9486_init_seq[++i] != TFT_EOL_MARKER) {
    //         if (ili9486_init_seq[i] == TFT_DELAY_MARKER)
    //             delay(ili9486_init_seq[++i]);
    //         else
    //             write_data(ili9486_init_seq[i]);
    //     }
    // }
}

template <Orientation O>
void LCD<O>::ready_region(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    std::array<uint8_t, 20> cmds = {
        // Column addr set.
        0x2A,
        // X start.
        (x >> 8) & 0xFF,
        x & 0xFF,
        // X end.
        ((x + w) >> 8) & 0xFF,
        (x + w) & 0xFF,
        // Row addr set.
        0x2B,
        // Y start.
        (y >> 8) & 0xFF,
        y & 0xFF,
        // Y end.
        ((y + h) >> 8) & 0xFF,
        (y + h) & 0xFF,
        // Memory write.
        0x2C,
    };

    uint32_t timeout = 100;
    HAL_SPI_Transmit(spi, cmds.data(), cmds.size(), timeout);

    // write_command(0x2a); // Column addr set.
    // write_data((x >> 8) & 0xFF);
    // write_data((x)&0xFF); // X START.
    // write_data(((x + w) >> 8) & 0xFF);
    // write_data((x + w) & 0xFF);

    // write_command(0x2b); // Row addr set.
    // write_data((y >> 8) & 0xFF);
    // write_data((y)&0xFF); // Y START.
    // write_data(((y + h) >> 8) & 0xFF);
    // write_data((y + h) & 0xFF);

    // write_command(0x2c);
}

template <Orientation O>
void LCD<O>::clear()
{
    draw_rect(palette.background(), 0, 0, LCD_WIDTH, LCD_HEIGHT);
}

template <Orientation O>
void LCD<O>::draw_pixel(uint16_t color, uint16_t x, uint16_t y)
{
    ready_region(x, y, 1, 1);
    write_data(color >> 8);
    write_data(color & 0xFF);
}

template <Orientation O>
void LCD<O>::draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    draw_rect(palette.foreground(), x, y, w, h);
}

template <Orientation O>
void LCD<O>::draw_rect(uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    ready_region(x, y, w, h);

    // TODO: try 32-bit copying implementation?
    uint32_t nbytes = w * h * 2;
    uint32_t size = std::min(nbytes, BUF_SIZE);
    for (int i = 0; i < size; i += 2) {
        curr_buffer[i] = color >> 8;
        curr_buffer[i + 1] = color & 0xFF;
    }

    spi_wait_finished(spi);
    ready_region(x, y, w, h);

    do {
        uint32_t send_size = std::min(nbytes, size);
        spi_wait_finished(spi);
        HAL_SPI_Transmit_DMA(spi, curr_buffer, send_size);
        // dma_wait();
        // dma_transfer(TFT_SPI.tx_dma, sending_buf->data(), l * Pixel::size());
        nbytes -= send_size;
    } while (nbytes > 0);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_image(const uint8_t* bytes, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    spi_wait_finished(spi);
    ready_region(x, y, w, h);
    HAL_SPI_Transmit_DMA(spi, const_cast<uint8_t*>(bytes), w * h);
}


template <Orientation O>
void LCD<O>::draw_char(char c, uint16_t x, uint16_t y)
{
    uint8_t* bufptr = curr_buffer;
    color_t color_fg = palette.foreground();
    color_t color_bg = palette.background();

    // Consider the current chunk and populate the buffer.
    for (uint16_t iy = 0; iy < CHAR_HEIGHT; iy++) {
        const uint8_t char_ptr = (CHAR_PTR(c))[iy];
        for (int8_t x = CHAR_WIDTH - 1; x >= 0; x--) {
            color_t color = ((char_ptr >> x) & 0x01) ? color_fg : color_bg;
            *bufptr++ = color >> 8;
            *bufptr++ = color & 0xFF;
        }
    }

    draw_image(bufptr, x, y, CHAR_WIDTH, CHAR_HEIGHT);
    swap_buffer();
}

template <Orientation O>
void LCD<O>::draw_string(const char* str, uint16_t x, uint16_t y)
{
    uint32_t len = strlen(str);
    uint8_t* bufptr = curr_buffer;
    color_t color_fg = palette.foreground();
    color_t color_bg = palette.background();
    do {
        uint32_t chunk_chars = std::min(BUF_CHARS, len);

        // Consider the current chunk and populate the buffer.
        for (uint16_t iy = 0; iy < CHAR_HEIGHT; iy++) {
            for (uint8_t is = 0; is < chunk_chars; is++) {
                const uint8_t char_ptr = (CHAR_PTR(str[is]))[iy];
                for (int8_t x = CHAR_WIDTH - 1; x >= 0; x--) {
                    color_t color = ((char_ptr >> x) & 0x01) ? color_fg : color_bg;
                    *bufptr++ = color >> 8;
                    *bufptr++ = color & 0xFF;
                }
            }
        }

        draw_image(bufptr, x, y, CHAR_WIDTH * chunk_chars, CHAR_HEIGHT);

        // Jump to next chunk, prepare for next iter.
        str += chunk_chars;
        len -= chunk_chars;
        swap_buffer();
        bufptr = curr_buffer;
    } while (*str);
}

template class LCD<LCD_ORIENTATION>;
