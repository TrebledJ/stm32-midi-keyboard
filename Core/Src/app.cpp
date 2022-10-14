#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"
#include "volume.hpp"
#include "waveform.hpp"

extern "C" {
#include "main.h"
}

#include "leaf.h"

// #define PI 3.1415926
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim8;
uint32_t sine_val[1000] = {0};
Waveform wav;

inline constexpr uint32_t MAX_AMP = (1 << 12) - 1;


Metronome metronome{TIM3};
LCD lcd{&hspi2};
Buttons button_matrix;
LEAF leaf;


#define MEM_SIZE 40000
char leaf_memory[MEM_SIZE];

// #define AUDIO_BUFFER_SIZE (1024*4)
inline constexpr size_t AUDIO_BUFFER_SIZE = 1024;
uint32_t audio_buffer[2][AUDIO_BUFFER_SIZE];

// // uint32_t Wave_LUT[128] = {
// //     2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355, 3431, 3504, 3574,
// 3639,
// //     3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076, 4087, 4094, 4095, 4091, 4082, 4069, 4050,
// 4026,
// //     3998, 3965, 3927, 3884, 3837, 3786, 3730, 3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883,
// 2790,
// //     2695, 2598, 2500, 2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
// 944,
// //     860,  779,  701,  627,  556,  488,  424,  365,  309,  258,  211,  168,  130,  97,   69,   45,   26,   13,   4,
// //     0,    1,    8,    19,   35,   56,   82,   113,  149,  189,  234,  283,  336,  394,  456,  521,  591,  664,
// 740,
// //     820,  902,  987,  1075, 1166, 1258, 1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047};

// // #define N 180
// // Waveform Wave_LUT       = generate::sine(880, 1.0);
// // uint32_t Wave_LUT2[200] = {};

inline constexpr size_t NUM_WAVES     = 2;
std::array<Waveform, NUM_WAVES> waves = {
    generate::sine(880, 1.0),
    generate::sine(440, 1.0),
};

float random_number() { return 0.5; }

inline constexpr int NUM_SINES = 8;
tCycle sine[NUM_SINES];
float freq[] = {440.0, 493.88, 554.37, 587.33, 659.25, 739.99, 830.61, 880};

tCycle sine0;

void app_init()
{
    metronome.init();
    lcd.init();
    lcd.clear();

    LEAF_init(&leaf, 21000, leaf_memory, MEM_SIZE, &random_number);

    tCycle_init(&sine0, &leaf);
    tCycle_setFreq(&sine0, 440.0);
    for (int i = 0; i < NUM_SINES; i++) {
        tCycle_init(&sine[i], &leaf);
        tCycle_setFreq(&sine[i], freq[i]);
    }
}

void app_run()
{
    int sound_mode  = 0;
    int curr_buff_i = 0;
    // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, waves[0].data.data(), waves[0].size(), DAC_ALIGN_12B_R);
    // TODO: make DAC Channel 2 non-circular.
    // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, waves[1].data.data(), waves[1].size(), DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim8);
    while (1) {
        // uint32_t tick = HAL_GetTick();
        constexpr uint32_t AMP = generate::MAX_AMP / 2;
        uint32_t* buffer       = audio_buffer[curr_buff_i];
        for (size_t i = 0; i < AUDIO_BUFFER_SIZE; i++) {
            // buffer[i] = uint32_t((tCycle_tick(&sine) + tCycle_tick(&sine2)) * AMP / 2 + AMP);
            // buffer[i] = uint32_t((tCycle_tick(&sine)) * AMP / 2 + AMP);
            // buffer[i] = uint32_t(tCycle_tick(&sine) + ) * AMP / 2 + AMP);
            float sum = 0.0;
            // for (int s = 0; s <= sound_mode; s++)
            //     sum += tCycle_tick(&sine[s]);
            // buffer[i] = sum * AMP / (sound_mode + 1) + AMP;
            sum       = tCycle_tick(&sine[sound_mode]);
            buffer[i] = sum * AMP + AMP;
        }
        // uint32_t fill_elapsed = HAL_GetTick() - tick;

        extern DMA_HandleTypeDef hdma_dac1;
        while (HAL_DMA_GetState(&hdma_dac1) != HAL_DMA_STATE_READY)
            ; // Wait for DMA stream to be ready.
        // uint32_t wait_elapsed = HAL_GetTick() - tick - fill_elapsed;
        // extern DMA_HandleTypeDef hdma_dac2;
        // while (HAL_DMA_GetState(&hdma_dac2) != HAL_DMA_STATE_READY); // Wait for DMA stream to be ready.

        HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, buffer, AUDIO_BUFFER_SIZE, DAC_ALIGN_12B_R);
        // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, buffer, AUDIO_BUFFER_SIZE, DAC_ALIGN_12B_R);

        curr_buff_i = 1 - curr_buff_i; // Select the other buffer.

        // uint32_t elapsed = HAL_GetTick() - tick;
        // lcd.draw_string(0, 0, "elapsed: %d %d %d", wait_elapsed, fill_elapsed, elapsed);

        static bool prev_pressed = false;
        bool pressed             = button_matrix.is_btn_pressed(BTN_A);
        if (!prev_pressed && pressed) {
            sound_mode = (sound_mode + 1) % NUM_SINES;
        }
        prev_pressed = pressed;


        // metronome.tick();
        // uint32_t tick = HAL_GetTick();
        button_matrix.tick();
        // uint32_t elapsed = HAL_GetTick() - tick;
        // lcd.draw_string(0, 0, "elapsed: %d", elapsed);

        // LED0::toggle();
    }
}