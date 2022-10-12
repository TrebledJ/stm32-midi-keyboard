#pragma once

#include "dial.hpp"


extern ADC_HandleTypeDef hadc1;

class volume
{
public:
    static Dial& instance()
    {
        static Dial dial{&hadc1};
        return dial;
    }

    static float get() { return instance().get_value(); }
};