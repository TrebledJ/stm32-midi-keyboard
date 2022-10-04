#pramga once

#include "dial.hpp"


extern ADC_HandleTypeDef hadc1;

class volume
{
public:
    volume() : dial(&hadc1) {}

    float get() const { return get_value(); }
};