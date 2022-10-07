#pragma once

#include "defines.hpp"
#include "lcd/lcd_defines.hpp"
#include "utils/builder.hpp"

#include <array>
#include <cstdint>


struct Palette {
    DEFINE_BUILDER_MEMBER(color_t, foreground);
    DEFINE_BUILDER_MEMBER(color_t, background);
    DEFINE_BUILDER_MEMBER(color_t, special);
    DEFINE_BUILDER_MEMBER(color_t, highlight);
};


template <Orientation ORIENTATION = LCD_ORIENTATION>
class LCD
{
public:
    Palette palette = Palette().background(BLACK).foreground(WHITE).special(CYAN);

    LCD(SPI_HandleTypeDef* spi) : spi{spi}
    {
        curr_buffer = buffer[0].data();
        next_buffer = buffer[1].data();
    }

    void init()
    {
        init_hardware();
        init_sequence();
    }

    void clear();

    void draw_pixel(color_t color, uint16_t x, uint16_t y);
    void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void draw_rect(color_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void draw_image(const uint8_t* bytes, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void draw_char(char c, uint16_t x, uint16_t y);
    void draw_string(const char* str, uint16_t x, uint16_t y);

private:
    SPI_HandleTypeDef* spi;
    // Orientation orientation;


    using buffer_t = std::array<uint8_t, BUF_SIZE>;
    buffer_t buffer[2];
    uint8_t* curr_buffer;
    uint8_t* next_buffer;


    static_assert(sizeof(color_t) == 2, "Implementation only works for 16-bit colours.");

    void init_hardware()
    {
        spi->Instance->CR1 |= SPI_CR1_SPE; // Enable SPI.
        // TFT_BLK::reset();
        TFT_DC::set();
        TFT_RES::set();
        delay(100);
        TFT_RES::reset();
        delay(100);
        TFT_RES::set();
        delay(100);
        // TFT_BLK::reset();
    }
    void init_sequence();
    void ready_region(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void swap_buffer()
    {
        uint8_t* tmp = curr_buffer;
        curr_buffer  = next_buffer;
        next_buffer  = tmp;
    }

    void write_cmd(uint8_t cmd);
    void write_data(uint8_t data);
};
