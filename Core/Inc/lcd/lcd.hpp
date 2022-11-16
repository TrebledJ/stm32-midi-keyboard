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

    void draw_pixel(uint16_t x, uint16_t y, color_t color);
    void draw_hline(uint16_t x0, uint16_t y0, uint16_t x1, color_t color);
    void draw_vline(uint16_t x0, uint16_t y0, uint16_t y1, color_t color);
    void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color);
    void draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* bytes);
    void draw_char(uint16_t x, uint16_t y, char c);
    void draw_string(uint16_t x, uint16_t y, const char* str);
    void draw_stringf(uint16_t x, uint16_t y, const char* fmt, ...) __attribute__((format(printf, 4, 5)));

private:
    SPI_HandleTypeDef* spi;

    using buffer_t = std::array<uint8_t, BUF_SIZE>;
    buffer_t buffer[2];
    uint8_t* curr_buffer;
    uint8_t* next_buffer;

    void init_hardware()
    {
        TFT_RES::reset();
        delay(100);
        TFT_RES::set();
        delay(100);
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

    void buf_color(color_t color, size_t n);
};
