#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
#include "lcd/lcd.hpp"
#include "leaf.hpp"
#include "metronome.hpp"
#include "profile.hpp"
#include "speaker.hpp"
#include "volume.hpp"
#include "waveform.hpp"

extern "C" {
#include "main.h"
}

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

Metronome metronome{TIM3};
LCD lcd{&hspi2};
Buttons button_matrix;


#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 8;
leaf::osc::cycle sine[NUM_SINES];

inline constexpr int NUM_SINES_CH = 2;
leaf::osc::cycle sine_ch[NUM_SINES_CH];

float freq[] = {440.0, 493.88, 554.37, 587.33, 659.25, 739.99, 830.61, 880};
// float chord[] = {440.0, 554.37, 659.25};
float chord[] = {440, 660};

void app_init()
{
    metronome.init();
    lcd.init();
    lcd.clear();

    leaf::init(21000, leaf_memory, MEM_SIZE, &random_number);

    for (int i = 0; i < NUM_SINES; i++) {
        sine[i].init();
        sine[i].setFreq(freq[i]);
    }
    for (int i = 0; i < NUM_SINES_CH; i++) {
        sine_ch[i].init();
        sine_ch[i].setFreq(chord[i]);
    }

    speaker::play();
}

void app_run()
{
    int sound_mode = 0;
    while (1) {
        // speaker::load(sine[sound_mode]);
        if (sound_mode == 0)
            speaker::load(sine_ch[0]);
        else if (sound_mode == 1)
            speaker::load(sine_ch[1]);
        else
            speaker::load(sine_ch[0], sine_ch[1]);
        speaker::send();

        static bool prev_pressed = false;

        bool pressed = button_matrix.is_btn_pressed(BTN_A);
        if (!prev_pressed && pressed) {
            // sound_mode = (sound_mode + 1) % NUM_SINES;
            sound_mode = (sound_mode + 1) % 3;
            if (sound_mode == 2) {
                sine_ch[0].setFreq(880);
                sine_ch[1].setFreq(1320);
            } else {
                sine_ch[0].setFreq(440);
                sine_ch[1].setFreq(660);
            }
            sine_ch[0].setPhase(0);
            sine_ch[1].setPhase(0);
        }
        prev_pressed = pressed;

        metronome.tick();
        button_matrix.tick();
        LED0::toggle();

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
}