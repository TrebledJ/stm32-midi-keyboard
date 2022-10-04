#pragma once

extern "C" {
#include "main.h"
}


class Dial
{
public:
    Dial(ADC_HandleTypeDef* adc) : adc{adc} {}

    /**
     * @brief   Gets the value of the dial.
     * @param   timeout Timeout in milliseconds.
     */
    float get_value(uint32_t timeout = 100)
    {
        HAL_ADC_PollForConversion(adc, timeout);
        return HAL_ADC_GetValue(adc) / 1.0;
    }

    float test() const
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
