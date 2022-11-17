#pragma once

extern "C" {
#include "main.h"
}

#include "gpio.hpp"
#include "utils/magic_enum.hpp"

////////////////////////////////////////////////////////////////
/// PROGRAM
////////////////////////////////////////////////////////////////

using string_t     = const char[16];
using stringview_t = std::string_view;

inline constexpr int NUM_SONGS    = 4;
inline constexpr int NUM_CHANNELS = 4;

enum Instrument {
    SINE,
    PIANO,
};

enum PageName {
    HOME,
    SONG,
    CHANNEL,
    EXPORT,
    SETTING,
    EFFECTS,
};

enum Effect {
    DEFAULT,       // Play normally.
    AUTO_CHORD,    // Complete notes based on the pressed notes.
    AUTO_ARPEGGIO, // Like autochord, but alternates between notes.
};

enum Equaliser {
    NO_FILTER,
    LOW_PASS,
    HIGH_PASS,
    LOW_BAND,
    HIGH_BAND,
};


inline constexpr size_t NUM_INSTRUMENTS = magic_enum::enum_count<Instrument>();
inline constexpr auto INSTRUMENT_NAMES  = magic_enum::enum_names<Instrument>();

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
