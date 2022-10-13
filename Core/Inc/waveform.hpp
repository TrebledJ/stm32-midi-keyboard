#include "main.h"

#include <array>
#include <cmath>
#include <numbers>


#define NOTE_FREQ_TABLE(X) \
    X(C0, 16.35)           \
    X(Db0, 17.32)          \
    X(D0, 18.35)           \
    X(Eb0, 19.45)          \
    X(E0, 20.6)            \
    X(F0, 21.83)           \
    X(Gb0, 23.12)          \
    X(G0, 24.5)            \
    X(Ab0, 25.96)          \
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
    X(C8, 4186.01)         \
    X(Db8, 4434.92)        \
    X(D8, 4698.63)         \
    X(Eb8, 4978.03)        \
    X(E8, 5274.04)         \
    X(F8, 5587.65)         \
    X(Gb8, 5919.91)        \
    X(G8, 6271.93)         \
    X(Ab8, 6644.88)        \
    X(A8, 7040)            \
    X(Bb8, 7458.62)        \
    X(B8, 7902.13)


#define enum_element(a, b) a,
typedef enum { NOTE_FREQ_TABLE(enum_element) NUM_OF_NOTES } Notes;
#undef enum_element

#define init_element(a, b) freq[a] = b;
typedef struct {
    float t;
    uint32_t wav_val;
} WavInfo;

// class Waveform
// {
// public:
//     Waveform() { NOTE_FREQ_TABLE(init_element) }
//     float generate_waveform(Notes note, float t);
//     float freq[NUM_OF_NOTES];

// private:
// };

struct Envelope {
    uint32_t a; // Duration of attack (in ms).
    uint32_t d; // Duration of decay (in ms).
    float s;    // Level of sustain (in %).
    uint32_t r; // Duration of release (in ms).
};

namespace null
{
    inline constexpr Envelope envelope = Envelope{0, 0, 1.0, 0};
}


class Waveform
{
public:
    using type = std::array<uint32_t, 400>;
    type data;

    constexpr Waveform(uint32_t sz = 400) : data_size{sz} {}

    constexpr uint32_t size() const { return data_size; }

private:
    uint32_t data_size;
};

class WaveformTransformer
{
public:
    WaveformTransformer(uint32_t tick, const Waveform& wav, const Envelope& env = null::envelope) : wav{wav}, env{env}
    {
        start = tick;
    }

    void reset(uint32_t tick) { start = tick; }

    /**
     * @brief   Apply the envelope to the waveform and get the data point associated with the tick.
     *          Call this when the waveform is being played.
     */
    uint32_t apply(uint32_t tick)
    {
        uint32_t data = wav.data[get_index()];
        uint32_t t    = tick - start;
        if (t < env.a) {
            // Attack: Interpolate 0 to max level.
            return data * t / env.a;
        } else if (t < env.a + env.d) {
            // Decay: Interpolate max and sustain levels.
            return data * (1 - (t - env.a) * (1.0 - env.s) / env.d);
        } else /* if (t >= env.a + env.d) */ {
            // Sustain.
            return data * env.s;
        }
    }

    /**
     * @brief   Apply the release stage to the waveform. Make sure to call `reset()` before this to reset the tick.
     */
    uint32_t release(uint32_t tick) { return wav.data[get_index()] * (tick - start) * env.s / env.r; }


private:
    uint32_t start;
    uint32_t index = 0;
    const Waveform& wav;
    const Envelope& env;

    /**
     * @brief   Get the current index and advance it to the next position.
     */
    uint32_t get_index()
    {
        if (index + 1 == wav.size()) {
            index = 0;
            return wav.size() - 1;
        } else {
            return index++;
        }
    }
};


namespace generate
{
    inline constexpr uint32_t MAX_AMP = (1 << 12) - 1;

    constexpr Waveform sine(uint16_t freq, double scale = 0.5)
    {
        uint32_t n = 168000 / freq;
        Waveform wav{n};
        for (uint32_t i = 0; i < n; i++) {
            wav.data[i] = (MAX_AMP / 2) + (MAX_AMP / 2) * scale * sin(double(2.0) * double(std::numbers::pi) * i / n);
        }
        return wav;
    }
}; // namespace generate
