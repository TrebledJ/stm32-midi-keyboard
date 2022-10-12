#include "app.hpp"

#include "buttons.hpp"
#include "defines.hpp"
#include "lcd/lcd.hpp"
#include "metronome.hpp"
#include "volume.hpp"
#include "waveform.hpp"
extern "C" {
#include "main.h"
#include "math.h"
}

#define PI 3.1415926
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim8;
uint32_t sine_val[1000] = {0};
Waveform wav;
volume vol;
void calcsin(float freq)
{
    for (int i = 0; i < 1000; i++) {
        // sine_val[i] = ((float)(sin((i / 1000.0) * 2 * PI * freq) + 1) * (2047.0f)); // 10Hz
        sine_val[i] = ((float)(sin((i / 1000.0) * 2 * PI * freq) + 1) * (vol.get() / 2.0)); // 10Hz
    }
}
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
        calcsin(wav.freq[C4]);
        HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
        HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_val, 1000, DAC_ALIGN_12B_R);
        HAL_TIM_Base_Start(&htim8);
        metronome.tick();

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                lcd.draw_string(0 + 3 * j, 3 + i, "%d",
                                button_matrix.is_btn_pressed(static_cast<ButtonName>(i * 8 + j)));

        // HAL_GPIO_WritePin(row4_GPIO_Port, row4_Pin, GPIO_PIN_RESET);

        // lcd.draw_string(3, 3 + 0, "%d", !HAL_GPIO_ReadPin(col0_GPIO_Port, col0_Pin));
        // lcd.draw_string(3, 3 + 1, "%d", !HAL_GPIO_ReadPin(col1_GPIO_Port, col1_Pin));
        // lcd.draw_string(3, 3 + 2, "%d", !HAL_GPIO_ReadPin(col2_GPIO_Port, col2_Pin));
        // lcd.draw_string(3, 3 + 3, "%d", !HAL_GPIO_ReadPin(col3_GPIO_Port, col3_Pin));
        // lcd.draw_string(3, 3 + 4, "%d", !HAL_GPIO_ReadPin(col4_GPIO_Port, col4_Pin));
        // lcd.draw_string(3, 3 + 5, "%d", !HAL_GPIO_ReadPin(col5_GPIO_Port, col5_Pin));
        // lcd.draw_string(3, 3 + 6, "%d", !HAL_GPIO_ReadPin(col6_GPIO_Port, col6_Pin));
        // lcd.draw_string(3, 3 + 7, "%d", !HAL_GPIO_ReadPin(col7_GPIO_Port, col7_Pin));
        button_matrix.tick();
        // if (button_matrix.is_btn_pressed(BTN_22_U)) {
        //     LED0::on();
        // } else {
        //     LED0::off();
        // }
        // if (button_matrix.is_btn_pressed(BTN_22_D)) {
        //     LED1::on();
        // } else {
        //     LED1::off();
        // }
        gpio_toggle(LED2);
    }
}