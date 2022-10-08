#include "app.hpp"

#include "defines.hpp"
#include "metronome.hpp"

extern "C" {
#include "buttons.hpp"
#include "main.h"
}


extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

uint64_t btn_matrix = 0;

Metronome metronome{TIM3};
Buttons button_matrix;

void app_init()
{
    metronome.init();
}


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
