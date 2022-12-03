#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
#include "display/menu.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"
#include "profile.hpp"
#include "settings.hpp"
#include "speaker.hpp"
#include "volume.hpp"

extern "C" {
#include "FLASH_SECTOR_F4.h"
#include "main.h"
}

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;

Metronome metronome{&htim3};

std::array<SongData, NUM_SONGS> songs = {

};

MenuController display{songs};
LCD<> lcd{&hspi2};

#define FLASH_ADDR_START 0x0800C000
uint8_t aa[3] = {'A', 'B', 'C'};

void app_init()
{
    metronome.init();
    lcd.init();
    lcd.clear();
    // display.init();
    speaker::init();
    Flash_Write_Bytes(FLASH_ADDR_START, aa, 3);
}
void app_run()
{
    display.draw();
    while (1) {
        kb::loop();
        speaker::loop();
        display.loop();
        settings::update();

        HAL_UART_Transmit_IT(&huart1, aa, sizeof(aa));

        // Tick everything.
        metronome.tick();
        buttons::tick();
        LED0::toggle();
    }
}