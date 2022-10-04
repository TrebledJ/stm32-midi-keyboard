#pragma once

extern "C" {
#include "main.h"
}

#include "gpio.hpp"


// We need to hardcode the ports because we can't pass pointer values as template parameters
// and also STMCubeMX's code gen is rather inflexible.
using LED0 = led<PB, 0>;
using LED1 = led<PC, 4>;
using LED2 = led<PA, 7>;
using LED3 = led<PC, 5>;
