#pragma once

extern "C" {
#include "main.h"
}
typedef enum {
    BTN_Bottom,
    BTN_Up,
    BTN_D,
    BTN_W,
    BTN_A,
    BTN_S,
    BTN_1_U,
    BTN_1_D,
    BTN_2_U,
    BTN_2_D,
    BTN_3_U,
    BTN_3_D,
    BTN_4_U,
    BTN_4_D,
    BTN_5_U,
    BTN_5_D,
    BTN_6_U,
    BTN_6_D,
    BTN_7_U,
    BTN_7_D,
    BTN_8_U,
    BTN_8_D,
    BTN_9_U,
    BTN_9_D,
    BTN_10_U,
    BTN_10_D,
    BTN_11_U,
    BTN_11_D,
    BTN_12_U,
    BTN_12_D,
    BTN_13_U,
    BTN_13_D,
    BTN_14_U,
    BTN_14_D,
    BTN_15_U,
    BTN_15_D,
    BTN_16_U,
    BTN_16_D,
    BTN_17_U,
    BTN_17_D,
    BTN_18_U,
    BTN_18_D,
    BTN_19_U,
    BTN_19_D,
    BTN_20_U,
    BTN_20_D,
    BTN_21_U,
    BTN_21_D,
    BTN_22_U,
    BTN_22_D,
    BTN_23_U,
    BTN_23_D,
    BTN_24_U,
    BTN_24_D,
    BTN_25_U,
    BTN_25_D,
    BTN_26_U,
    BTN_26_D,
    BTN_27_U,
    BTN_27_D,
    BTN_28_U,
    BTN_28_D,
    BTN_29_U,
    BTN_29_D,
    BTN_Nothing,
} ButtonName;
typedef enum {
    all_released,
    releasing,
    all_pressed,
    pressing,
} ButtonState;

class Buttons
{
public:
    void tick() { btn_matrix = detect_key_matrix(); }
    uint64_t detect_key_matrix();
    void wait_key(int key);
    void key_velocity(uint64_t btn_matrix);
    uint8_t read_button_matrix(ButtonName btn)
    {
        // TODO
        return (btn_matrix >> btn) & 1;
    }

private:
    uint64_t btn_matrix;
    uint8_t delta_t_pressed[29];
    ButtonState key_state[29];
    uint8_t delta_t_released[29];
};