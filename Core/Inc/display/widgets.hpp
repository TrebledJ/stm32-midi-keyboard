#pragma once

#include "display/display_utils.hpp"
#include "utils/shapes.hpp"


class ComboSelectWidget
{
public:
    ComboSelectWidget(const stringview_t* options, uint8_t max) : options{options}, max{max} {}

    void set_index(uint8_t index) { this->curr = (index >= max ? max - 1 : index); }
    void select_prev() { curr = (curr == 0 ? max - 1 : curr - 1); }
    void select_next() { curr = (curr + 1 == max ? 0 : curr + 1); }

    void draw(const urect& bounds, bool force = false)
    {
        extern LCD_ lcd;
        if (force || prev != curr) {
            prev = curr;
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "%-10s", options[curr].data());
        }
    }

private:
    const stringview_t* options;
    uint8_t prev;
    uint8_t curr;
    uint8_t max;
};


class ToggleWidget
{
public:
    bool state;
    bool prev_state;

    ToggleWidget(bool state = false) : state{state} {}

    void toggle() { state = !state; }
    void set_state(bool state) { this->state = state; }

    void draw(const urect& bounds, bool force = false)
    {
        extern LCD_ lcd;
        if (force || prev_state != state) {
            prev_state = state;
            state ? lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, GREEN)
                  : lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, RED);
        }
    }
};


class SliderWidget
{
public:
    SliderWidget(int32_t value = 0, int32_t inc = 1) : m_value{value}, inc{inc} {}

    int32_t value() const { return m_value; }

    void set_range(int32_t min, int32_t max)
    {
        this->min = min;
        this->max = max;
    }
    void set_value(int32_t value) { m_value = value; }
    void set_inc(int32_t inc) { this->inc = inc; }
    void up()
    {
        m_value += inc;
        if (m_value > max)
            m_value = max;
    }
    void down()
    {
        m_value -= inc;
        if (m_value < min)
            m_value = min;
    }

    void draw(const urect& bounds, bool force = false)
    {
        extern LCD_ lcd;
        if (force || prev_value != m_value) {
            prev_value = m_value;

            float prog = 1.0 * (m_value - min) / (max - min);
            lcd.draw_rect(bounds.x, bounds.y, bounds.w * prog, bounds.h, lcd.palette.foreground());
            lcd.draw_rect(bounds.x + bounds.w * prog, bounds.y, bounds.w * (1 - prog), bounds.h, WHITE);
        }
    }

private:
    int32_t m_value = 50;
    int32_t prev_value;
    int32_t inc = 1;
    int32_t min = 0;
    int32_t max = 100;
};
