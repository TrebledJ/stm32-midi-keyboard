#pragma once

extern "C" {
#include "main.h"
}

// extern TIM_HandleTypeDef htim3;

class Metronome
{
public:
    Metronome(TIM_TypeDef* timer, uint32_t bpm = 80, uint16_t div = 4) : timer{timer}
    {
        set_tempo(bpm);
        set_division(div);
    }

    void init()
    {
        timer->ARR  = 1000; // set the timer1 auto-reload counter
        timer->PSC  = 84;   // set the timer1 prescaler value
        timer->CCR1 = 500;  // set the compare value of timer1 channel1
        //  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // too noisy
    }

    void set_tempo(uint32_t bpm) { interval = 60000 / bpm; }
    void set_division(uint16_t div) { division = div; }

    void tick()
    {
        if (is_beeping && HAL_GetTick() - last_ticks > beep_interval) {
            last_ticks = HAL_GetTick();
            is_beeping = false;
            beeper_off();
        } else if (HAL_GetTick() - last_ticks > interval - beep_interval) {
            last_ticks = HAL_GetTick();
            is_beeping = true;
            if (count == division) {
                beeper_strong();
                count = 0;
            } else {
                beeper_weak();
                count++;
            }
            gpio_toggle(LED2);
        }
    }

private:
    TIM_TypeDef* timer;         // Timer object.
    uint32_t interval;          // Interval between beats, in ms.
    uint32_t beep_interval = 5; // The interval to beep on each beat, in ms.
    uint16_t division;          // Set the number of divisions (beats) per bar.

    // Internal parameters to track metronome.
    uint16_t count      = 0;     // Which beat we're currently at.
    bool is_beeping     = false; // Whether beeper is currently on.
    uint32_t last_ticks = 0;     // Keeps track of the last update.

    static void beeper_strong()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload counter
        TIM3->PSC  = 120;  // set the timer1 prescaler value
        TIM3->CCR1 = 500;  // set the compare value of timer1 channel1
    }

    static void beeper_weak()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload counter
        TIM3->PSC  = 168;  // set the timer1 prescaler value
        TIM3->CCR1 = 500;  // set the compare value of timer1 channel1
    }

    static void beeper_off()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload counter
        TIM3->PSC  = 84;   // set the timer1 prescaler value
        TIM3->CCR1 = 0;    // set the compare value of timer1 channel1
    }
};