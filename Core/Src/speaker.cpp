#include "speaker.hpp"

#include "buttons.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "main.h"
#include "waveform.hpp"

#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 29;
leaf::osc::cycle sine[NUM_SINES];
inline constexpr int NUM_SINES_CH = 2;
leaf::osc::cycle sine_ch[NUM_SINES_CH];
extern LCD<> lcd;

float freq[88];
// float chord[] = {440.0, 554.37, 659.25};
float chord[] = {440, 660};
uint32_t midi_time_stamp[100];
MIDI_Pkg midi_file[1000];
uint8_t active[NUM_SINES] = {0};

void speaker::init()
{
    NOTE_FREQ_TABLE(init_element);
    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);
    midi_poly.init(4);

    for (int i = 0; i < NUM_SINES; i++) {
        sine[i].init();
        sine[i].setFreq(freq[C4 + i]);
        sine[i].setPhase(0);
    }
    for (int i = 0; i < NUM_SINES_CH; i++) {
        sine_ch[i].init();
        sine_ch[i].setFreq(chord[i]);
    }

    speaker::play();
}

void speaker::loop()
{
    // static int sound_mode = 0;
    int active_count = 0;

    // speaker::load(sine[sound_mode]);
    // if (sound_mode == 0)
    //     speaker::load(sine_ch[0]);
    // else if (sound_mode == 1)
    //     speaker::load(sine_ch[1]);
    // else
    //     speaker::load(sine_ch[0], sine_ch[1]);

    for (int i = 0; i < NUM_SINES; i += 2) {
        active[i / 2] =
            buttons::is_btn_pressed((ButtonName)(i + BTN_1_U)) || buttons::is_btn_pressed((ButtonName)(i + BTN_1_D));
        if (active[i / 2]) {
            active_count++;
        }
    }
    // for (int i = 0; i < 29; i++)
    //     lcd.draw_string(i, 0, "%d", active[i]);
    // lcd.draw_string(0, 1, "%d", active_count);

    // on key released
    speaker::load(sine, active, active_count);
    speaker::send();


    // Testing Purposes: Check state for changing sound.
    // static bool prev_pressed = false;

    // bool pressed = buttons::is_btn_pressed(BTN_A);
    // if (!prev_pressed && pressed) {
    //     // sound_mode = (sound_mode + 1) % NUM_SINES;
    //     sound_mode = (sound_mode + 1) % 3;
    //     if (sound_mode == 2) {
    //         sine_ch[0].setFreq(880);
    //         sine_ch[1].setFreq(1320);
    //     } else {
    //         sine_ch[0].setFreq(440);
    //         sine_ch[1].setFreq(660);
    //     }
    //     sine_ch[0].setPhase(0);
    //     sine_ch[1].setPhase(0);
    // }
    // prev_pressed = pressed;

    // timeit(elapsed, speaker::tick(sine[sound_mode]););
    // timeit(elapsed_send, speaker::send(););

    // timeit(elapsed_loop, {
    //     static bool prev_pressed = false;
    //     bool pressed             = button_matrix.is_btn_pressed(BTN_A);
    //     if (!prev_pressed && pressed) {
    //         sound_mode = (sound_mode + 1) % NUM_SINES;
    //     }
    //     prev_pressed = pressed;


    //     metronome.tick();
    //     button_matrix.tick();
    //     LED0::toggle();
    // });
    // lcd.draw_string(0, 0, "elapsed: %d %d %d", elapsed, elapsed_send, elapsed_loop);
}
