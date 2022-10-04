#include "app.hpp"

#include "defines.hpp"
#include "metronome.hpp"

extern "C" {
#include "button_matrix.h"
#include "main.h"
}


extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

uint64_t btn_matrix = 0;
uint8_t last_btn = 0;

volatile uint32_t last_ticks;

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
            LED0::on();
        } else {
            LED0::off();
        }
        if (read_key_matrix(BTN_1_D)) {
            LED1::on();
        } else {
            LED1::off();
        }
        //	  LED2.toggle();
        //	  delay(499);
    }
}
