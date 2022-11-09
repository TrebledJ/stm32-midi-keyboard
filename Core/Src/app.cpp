#include "app.hpp"

#include "buttons.hpp"
#include "defines.hpp"
#include "display.hpp"
#include "lcd/lcd.hpp"
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

Menu display;
void app_init()
{
    metronome.init();
    display.init();
    speaker::init();
}

void app_run()
{
    while (1) {
        speaker::loop();
        display.tick();
        // for (int i = 0; i < 8; i++)
        //     for (int j = 0; j < 8; j++)
        //         display.lcd.draw_string(0 + 3 * j, 3 + i, "%d%d",
        //                                 buttons::is_btn_pressed(static_cast<ButtonName>(i * 8 + j)),
        //                                 buttons::is_btn_just_pressed(static_cast<ButtonName>(i * 8 + j)));

        // Tick everything.
        if (buttons::is_btn_just_pressed(BTN_Bottom)) {
            display.minus();
        }
        if (buttons::is_btn_just_pressed(BTN_Up)) {
            display.plus();
        }
        if (buttons::is_btn_just_pressed(BTN_W)) {
            display.move_up();
            LED1::toggle();
        }
        if (buttons::is_btn_just_pressed(BTN_A)) {
            display.exit();
        }
        if (buttons::is_btn_just_pressed(BTN_S)) {
            display.move_down();
            LED0::toggle();
        }
        if (buttons::is_btn_just_pressed(BTN_D)) {
            display.enter();
        }
        metronome.tick();
        buttons::tick();
        LED0::toggle();
    }
}