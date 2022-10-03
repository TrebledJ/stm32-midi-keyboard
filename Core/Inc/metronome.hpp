extern "C" {
    #include "main.h"
}

extern TIM_HandleTypeDef htim3;

class metronome
{
public:
    static void init() {
        TIM3->ARR = 1000; // set the timer1 auto-reload counter
        TIM3->PSC = 84;   // set the timer1 prescaler value
        TIM3->CCR1 = 500; // set the compare value of timer1 channel1
        //  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);//too noist
    }

    static void tick(uint32_t last_ticks, int bpm, int idk_what_it_call) {
        float b2b_time = 1 / (bpm / 60.0f) * 1000;
        static uint16_t count = 0;
        static uint8_t buzz_offed = 0;
        if (HAL_GetTick() - last_ticks > b2b_time) {
            last_ticks = HAL_GetTick();
            buzz_offed = 0;
            if (!(count % idk_what_it_call)) {
                beeper_high();
            } else {
                beeper_low();
            }
            count++;
            gpio_toggle(LED2);
        }
        if (HAL_GetTick() - last_ticks > 5 && !buzz_offed) {
            beeper_off();
            buzz_offed = 1;
            last_ticks = HAL_GetTick();
        }
    }

private:
    static void beeper_high()
    {
        TIM3->ARR = 1000; // set the timer1 auto-reload counter
        TIM3->PSC = 120;  // set the timer1 prescaler value
        TIM3->CCR1 = 500; // set the compare value of timer1 channel1
    }

    static void beeper_low()
    {
        TIM3->ARR = 1000; // set the timer1 auto-reload counter
        TIM3->PSC = 168;  // set the timer1 prescaler value
        TIM3->CCR1 = 500; // set the compare value of timer1 channel1
    }
    
    static void beeper_off()
    {
        TIM3->ARR = 1000; // set the timer1 auto-reload counter
        TIM3->PSC = 84;   // set the timer1 prescaler value
        TIM3->CCR1 = 0;   // set the compare value of timer1 channel1
    }
};