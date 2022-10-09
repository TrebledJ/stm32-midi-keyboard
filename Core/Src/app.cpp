#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"

extern "C" {
#include "main.h"
}


extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

Metronome metronome{TIM3};
LCD lcd{&hspi2};
Buttons button_matrix;


void app_init()
{
    metronome.init();
    lcd.init();
}

void app_run()
{
    lcd.clear();
    lcd.draw_rect(200, 200, 50, 50, GREEN);
    lcd.draw_char(1, 1, 'X');
    lcd.draw_string(0, 0, "hello world!");
    lcd.draw_string(0, 0, "%d + %d = %d", 1, 2, 3);
    while (1) {
        metronome.tick();
        button_matrix.tick();
        if (button_matrix.is_btn_pressed(BTN_2_U)) {
            LED0::on();
        } else {
            LED0::off();
        }
        if (button_matrix.is_btn_pressed(BTN_2_D)) {
            LED1::on();
        } else {
            LED1::off();
        }
        //	  LED2::toggle();
        //	  delay(499);
    }
}