#pragma once

#include "utils/notes.hpp"


template <typename T>
class ActiveSoundArray
{
public:
    // Maximum number of active sounds at once.
    static constexpr int MAX_OSC = 16;

    void init()
    {
        for (int i = 0; i < MAX_OSC; i++) {
            banks[i].init();
        }
    }

    void note_on(Note note, uint8_t vel)
    {
        if (m_num_active == MAX_OSC)
            return;

        for (int i = 0; i < MAX_OSC; i++) {
            if (!used[i]) {
                banks[i].setFreq(notes::get_freq(note));
                banks[i].setPhase(0);
                used[i]  = true;
                notes[i] = note;
                vels[i]  = vel;
            }
        }
    }

    void note_off(Note note)
    {
        if (m_num_active == 0)
            return;

        for (int i = 0; i < MAX_OSC; i++) {
            if (used[i] && notes[i] == note) {
                used[i] = false;
                m_num_active--;
                break;
            }
        }
    }

    void reset()
    {
        m_num_active = 0;
        for (int i = 0; i < MAX_OSC; i++)
            used[i] = false;
    }

    float tick()
    {
        if (m_num_active == 0)
            return 0;

        float sum = 0;
        for (int i = 0; i < MAX_OSC; i++) {
            if (used[i])
                sum += banks[i].tick() * (vels[i] / 127);
        }
        return sum / m_num_active;
    }


private:
    T banks[MAX_OSC];
    Note notes[MAX_OSC];
    uint8_t vels[MAX_OSC];
    bool used[MAX_OSC];
    int m_num_active = 0;
};
