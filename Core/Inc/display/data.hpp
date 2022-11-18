#pragma once

#include "defines.hpp"


struct ChannelData {
    static constexpr uint8_t MUTE = 0x01; // TODO: aggregate this in all song data?
    static constexpr uint8_t SOLO = 0x02;

    Instrument instrument = SINE;
    uint8_t flags         = 0;
    uint8_t volume        = 50;

    bool is_mute() const { return flags & MUTE; }
    bool is_solo() const { return flags & SOLO; }
};

struct SongData {
    ChannelData channel[NUM_CHANNELS];
};
