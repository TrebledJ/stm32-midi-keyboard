#include "app.hpp"

#include "metronome.hpp"

extern "C" {
#include "button_matrix.h"
#include "main.h"
}


extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

uint32_t raw = 0;

uint64_t btn_matrix = 0;
uint8_t last_btn = 0;
float volume = 0;


volatile uint32_t last_ticks;
float get_volume()
{
    uint32_t knob_reading = 0;
    HAL_ADC_PollForConversion(&hadc1, 100);
    knob_reading = HAL_ADC_GetValue(&hadc1);
    return knob_reading / 1.0;
}

void app_init()
{
    metronome::init();
    last_ticks = HAL_GetTick();
}


void app_run()
{
    while (1) {
        metronome::tick(last_ticks, 80, 3);
        btn_matrix = detect_key_matrix();
        if (read_key_matrix(BTN_1_U)) {
            gpio_reset(LED0);
        } else {
            gpio_set(LED0);
        }
        if (read_key_matrix(BTN_1_D)) {
            gpio_reset(LED1);
        } else {
            gpio_set(LED1);
        }
        //	  gpio_toggle(LED2);
        //	  delay(499);
        //	volume = get_volume();
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        raw = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
    }
}
