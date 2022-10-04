#pragma once

extern "C" {
#include "main.h"
}


class dial
{
public:
    dial(ADC_HandleTypeDef* adc);

    /**
     * @brief   Gets the value of the dial.
     * @param   timeout Timeout in milliseconds.
     */
    float get_value(uint32_t timeout = 100)
    {
        HAL_ADC_PollForConversion(m_adc, timeout);
        return HAL_ADC_GetValue(&hadc1) / 1.0;
    }

    float test() const
    {
        HAL_ADC_Start(m_adc);
        HAL_ADC_PollForConversion(m_adc, HAL_MAX_DELAY);
        uint32_t raw = HAL_ADC_GetValue(m_adc);
        HAL_ADC_Stop(m_adc);
        return raw;
    }

private:
    ADC_HandleTypeDef* m_adc;
};
