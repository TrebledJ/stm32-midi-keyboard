#pragma once

extern "C" {
#include "main.h"
}

#include "gpio.hpp"

////////////////////////////////////////////////////////////////
/// PROGRAM
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
/// BOARD
////////////////////////////////////////////////////////////////

// We need to hardcode the ports because we can't pass pointer values as template parameters
// and also STMCubeMX's code gen is rather inflexible.
using LED0 = led<PB, 0>;
using LED1 = led<PC, 4>;
using LED2 = led<PA, 7>;
using LED3 = led<PC, 5>;

using TFT_BLK = gpio<PC, 2>;
using TFT_SDA = gpio<PC, 3>;
using TFT_RES = gpio<PB, 1>;
using TFT_DC  = gpio<PB, 2>;
using TFT_CLK = gpio<PB, 10>;
using TFT_CS  = gpio<PB, 11>;
