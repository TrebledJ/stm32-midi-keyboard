#pragma once

#include "defines.hpp"
#include "utils/magic_enum.hpp"
#include "utils/notes.hpp"
#include "utils/tinyvector.hpp"

#include <array>


enum AutoChord {
    AUTOCHORD_NONE,
    AUTOCHORD_PERFECT, // P5 P8
    AUTOCHORD_TRIADS,
    AUTOCHORD_OPEN_CHORD,
};

inline constexpr auto AUTOCHORD_OPTIONS = magic_enum::enum_names<AutoChord>();

namespace autochord
{
    chord autonotes(Note note, AutoChord strategy);
}
