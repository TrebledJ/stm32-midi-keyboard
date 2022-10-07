#include "app.hpp"
#include "defines.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"

extern "C" {
#include "button_matrix.h"
#include "main.h"
}


extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

uint64_t btn_matrix = 0;

Metronome metronome{TIM3};
LCD lcd{&hspi2};


void app_init()
{
    metronome.init();
    lcd.init();
}


void app_run()
{
    lcd.clear();
    // lcd.draw_string("hello world!", 0, 0);
    // lcd.draw_char('X', 1, 1);
    // lcd.draw_rect(GREEN, 200, 200, 50, 50);
    while (1) {
        metronome.tick();
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
