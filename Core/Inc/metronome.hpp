#pragma once

extern "C" {
#include "main.h"
}


class Metronome
{
public:
    Metronome(TIM_HandleTypeDef* timer, uint32_t bpm = 80, uint16_t div = 4) : timer{timer}
    {
        set_tempo(bpm);
        set_division(div);
    }

    void init()
    {
        timer->Instance->ARR  = 1000; // set the timer1 auto-reload m_counter
        timer->Instance->PSC  = 84;   // set the timer1 prescaler value
        timer->Instance->CCR1 = 500;  // set the compare value of timer1 channel1
    }

    void start()
    {
        HAL_TIM_PWM_Start(timer, TIM_CHANNEL_1);
        is_running = true;
        m_count    = m_division - 1;
    }

    void stop()
    {
        HAL_TIM_PWM_Stop(timer, TIM_CHANNEL_1);
        is_running = false;
    }

    void toggle() { (is_running ? stop() : start()); }

    void set_tempo(uint32_t bpm) { interval = 60000 / bpm; }
    void set_division(uint16_t div) { m_division = div; }

    uint16_t count() const { return m_count; }
    uint16_t division() const { return m_division; }

    void tick()
    {
        if (!is_running)
            return;

        if (is_beeping && HAL_GetTick() - last_ticks > beep_interval) {
            last_ticks = HAL_GetTick();
            is_beeping = false;
            beeper_off();
        } else if (HAL_GetTick() - last_ticks > interval - beep_interval) {
            last_ticks = HAL_GetTick();
            is_beeping = true;

            m_count = (m_count + 1 == m_division ? 0 : m_count + 1);
            if (m_count == 0) {
                beeper_strong();
            } else {
                beeper_weak();
            }

            gpio_toggle(LED2);
        }
    }

private:
    TIM_HandleTypeDef* timer;   // Timer object.
    uint32_t interval;          // Interval between beats, in ms.
    uint32_t beep_interval = 5; // The interval to beep on each beat, in ms.
    uint16_t m_division;        // Set the number of divisions (beats) per bar.

    // Internal parameters to track metronome.
    bool is_running     = false;
    uint16_t m_count    = 0;     // Which beat we're currently at.
    bool is_beeping     = false; // Whether beeper is currently on.
    uint32_t last_ticks = 0;     // Keeps track of the last update.

    static void beeper_strong()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload m_counter
        TIM3->PSC  = 120;  // set the timer1 prescaler value
        TIM3->CCR1 = 500;  // set the compare value of timer1 channel1
    }

    static void beeper_weak()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload m_counter
        TIM3->PSC  = 168;  // set the timer1 prescaler value
        TIM3->CCR1 = 500;  // set the compare value of timer1 channel1
    }

    static void beeper_off()
    {
        TIM3->ARR  = 1000; // set the timer1 auto-reload m_counter
        TIM3->PSC  = 84;   // set the timer1 prescaler value
        TIM3->CCR1 = 0;    // set the compare value of timer1 channel1
    }
};