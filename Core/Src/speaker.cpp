#include "buttons.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "main.h"
#include "profile.hpp"
#include "speaker.hpp"
#include "utils/notes.hpp"

#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 29;


void speaker::init()
{
    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);

    instance().sines.init();

    speaker::play();
}

void speaker::note_on(Note note, uint8_t vel) { instance().sines.note_on(note, vel); }

void speaker::note_off(Note note) { instance().sines.note_off(note); }


void speaker::loop()
{
    extern LCD_ lcd;
    int count = 0;
    for (int i = 0; i < instance().sines.MAX_OSC; i++) {
        if (instance().sines.used[i]) {
            lcd.draw_stringf(0 + 4 * count, 15, "%-3s", notes::get_note_name(instance().sines.notes[i]).data());
            count++;
        }
    }
    if (count == 0) {
        lcd.draw_stringf(0, 15, "%-40s", "");
    }

    // TODO: introduce other instruments/oscillators
    for (size_t i = 0; i < buffer_size; i++) {
        instance().curr_buffer[i] = scale(instance().sines.tick());
    }

    static uint32_t prev_wait_time = 0;
    timeit(wait_time, speaker::send(););
    if (fabs(wait_time - prev_wait_time) > 3) {
        lcd.draw_stringf(57, 19, "%3ld", wait_time);
        prev_wait_time = wait_time;
    }

    instance().adjusted_amplitude = amplitude * volume::get();
}
