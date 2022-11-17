#pragma once

template <typename T>
struct rect {
    T x;
    T y;
    T w;
    T h;
};

using urect = rect<uint16_t>;
