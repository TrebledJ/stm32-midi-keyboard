#include "utils/magic_enum.hpp"

//+21
#define NOTE_FREQ_TABLE(X) \
    X(A0, 27.5)            \
    X(Bb0, 29.14)          \
    X(B0, 30.87)           \
    X(C1, 32.7)            \
    X(Db1, 34.65)          \
    X(D1, 36.71)           \
    X(Eb1, 38.89)          \
    X(E1, 41.2)            \
    X(F1, 43.65)           \
    X(Gb1, 46.25)          \
    X(G1, 49)              \
    X(Ab1, 51.91)          \
    X(A1, 55)              \
    X(Bb1, 58.27)          \
    X(B1, 61.74)           \
    X(C2, 65.41)           \
    X(Db2, 69.3)           \
    X(D2, 73.42)           \
    X(Eb2, 77.78)          \
    X(E2, 82.41)           \
    X(F2, 87.31)           \
    X(Gb2, 92.5)           \
    X(G2, 98)              \
    X(Ab2, 103.83)         \
    X(A2, 110)             \
    X(Bb2, 116.54)         \
    X(B2, 123.47)          \
    X(C3, 130.81)          \
    X(Db3, 138.59)         \
    X(D3, 146.83)          \
    X(Eb3, 155.56)         \
    X(E3, 164.81)          \
    X(F3, 174.61)          \
    X(Gb3, 185)            \
    X(G3, 196)             \
    X(Ab3, 207.65)         \
    X(A3, 220)             \
    X(Bb3, 233.08)         \
    X(B3, 246.94)          \
    X(C4, 261.63)          \
    X(Db4, 277.18)         \
    X(D4, 293.66)          \
    X(Eb4, 311.13)         \
    X(E4, 329.63)          \
    X(F4, 349.23)          \
    X(Gb4, 369.99)         \
    X(G4, 392)             \
    X(Ab4, 415.3)          \
    X(A4, 440)             \
    X(Bb4, 466.16)         \
    X(B4, 493.88)          \
    X(C5, 523.25)          \
    X(Db5, 554.37)         \
    X(D5, 587.33)          \
    X(Eb5, 622.25)         \
    X(E5, 659.25)          \
    X(F5, 698.46)          \
    X(Gb5, 739.99)         \
    X(G5, 783.99)          \
    X(Ab5, 830.61)         \
    X(A5, 880)             \
    X(Bb5, 932.33)         \
    X(B5, 987.77)          \
    X(C6, 1046.5)          \
    X(Db6, 1108.73)        \
    X(D6, 1174.66)         \
    X(Eb6, 1244.51)        \
    X(E6, 1318.51)         \
    X(F6, 1396.91)         \
    X(Gb6, 1479.98)        \
    X(G6, 1567.98)         \
    X(Ab6, 1661.22)        \
    X(A6, 1760)            \
    X(Bb6, 1864.66)        \
    X(B6, 1975.53)         \
    X(C7, 2093)            \
    X(Db7, 2217.46)        \
    X(D7, 2349.32)         \
    X(Eb7, 2489.02)        \
    X(E7, 2637.02)         \
    X(F7, 2793.83)         \
    X(Gb7, 2959.96)        \
    X(G7, 3135.96)         \
    X(Ab7, 3322.44)        \
    X(A7, 3520)            \
    X(Bb7, 3729.31)        \
    X(B7, 3951.07)         \
    X(C8, 4186.01)


#define X(a, b) a,
enum Note { NOTE_FREQ_TABLE(X) };
#undef X

inline constexpr int NUM_NOTES = magic_enum::enum_count<Note>();


class notes
{
public:
    static float get_freq(Note note) { return freq[note]; }

private:
    static constexpr float freq[NUM_NOTES] = {
#define X(a, b) b,
        NOTE_FREQ_TABLE(X)
#undef X
    };
};


#define note2midi(n)   (n + 21)
#define midi2note(n)   (static_cast<Note>(n - 21))
#define note2button(n) (static_cast<ButtonName>(n - C4))
#define button2note(n) (static_cast<Note>(n + C4))
