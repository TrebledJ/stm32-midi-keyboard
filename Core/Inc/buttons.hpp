#pragma once

extern "C" {
#include "main.h"
}

class Buttons
{
public:
    void tick(uint32_t last_ticks) { read_button_matrix(); }

private:
    uint64_t btn_matrix;

    void read_button_matrix()
    {
        // TODO
    }
};
