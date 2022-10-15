#pragma once

extern "C" {
#include "main.h"
}

extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim8;

class speaker
{
    static constexpr TIM_HandleTypeDef* timer = &htim8;
    static constexpr DAC_HandleTypeDef* dac   = &hdac;

    static constexpr uint16_t amplitude = ((1 << 12) - 1) / 2;
    static constexpr size_t buffer_size = 1024;

    bool playing = false;

    using buffer_t = std::array<uint16_t, buffer_size>;
    buffer_t buffer[2];
    uint16_t* curr_buffer;
    uint16_t* next_buffer;

public:
    speaker()
    {
        curr_buffer = buffer[0].data();
        next_buffer = buffer[1].data();
    }

    static speaker& instance()
    {
        static speaker s;
        return s;
    }

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

    template <typename T>
    static void tick(T signal)
    {
        for (size_t i = 0; i < buffer_size; i++) {
            instance().curr_buffer[i] = signal.tick() * amplitude + amplitude;
        }
    }

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

private:
    void swap_buffer() { std::swap(curr_buffer, next_buffer); }
};
