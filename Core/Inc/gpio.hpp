#pragma once

extern "C" {
#include "main.h"
}

#ifdef GPIOA_BASE
struct PA { static constexpr uintptr_t base = GPIOA_BASE; };
#endif
#ifdef GPIOB_BASE
struct PB { static constexpr uintptr_t base = GPIOB_BASE; };
#endif
#ifdef GPIOC_BASE
struct PC { static constexpr uintptr_t base = GPIOC_BASE; };
#endif
#ifdef GPIOD_BASE
struct PD { static constexpr uintptr_t base = GPIOD_BASE; };
#endif
#ifdef GPIOE_BASE
struct PE { static constexpr uintptr_t base = GPIOE_BASE; };
#endif
#ifdef GPIOF_BASE
struct PF { static constexpr uintptr_t base = GPIOF_BASE; };
#endif


template <typename PORT, uint16_t PIN>
struct gpio {
    // Inline everything so that code isn't bloated by all the generated template ugliness.

    static void set() { HAL_GPIO_WritePin((GPIO_TypeDef*)PORT::base, (1 << PIN), GPIO_PIN_SET); }
    static void reset() { HAL_GPIO_WritePin((GPIO_TypeDef*)PORT::base, (1 << PIN), GPIO_PIN_RESET); }
    static void write(bool value) { HAL_GPIO_WritePin((GPIO_TypeDef*)PORT::base, (1 << PIN), value ? GPIO_PIN_SET : GPIO_PIN_RESET); }
    static void toggle() { HAL_GPIO_TogglePin((GPIO_TypeDef*)PORT::base, (1 << PIN)); }
    static bool read() { return HAL_GPIO_ReadPin((GPIO_TypeDef*)PORT::base, (1 << PIN)); }
};

template <typename PORT, uint16_t PIN>
struct led : gpio<PORT, PIN> { 
    static void on() { gpio<PORT, PIN>::reset(); }
    static void off() { gpio<PORT, PIN>::set(); }
};
