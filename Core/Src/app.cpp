#include "app.hpp"

#include "buttons.hpp"
#include "defines.hpp"
#include "metronome.hpp"
#include "volume.hpp"
#include "waveform.hpp"
extern "C" {
#include "main.h"
#include "math.h"
}

#define PI 3.1415926
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim8;
uint32_t sine_val[1000] = {0};
Waveform wav;
volume vol;
void calcsin(float freq)
{
    for (int i = 0; i < 1000; i++) {
        sine_val[i] = ((float)(sin((i / 1000.0) * 2 * PI * freq) + 1) * (2047.0f)); // 10Hz
        // sine_val[i] = ((float)(sin((i / 1000.0) * 2 * PI * freq) + 1) * vol.get() / 2); // 10Hz
    }
}
Metronome metronome{TIM3};
Buttons button_matrix;

void app_init()
{
    metronome.init();

    calcsin(wav.freq[C4]);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim8);
}
float val    = 1.2;
uint32_t var = (uint32_t)(val * 4096) / 3.3;

void app_run()
{
    while (1) {
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