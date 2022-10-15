#ifndef LEAFPP_H
#define LEAFPP_H

#include "leaf.h"

namespace leaf
{
    class cycle
    {
        tCycle obj;

    public:
        void init(LEAF& leaf) { tCycle_init(&obj, &leaf); }
        void free() { tCycle_free(&obj); }

        float tick() { return tCycle_tick(&obj); }
        void setFreq(float freq) { return tCycle_setFreq(&obj, freq); }
        void setPhase(float phase) { return tCycle_setPhase(&obj, phase); }
        void setSampleRate(float sr) { return tCycle_setSampleRate(&obj, sr); }
    };

}; // namespace leaf


#endif
