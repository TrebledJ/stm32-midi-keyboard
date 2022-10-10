#include "app.hpp"

#include "defines.hpp"
#include "metronome.hpp"
#include "waveform.hpp"

extern "C" {
#include "buttons.hpp"
#include "main.h"
#include "math.h"
}

#define PI 3.1415926
extern DAC_HandleTypeDef hdac;
extern DAC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim8;
uint32_t sine_val[1000] = {0};
Waveform wav;
void calcsin(float freq)
{
    for (int i = 0; i < 1000; i++) {
        sine_val[i] = ((float)(sin((i / 1000.0) * 2 * PI * freq) + 1) * (2047.0f)); // 10Hz
    }
}
#define NS 128
uint32_t Wave_LUT[NS] = {
    2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355, 3431, 3504, 3574, 3639,
    3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076, 4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026,
    3998, 3965, 3927, 3884, 3837, 3786, 3730, 3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790,
    2695, 2598, 2500, 2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031, 944,
    860,  779,  701,  627,  556,  488,  424,  365,  309,  258,  211,  168,  130,  97,   69,   45,   26,   13,   4,
    0,    1,    8,    19,   35,   56,   82,   113,  149,  189,  234,  283,  336,  394,  456,  521,  591,  664,  740,
    820,  902,  987,  1075, 1166, 1258, 1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047};
Metronome metronome{TIM3};
Buttons button_matrix;

void app_init()
{
    metronome.init();

    calcsin(wav.freq[C4]);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
    // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)Wave_LUT, 128, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim8);
}
float val    = 1.2;
uint32_t var = (uint32_t)(val * 4096) / 3.3;

void app_run()
{
    while (1) {
        // HAL_TIM_Base_Start(&htim8);
        // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, sine_val, 100, DAC_ALIGN_12B_R);

        metronome.tick();
        button_matrix.tick();
        if (button_matrix.read_button_matrix(BTN_2_U)) {
            LED0::on();
        } else {
            LED0::off();
        }
        if (button_matrix.read_button_matrix(BTN_2_D)) {
            LED1::on();
        } else {
            LED1::off();
        }
        //	  LED2.toggle();
        //	  delay(499);
    }
}