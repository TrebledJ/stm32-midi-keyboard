#include "buttons.hpp"


#define row_port(p) row##p##_GPIO_Port,
#define row_pin(p)  row##p##_Pin,

#define col_port(p) col##p##_GPIO_Port,
#define col_pin(p)  col##p##_Pin,

#define row_set(p) HAL_GPIO_WritePin(row##p##_GPIO_Port, row##p##_Pin, GPIO_PIN_SET);

#define btn_table(X) \
    X(0)             \
    X(1)             \
    X(2)             \
    X(3)             \
    X(4)             \
    X(5)             \
    X(6)             \
    X(7)

static GPIO_TypeDef* btn_row_port[] = {btn_table(row_port)};
static const uint16_t btn_row_pin[] = {btn_table(row_pin)};

static GPIO_TypeDef* btn_col_port[] = {btn_table(col_port)};
static const uint16_t btn_col_pin[] = {btn_table(col_pin)};


uint64_t Buttons::detect_key_matrix()
{
    uint64_t result = 0;
    btn_table(row_set);
    for (int row = 0; row < 8; row++) {
        HAL_Delay(1);
        HAL_GPIO_WritePin(btn_row_port[row], btn_row_pin[row], GPIO_PIN_RESET);
        for (int col = 0; col < 8; col++) {
            if (!HAL_GPIO_ReadPin(btn_col_port[col], btn_col_pin[col])) {
                result |= 1ULL << (row * 8 + col);
            }
        }
        HAL_GPIO_WritePin(btn_row_port[row], btn_row_pin[row], GPIO_PIN_SET);
    }
    return result;
}

void Buttons::wait_key(int key)
{
    int col = key % 8;
    int row = key / 8;
    btn_table(row_set);
    HAL_GPIO_WritePin(btn_row_port[row], btn_row_pin[row], GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(btn_col_port[col], btn_col_pin[col])) {
        delay(1);
    }
    return;
}

void Buttons::update_velocity()
{
    for (uint8_t i = BTN_1_U; i <= BTN_29_D; i += 2) {
        int index     = (i - BTN_1_U) / 2;
        bool key_up   = (btn_matrix >> (i + 0)) & 1;
        bool key_down = (btn_matrix >> (i + 1)) & 1;
        if (key_down && !key_up && key_state[index] == ALL_RELEASED) { //(0x)
            delta_t_pressed[index] = get_ticks();
            key_state[index]       = PRESSING;
        }
        if (key_up && key_down && key_state[index] == PRESSING) { //(xx)
            delta_t_pressed[index] -= get_ticks();
            key_state[index] = ALL_PRESSED;
            // TODO: send a play signal
        }
        if (key_down && !key_up && key_state[index] == ALL_PRESSED) { //(0x)
            delta_t_released[index] = get_ticks();
            key_state[index]        = RELEASING;
        }
        if (!key_down && !key_up && key_state[index] == RELEASING) { //(00)
            delta_t_released[index] -= get_ticks();
            key_state[index] = ALL_RELEASED;
            // TODO: send a stop signal
        }
    }
}
