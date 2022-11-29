#pragma once


template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
T clamp(T x, T a, T b)
{
    return min(max(x, a), b);
}
