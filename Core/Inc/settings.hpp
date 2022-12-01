#pragma once

#include "speaker.hpp"

#include <cstdint>


struct settings {
    int32_t transpose = 0;

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

private:
    settings(const settings&) = default;
    settings(settings&&)      = delete;
    settings& operator=(const settings&) = default;
    settings& operator=(settings&&) = delete;
};
