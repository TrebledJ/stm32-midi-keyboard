#pragma once

#include "display/display_utils.hpp"
#include "utils/shapes.hpp"

#include <functional>


class ComboSelectWidget
{
public:
    using handler = std::function<void(uint8_t)>;

    ComboSelectWidget(const stringview_t* options, uint8_t max) : options{options}, max{max} {}

    void on_value_changed(handler&& h) { m_on_value_changed = h; }
    void set_index(uint8_t index)
    {
        this->curr = (index >= max ? max - 1 : index);
        update();
    }
    void select_prev()
    {
        curr = (curr == 0 ? max - 1 : curr - 1);
        update();
    }
    void select_next()
    {
        curr = (curr + 1 == max ? 0 : curr + 1);
        update();
    }

    void draw(const urect& bounds, bool force = false)
    {
        extern LCD_ lcd;
        if (force || prev != curr) {
            prev = curr;
            lcd.draw_stringf(bounds.x / CHAR_WIDTH, bounds.y / CHAR_HEIGHT, "%-20s", options[curr].data());
        }
    }

private:
    const stringview_t* options;
    uint8_t prev;
    uint8_t curr;
    uint8_t max;

    handler m_on_value_changed;

    void update()
    {
        if (prev != curr && m_on_value_changed) {
            m_on_value_changed(curr);
        }
    }
};


class ToggleWidget
{
public:
    using handler = std::function<void(bool)>;

    ToggleWidget(bool state = false) : state{state} {}

    void on_value_changed(handler&& h) { m_on_value_changed = h; }
    void toggle()
    {
        state = !state;
        if (m_on_value_changed)
            m_on_value_changed(state);
    }
    void set_state(bool state)
    {
        if (this->state != state) {
            this->state = state;
            m_on_value_changed(state);
        }
    }

    void draw(const urect& bounds, bool force = false)
    {
        extern LCD_ lcd;
        if (force || prev_state != state) {
            prev_state = state;
            state ? lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, GREEN)
                  : lcd.draw_rect(bounds.x + 1, bounds.y + 1, bounds.w - 2, bounds.h - 2, RED);
        }
    }

private:
    bool state;
    bool prev_state;

    handler m_on_value_changed;
};


class SliderWidget
{
public:
    using handler = std::function<void(int32_t)>;

    SliderWidget(int32_t value = 0, int32_t inc = 1) : m_value{value}, inc{inc} {}

    int32_t value() const { return m_value; }

    void on_value_changed(handler&& h) { m_on_value_changed = h; }

    void set_range(int32_t min, int32_t max)
    {
        this->min = min;
        this->max = max;
    }
    void set_value(int32_t value)
    {
        if (m_value != value) {
            m_value = value;
            if (m_on_value_changed)
                m_on_value_changed(m_value);
        }
    }
    void set_inc(int32_t inc) { this->inc = inc; }
    void up()
    {
        if (m_value == max)
            return;

        m_value = std::min(m_value + inc, max);
        if (m_on_value_changed)
            m_on_value_changed(m_value);
    }
    void down()
    {
        if (m_value == min)
            return;

        m_value = std::max(m_value - inc, min);
        if (m_on_value_changed)
            m_on_value_changed(m_value);
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

    handler m_on_value_changed;
};
