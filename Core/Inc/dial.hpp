#pragma once

extern "C" {
#include "main.h"
}


class Dial
{
public:
    static constexpr uint32_t MAX = (1 << 12) - 1; // Max ADC value.

    Dial(ADC_HandleTypeDef* adc) : adc{adc} {}

    /**
     * @brief   Gets the value of the dial.
     */
    uint32_t get_value() const { return test(); }

    uint32_t test() const
    {
        HAL_ADC_Start(adc);
        HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);
        uint32_t raw = HAL_ADC_GetValue(adc);
        HAL_ADC_Stop(adc);
        return raw;
    }

private:
    ADC_HandleTypeDef* adc;
};
