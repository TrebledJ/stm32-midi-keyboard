#include "main.h"
typedef struct {
    char col;
    char row;
} Name;
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
#define row_port(p) row##p##_GPIO_Port,
#define row_pin(p)  row##p##_Pin,

#define col_port(p) col##p##_GPIO_Port,
#define col_pin(p)  col##p##_Pin,

#define btn_table(X) \
    X(0)             \
    X(1)             \
    X(2)             \
    X(3)             \
    X(4)             \
    X(5)             \
    X(6)             \
    X(7)

uint64_t detect_key_matrix();
#define read_key_matrix(btn) (btn_matrix >> btn) & 1

void wait_key(int key);
