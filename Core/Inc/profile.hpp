#pragma once

extern "C" {
#include "main.h"
}


namespace profile
{
    struct time {
        uint32_t start;
        time() : start{HAL_GetTick()} {}
        uint32_t operator()() const { return HAL_GetTick() - start; }
    };
} // namespace profile

#define timeit(var, X)                  \
    auto var = ({                       \
        auto elapsed = profile::time(); \
        X;                              \
        elapsed();                      \
    })
// #define timeit(var, X) do { auto var = 0; X; } while (0)
