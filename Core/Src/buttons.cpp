#include "buttons.hpp"


static GPIO_TypeDef* Btn_Row_Port[] = {btn_table(row_port)};
static const uint16_t Btn_Row_Pin[] = {btn_table(row_pin)};

static GPIO_TypeDef* Btn_Col_Port[] = {btn_table(col_port)};
static const uint16_t Btn_Col_Pin[] = {btn_table(col_pin)};


uint64_t Buttons::detect_key_matrix()
{
    uint64_t result = 0;
    for (int row = 0; row < 8; row++) {
        btn_table(row_set);
        HAL_GPIO_WritePin(Btn_Row_Port[row], Btn_Row_Pin[row], GPIO_PIN_RESET);
        for (int col = 0; col < 8; col++) {
            if (!HAL_GPIO_ReadPin(Btn_Col_Port[col], Btn_Col_Pin[col])) {
                result = result | 1 << (row * 8 + col);
            }
        }
    }
    return result;
}

void Buttons::wait_key(int key)
{
    int col = key % 8;
    int row = key / 8;
    btn_table(row_set);
    HAL_GPIO_WritePin(Btn_Row_Port[row], Btn_Row_Pin[row], GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(Btn_Col_Port[col], Btn_Col_Pin[col])) {
        delay(1);
    }
    return;
}

void Buttons::key_velocity(uint64_t btn_matrix)
{
    for (int i = 6; i < 64; i += 2) {
        int keys_detecing = (i - 6) / 2;
        uint8_t key_up    = (btn_matrix >> (i)) & 1;
        uint8_t key_down  = (btn_matrix >> (i + 1)) & 1;
        if (key_down && !key_up && key_state[keys_detecing] == all_released) { //(0x)
            delta_t_pressed[keys_detecing] = get_ticks();
            key_state[keys_detecing]       = pressing;
        }
        if (key_up && key_down && key_state[keys_detecing] == pressing) { //(xx)
            delta_t_pressed[keys_detecing] -= get_ticks();
            key_state[keys_detecing] = all_pressed;
            // maybe signal the sound play and send midi file
        }
        if (key_down && !key_up && key_state[keys_detecing] == all_pressed) { //(0x)
            delta_t_released[keys_detecing] = get_ticks();
            key_state[keys_detecing]        = releasing;
        }
        if (!key_down && !key_up && key_state[keys_detecing] == releasing) { //(00)
            delta_t_released[keys_detecing] -= get_ticks();
            key_state[keys_detecing] = all_released;
            // maybe signal the sound play and send midi file
        }
    }
}
