#pragma once

#include "autochord.hpp"
#include "speaker.hpp"
#include "utils/notes.hpp"

#include <cstdint>


struct settings {
    int32_t transpose   = 0;
    AutoChord autochord = AUTOCHORD_NONE;
    DiatonicNote key    = C;


    settings() = default;

    static settings& curr()
    {
        static settings s;
        return s;
    }

    static settings& prev()
    {
        static settings s;
        return s;
    }

    static void load(const settings& s) { curr() = s; }
    static void update();

    static void set_transpose(int32_t transpose) { curr().transpose = transpose; }
    static void set_autochord(uint8_t autochord) { curr().autochord = static_cast<AutoChord>(autochord); }
    static void set_key(uint8_t key) { curr().key = static_cast<DiatonicNote>(key); }

private:
    settings(const settings&) = default;
    settings(settings&&)      = delete;
    settings& operator=(const settings&) = default;
    settings& operator=(settings&&) = delete;
};
