#pragma once

#include "dial.hpp"
#include "utils/math.hpp"


extern ADC_HandleTypeDef hadc1;

class volume
{
public:
    static Dial& instance()
    {
        static Dial dial{&hadc1};
        return dial;
    }

    // Gets the volume in the range [0.0, 1.0f].
    static float get()
    {
        constexpr uint32_t leeway = 30;

        // Clamp it to reduce noise along maximums.
        uint32_t val = clamp(instance().get_value(), leeway, Dial::MAX - leeway);

        // Avoid noise by reducing bit-depth.
        val >>= 4;

        return 1.0 * val / (Dial::MAX >> 4);
    }
};