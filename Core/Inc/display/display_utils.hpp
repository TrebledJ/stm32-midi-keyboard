#pragma once

#include "lcd/lcd.hpp"

// Call and return true;
#define truef(...)   \
    ({               \
        __VA_ARGS__; \
        true;        \
    })

#define GET_MACRO3(_1, _2, _3, NAME, ...) NAME


struct scoped_foreground {
    color_t after;
    scoped_foreground(color_t c1) : after{LCD_::default_palette.foreground()}
    {
        extern LCD_ lcd;
        lcd.palette.foreground(c1);
    }
    scoped_foreground(color_t c1, color_t c2) : after{c2}
    {
        extern LCD_ lcd;
        lcd.palette.foreground(c1);
    }
    ~scoped_foreground()
    {
        extern LCD_ lcd;
        lcd.palette.foreground(after);
    }
};

#define with_bg(COLOR) if ([[maybe_unused]] auto _tmp = scoped_background((COLOR)); true)

#define with_fg_if(...) GET_MACRO3(__VA_ARGS__, with_fg_if3, with_fg_if2)(__VA_ARGS__)

#define with_fg_if2(COND, THEN_COLOR) \
    if ([[maybe_unused]] auto _tmp = scoped_foreground((COND) ? THEN_COLOR : LCD_::default_palette.foreground()); true)

#define with_fg_if3(COND, THEN_COLOR, ELSE_COLOR) \
    if ([[maybe_unused]] auto _tmp = scoped_foreground((COND) ? THEN_COLOR : ELSE_COLOR); true)


struct scoped_background {
    color_t after;
    scoped_background(color_t c1) : after{LCD_::default_palette.background()}
    {
        extern LCD_ lcd;
        lcd.palette.background(c1);
    }
    scoped_background(color_t c1, color_t c2) : after{c2}
    {
        extern LCD_ lcd;
        lcd.palette.background(c1);
    }
    ~scoped_background()
    {
        extern LCD_ lcd;
        lcd.palette.background(after);
    }
};

#define with_bg(COLOR) if ([[maybe_unused]] auto _tmp = scoped_background((COLOR)); true)

#define with_bg_if(...) GET_MACRO3(__VA_ARGS__, with_bg_if3, with_bg_if2)(__VA_ARGS__)

#define with_bg_if2(COND, THEN_COLOR) with_bg((COND) ? THEN_COLOR : LCD_::default_palette.background())

#define with_bg_if3(COND, THEN_COLOR, ELSE_COLOR) with_bg((COND) ? THEN_COLOR : ELSE_COLOR)


template <size_t MAX, typename T = uint16_t>
struct clamped_int {
    using type = T;
    T data;

    template <typename U>
    clamped_int(U x = 0) : data{static_cast<T>(x)}
    {
    }

    constexpr T& operator++() { return data = (data + 1 == MAX ? data : data + 1); }
    constexpr T operator++(int)
    {
        T tmp = data;
        ++(*this);
        return tmp;
    }
    constexpr T& operator--() { return data = (data == 0 ? data : data - 1); }
    constexpr T operator--(int)
    {
        T tmp = data;
        --(*this);
        return tmp;
    }
    constexpr T& operator()() { return data; }

    constexpr clamped_int& operator=(int x)
    {
        data = x;
        return *this;
    }
    operator int() { return data; }
};

#define clamped_int_bin_op(OP)                           \
    template <size_t N, typename T, typename U>          \
    constexpr bool operator OP(clamped_int<N, T> a, U b) \
    {                                                    \
        return a.data OP b;                              \
    }

clamped_int_bin_op(==);
clamped_int_bin_op(!=);
clamped_int_bin_op(<);
clamped_int_bin_op(<=);
clamped_int_bin_op(>);
clamped_int_bin_op(>=);
