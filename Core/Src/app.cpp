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
uint8_t aa[4] = {'A', 'B', 'C', '\n'};

void app_init()
{
    // metronome.init();
    lcd.init();
    lcd.clear();
    // display.init();
    speaker::init();
    // kb::init();
    // Flash_Write_Bytes(FLASH_ADDR_START, aa, 3);
}

void app_run()
{
#define SAMPLE_RATE 42000
#define BUFFER_SIZE 1024

    uint16_t buffers[2][BUFFER_SIZE];
    uint8_t curr = 0;
    uint32_t t   = 0;

    HAL_TIM_Base_Start(&htim8);

    while (1) {
        // // Prep the buffer.
        // // uint16_t* buffer = buffers[curr];
        // for (int i = 0; i < BUFFER_SIZE; i++, t++) {
        //     buffer[i] = 2047 * sin(2 * M_PI * 440 * t / SAMPLE_RATE) + 2047;
        //     // buffer[i] = amp * (sin(fA * t) + sin(fCs * t) + sin(fE * t)) + 2047;
        //     // buffer[i] = amp * (sin(fA * t)) + 2047;
        //     // buffer[i] =
        // }

        // // Wait for DAC to be ready.
        // while (HAL_DAC_GetState(&hdac) != HAL_DAC_STATE_READY)
        //     ;

        // // Start the DMA.
        // HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)buffer, BUFFER_SIZE, DAC_ALIGN_12B_R);
        // curr = !curr;

        // lcd.draw_stringf(0, 15, "%8d  %10d   ", curr_buffer[0], t);

        // kb::loop();
        // speaker::loop();
        // display.loop();
        // settings::update();

        // // HAL_UART_Transmit_IT(&huart1, aa, sizeof(aa));

        // // Tick everything.
        // metronome.tick();
        // buttons::tick();
        // LED0::toggle();
    }
}