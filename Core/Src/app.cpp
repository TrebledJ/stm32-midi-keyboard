#include "app.hpp"
#include "buttons.hpp"
#include "defines.hpp"
#include "display/menu.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"
#include "profile.hpp"
#include "speaker.hpp"
#include "volume.hpp"
extern "C" {
#include "FLASH_SECTOR_F4.h"
#include "main.h"
}

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

Metronome metronome{TIM3};

std::array<SongData, NUM_SONGS> songs = {

};

MenuController display{songs};
LCD<> lcd{&hspi2};

#define FLASH_ADDR_START 0x0800C000
uint8_t aa[3]     = {69, 123, 5};
uint8_t rx_buf[3] = {0};

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

        // Flash_Read_Bytes(FLASH_ADDR_START, rx_buf, 3);
        // lcd.draw_stringf(0, 0, "%d %d %d", rx_buf[0], rx_buf[1], rx_buf[2]);

        // Print button states.
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                lcd.draw_stringf(0 + 3 * j, 3 + i, "%d", buttons::is_btn_pressed(static_cast<ButtonName>(i * 8 + j)));

        // Tick everything.
        metronome.tick();
        buttons::tick();
        LED0::toggle();
    }
}