#include "buttons.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "main.h"
#include "speaker.hpp"
#include "utils/notes.hpp"

#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 29;
leaf::osc::cycle sine[NUM_SINES];
leaf::midi::poly midi_poly;
inline constexpr int NUM_SINES_CH = 2;
leaf::osc::cycle sine_ch[NUM_SINES_CH];
extern LCD<> lcd;
// float chord[] = {440.0, 554.37, 659.25};
float chord[] = {440, 660};


void speaker::init()
{
    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);
    midi_poly.init(4);

    for (int i = 0; i < NUM_SINES; i++) {
        sine[i].init();
        sine[i].setFreq(notes::get_freq(button2note(i)));
        sine[i].setPhase(0);
    }
    for (int i = 0; i < NUM_SINES_CH; i++) {
        sine_ch[i].init();
        sine_ch[i].setFreq(chord[i]);
    }

    speaker::play();
}


void speaker::default_load(bool (&active)[NUM_KEYBOARD_KEYS])
{
    // TODO: introduce other instruments/oscillators
    speaker::load(sine, active);
}


void speaker::loop()
{
    speaker::send();

    instance().adjusted_amplitude = amplitude * volume::get();
}
