#include "lcd/lcd.hpp"
#include "lcd/lcd_font.h"
#include "utils/utils.hpp"

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
    HAL_SPI_Transmit(spi, cmds.data(), cmds.size(), timeout);
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
void LCD<O>::draw_pixel(uint16_t color, uint16_t x, uint16_t y)
{
    ready_region(x, y, 1, 1);
    write_data(color >> 8);
    write_data(color & 0xFF);
}

template <Orientation O>
void LCD<O>::draw_rectangle(uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    ready_region(x, y, w, h);

    // TODO: try 32-bit copying implementation?
    uint32_t nbytes = w * h * 2;
    uint32_t size = std::min(nbytes, buf_size);
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
    HAL_SPI_Transmit_DMA(spi, bytes, w * h);
}


template <Orientation O>
void LCD<O>::draw_char(char c, color_t color, uint16_t x, uint16_t y)
{
    spi_wait_finished(spi);
    ready_region(CHAR_WIDTH * x, CHAR_HEIGHT * y, CHAR_WIDTH, CHAR_HEIGHT);

    uint32_t nbytes = CHAR_WIDTH * CHAR_HEIGHT * sizeof(color_t);
    uint16_t i = 0; // Index along character image.
    do {
        // Populate buffer.
        uint32_t size = std::min(nbytes, buf_size);
        for (int j = 0; j < size; i++, j += 2) {
            curr_buffer[j] = color >> 8;
            curr_buffer[j + 1] = color & 0xFF;
        }

        // TX buffer.
        spi_wait_finished(spi);
        HAL_SPI_Transmit_DMA(spi, curr_buffer, size);

        // Swap buffer so that we can continue populating next iteration.
        swap_buffer();
        nbytes -= size;
    } while (nbytes > 0);
}

template <Orientation O>
void LCD<O>::draw_string(const char* str, color_t color, uint16_t x, uint16_t y)
{
    for (uint16_t ix = x; *str && ix < LCD_WIDTH / CHAR_WIDTH; str++, ix++) {
        draw_char(*str, color, i, y);
    }
    // TODO: optimise for sending multiple chars.
}

extern template class LCD<LCD_ORIENTATION>;
