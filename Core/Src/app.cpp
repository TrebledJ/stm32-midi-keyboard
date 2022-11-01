#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
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
LCD lcd{&hspi2};


void app_init()
{
    metronome.init();
    lcd.init();
    lcd.clear();
    speaker::init();
}

void app_run()
{
    while (1) {
        speaker::loop();

        // Print button states.
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                lcd.draw_string(0 + 3 * j, 3 + i, "%d", buttons::is_btn_pressed(static_cast<ButtonName>(i * 8 + j)));

        // Tick everything.
        metronome.tick();
        buttons::tick();
        LED0::toggle();
    }
}