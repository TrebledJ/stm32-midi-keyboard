#pragma once

extern "C" {
#include "main.h"
}

#include <array>

extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim8;

class speaker
{
    static constexpr TIM_HandleTypeDef* timer = &htim8;
    static constexpr DAC_HandleTypeDef* dac   = &hdac;

    static constexpr uint16_t amplitude = ((1 << 12) - 1) / 16;
    static constexpr size_t buffer_size = 1024;

public:
    using buffer_t = std::array<uint16_t, buffer_size>;

    static speaker& instance()
    {
        static speaker s;
        return s;
    }

    static void init();
    static void loop();

    // Scale a float value from [-1.0f, 1.0f] to [0, 2*amplitude].
    static uint16_t scale(float val) { return val * amplitude + amplitude; }

    static void play()
    {
        instance().playing = true;
        HAL_TIM_Base_Start(timer);
    }

    static void stop()
    {
        instance().playing = false;
        HAL_TIM_Base_Stop(timer);
    }

    template <size_t N>
    void set_active_keys(bool (&active)[N])
    {
    }

    template <typename... Ts>
    static void load(Ts&... ss)
    {
        for (size_t i = 0; i < buffer_size; i++) {
            instance().curr_buffer[i] = scale((ss.tick() + ...) / sizeof...(Ts));
        }
    }

    template <typename Ts, size_t N>
    static void load(Ts (&arr)[N], bool active[N])
    {
        int active_count = 0;
        for (int i = 0; i < N; i++)
            if (active[i])
                active_count++;

        for (size_t i = 0; i < buffer_size; i++) {
            float sum = 0;
            for (int i = 0; i < N; i++) {
                sum += active[i] * arr[i].tick();
            }

            instance().curr_buffer[i] = scale(sum / active_count);
        }
    }

    // Load using the currently selected oscillators.
    static void default_load(bool (&active)[NUM_KEYBOARD_KEYS]);

private:
    bool playing = false;

    buffer_t buffer[2];
    uint16_t* curr_buffer;
    uint16_t* next_buffer;

    speaker()
    {
        curr_buffer = buffer[0].data();
        next_buffer = buffer[1].data();
    }

    void swap_buffer() { std::swap(curr_buffer, next_buffer); }

    // Blocks until DAC can send, then sends the buffer over DMA.
    static void send()
    {
        while (HAL_DAC_GetState(dac) != HAL_DAC_STATE_READY)
            ; // Wait for DAC to be ready.

        // TODO: stereo
        auto buf = reinterpret_cast<uint32_t*>(instance().curr_buffer);
        HAL_DAC_Start_DMA(dac, DAC_CHANNEL_1, buf, buffer_size, DAC_ALIGN_12B_R);
        HAL_DAC_Start_DMA(dac, DAC_CHANNEL_2, buf, buffer_size, DAC_ALIGN_12B_R);

        instance().swap_buffer();
    }
};
