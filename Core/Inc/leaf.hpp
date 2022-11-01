#ifndef LEAF_HPP
#define LEAF_HPP

#include "leaf.h"

namespace leaf
{
    extern LEAF main;

    inline void init(LEAF& leaf, float sampleRate, char* memory, size_t memorySize, float (*random)(void))
    {
        LEAF_init(&leaf, sampleRate, memory, memorySize, random);
    }

    inline void init(float sampleRate, char* memory, size_t memorySize, float (*random)(void))
    {
        init(main, sampleRate, memory, memorySize, random);
    }

    inline void setSampleRate(LEAF& leaf, float sampleRate) { LEAF_setSampleRate(&leaf, sampleRate); }
    inline void setSampleRate(float sampleRate) { setSampleRate(main, sampleRate); }
    inline float getSampleRate(const LEAF& leaf = main) { return leaf.sampleRate; }

    namespace distortion
    {
        struct sample_reducer {
            tSampleReducer obj;

            void init(LEAF& leaf = main) { tSampleReducer_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSampleReducer_initToPool(&obj, mempool); }
            void free() { tSampleReducer_free(&obj); }

            float tick(float input) { return tSampleReducer_tick(&obj, input); }
            void setRatio(float ratio) { tSampleReducer_setRatio(&obj, ratio); }
        };

        struct oversampler {
            tOversampler obj;

            void init(int order, int extraQuality, LEAF& leaf = main)
            {
                tOversampler_init(&obj, order, extraQuality, &leaf);
            }
            void initToPool(int order, int extraQuality, tMempool* mempool)
            {
                tOversampler_initToPool(&obj, order, extraQuality, mempool);
            }
            void free() { tOversampler_free(&obj); }

            void upsample(float input, float* output) { tOversampler_upsample(&obj, input, output); }
            float downsample(float* input) { return tOversampler_downsample(&obj, input); }
            float tick(float input, float* oversample, float (*effectTick)(float))
            {
                return tOversampler_tick(&obj, input, oversample, effectTick);
            }

            void setRatio(int ratio) { tOversampler_setRatio(&obj, ratio); }
            void setQuality(int quality) { tOversampler_setQuality(&obj, quality); }
            int getLatency() { return tOversampler_getLatency(&obj); }
        };

        struct lockhart_wavefolder {
            tLockhartWavefolder obj;

            void init(LEAF& leaf = main) { tLockhartWavefolder_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tLockhartWavefolder_initToPool(&obj, mempool); }
            void free() { tLockhartWavefolder_free(&obj); }

            float tick(float samp) { return tLockhartWavefolder_tick(&obj, samp); }
        };

        struct crusher {
            tCrusher obj;

            void init(LEAF& leaf = main) { tCrusher_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tCrusher_initToPool(&obj, mempool); }
            void free() { tCrusher_free(&obj); }

            float tick(float input) { return tCrusher_tick(&obj, input); }
            void setOperation(float op) { tCrusher_setOperation(&obj, op); }
            void setQuality(float val) { tCrusher_setQuality(&obj, val); }
            void setRound(float rnd) { tCrusher_setRound(&obj, rnd); }
            void setSamplingRatio(float ratio) { tCrusher_setSamplingRatio(&obj, ratio); }
        };

    } // namespace distortion

    namespace midi
    {
        struct stack {
            tStack obj;

            void init(LEAF& leaf = main) { tStack_init(&obj, &leaf); }
            void initToPool(tMempool* pool) { tStack_initToPool(&obj, pool); }
            void free() { tStack_free(&obj); }

            void setCapacity(uint16_t cap) { tStack_setCapacity(&obj, cap); }
            int addIfNotAlreadyThere(uint16_t item) { return tStack_addIfNotAlreadyThere(&obj, item); }
            void add(uint16_t item) { tStack_add(&obj, item); }
            int remove(uint16_t item) { return tStack_remove(&obj, item); }
            void clear() { tStack_clear(&obj); }
            int first() { return tStack_first(&obj); }
            int getSize() { return tStack_getSize(&obj); }
            int contains(uint16_t item) { return tStack_contains(&obj, item); }
            int next() { return tStack_next(&obj); }
            int get(int index) { return tStack_get(&obj, index); }
        };

        struct poly {
            tPoly obj;

            void init(int maxNumVoices, LEAF& leaf = main) { tPoly_init(&obj, maxNumVoices, &leaf); }
            void initToPool(int maxNumVoices, tMempool* pool) { tPoly_initToPool(&obj, maxNumVoices, pool); }
            void free() { tPoly_free(&obj); }

            int noteOn(int note, uint8_t vel) { return tPoly_noteOn(&obj, note, vel); }
            int noteOff(uint8_t note) { return tPoly_noteOff(&obj, note); }
            void orderedAddToStack(uint8_t note) { tPoly_orderedAddToStack(&obj, note); }
            void setNumVoices(uint8_t numVoices) { tPoly_setNumVoices(&obj, numVoices); }
            void setPitchGlideActive(int isActive) { tPoly_setPitchGlideActive(&obj, isActive); }
            void setPitchGlideTime(float t) { tPoly_setPitchGlideTime(&obj, t); }
            void setPitchBend(float pitchBend) { tPoly_setPitchBend(&obj, pitchBend); }
            void setBendGlideTime(float t) { tPoly_setBendGlideTime(&obj, t); }
            void setBendSamplesPerTick(float t) { tPoly_setBendSamplesPerTick(&obj, t); }
            void tickPitch() { tPoly_tickPitch(&obj); }
            void tickPitchGlide() { tPoly_tickPitchGlide(&obj); }
            void tickPitchBend() { tPoly_tickPitchBend(&obj); }
            int getNumVoices() { return tPoly_getNumVoices(&obj); }
            int getNumActiveVoices() { return tPoly_getNumActiveVoices(&obj); }
            float getPitch(uint8_t voice) { return tPoly_getPitch(&obj, voice); }
            int getKey(uint8_t voice) { return tPoly_getKey(&obj, voice); }
            int getVelocity(uint8_t voice) { return tPoly_getVelocity(&obj, voice); }
            int isOn(uint8_t voice) { return tPoly_isOn(&obj, voice); }
            void setSampleRate(float sr) { tPoly_setSampleRate(&obj, sr); }
        };

        struct simple_poly {
            tSimplePoly obj;

            void init(int maxNumVoices, LEAF& leaf = main) { tSimplePoly_init(&obj, maxNumVoices, &leaf); }
            void initToPool(int maxNumVoices, tMempool* pool) { tSimplePoly_initToPool(&obj, maxNumVoices, pool); }
            void free() { tSimplePoly_free(&obj); }

            int noteOn(int note, uint8_t vel) { return tSimplePoly_noteOn(&obj, note, vel); }
            int noteOff(uint8_t note) { return tSimplePoly_noteOff(&obj, note); }
            void deactivateVoice(uint8_t voice) { tSimplePoly_deactivateVoice(&obj, voice); }
            int markPendingNoteOff(uint8_t note) { return tSimplePoly_markPendingNoteOff(&obj, note); }
            int findVoiceAssignedToNote(uint8_t note) { return tSimplePoly_findVoiceAssignedToNote(&obj, note); }
            void setNumVoices(uint8_t numVoices) { tSimplePoly_setNumVoices(&obj, numVoices); }
            int getNumVoices() { return tSimplePoly_getNumVoices(&obj); }
            int getNumActiveVoices() { return tSimplePoly_getNumActiveVoices(&obj); }
            int getPitch(uint8_t voice) { return tSimplePoly_getPitch(&obj, voice); }
            int getPitchAndCheckActive(uint8_t voice) { return tSimplePoly_getPitchAndCheckActive(&obj, voice); }
            int getVelocity(uint8_t voice) { return tSimplePoly_getVelocity(&obj, voice); }
            int isOn(uint8_t voice) { return tSimplePoly_isOn(&obj, voice); }
        };

    } // namespace midi

    namespace env
    {
        struct envelope {
            tEnvelope obj;

            void init(float attack, float decay, int loop, LEAF& leaf = main)
            {
                tEnvelope_init(&obj, attack, decay, loop, &leaf);
            }
            void initToPool(float attack, float decay, int loop, tMempool* mempool)
            {
                tEnvelope_initToPool(&obj, attack, decay, loop, mempool);
            }
            void free() { tEnvelope_free(&obj); }

            float tick() { return tEnvelope_tick(&obj); }
            void setAttack(float attack) { tEnvelope_setAttack(&obj, attack); }
            void setDecay(float decay) { tEnvelope_setDecay(&obj, decay); }
            void loop(int loop) { tEnvelope_loop(&obj, loop); }
            void on(float velocity) { tEnvelope_on(&obj, velocity); }
        };

        struct exp_smooth {
            tExpSmooth obj;

            void init(float val, float factor, LEAF& leaf = main) { tExpSmooth_init(&obj, val, factor, &leaf); }
            void initToPool(float val, float factor, tMempool* mempool)
            {
                tExpSmooth_initToPool(&obj, val, factor, mempool);
            }
            void free() { tExpSmooth_free(&obj); }

            float tick() { return tExpSmooth_tick(&obj); }
            float sample() { return tExpSmooth_sample(&obj); }
            void setFactor(float factor) { tExpSmooth_setFactor(&obj, factor); }
            void setDest(float dest) { tExpSmooth_setDest(&obj, dest); }
            void setVal(float val) { tExpSmooth_setVal(&obj, val); }
            void setValAndDest(float val) { tExpSmooth_setValAndDest(&obj, val); }
            void setSampleRate(float sr) { tExpSmooth_setSampleRate(&obj, sr); }
        };

        struct adsr {
            tADSR obj;

            void init(float attack, float decay, float sustain, float release, LEAF& leaf = main)
            {
                tADSR_init(&obj, attack, decay, sustain, release, &leaf);
            }
            void free() { tADSR_free(&obj); }

            float tick() { return tADSR_tick(&obj); }
            void setAttack(float attack) { tADSR_setAttack(&obj, attack); }
            void setDecay(float decay) { tADSR_setDecay(&obj, decay); }
            void setSustain(float sustain) { tADSR_setSustain(&obj, sustain); }
            void setRelease(float release) { tADSR_setRelease(&obj, release); }
            void setLeakFactor(float leakFactor) { tADSR_setLeakFactor(&obj, leakFactor); }
            void on(float velocity) { tADSR_on(&obj, velocity); }
            void off() { tADSR_off(&obj); }
            void setSampleRate(float sr) { tADSR_setSampleRate(&obj, sr); }
        };

        struct adsrt {
            tADSRT obj;

            void free() { tADSRT_free(&obj); }

            float tick() { return tADSRT_tick(&obj); }
            float tickNoInterp() { return tADSRT_tickNoInterp(&obj); }
            void setAttack(float attack) { tADSRT_setAttack(&obj, attack); }
            void setDecay(float decay) { tADSRT_setDecay(&obj, decay); }
            void setSustain(float sustain) { tADSRT_setSustain(&obj, sustain); }
            void setRelease(float release) { tADSRT_setRelease(&obj, release); }
            void setLeakFactor(float leakFactor) { tADSRT_setLeakFactor(&obj, leakFactor); }
            void on(float velocity) { tADSRT_on(&obj, velocity); }
            void off() { tADSRT_off(&obj); }
            void setSampleRate(float sr) { tADSRT_setSampleRate(&obj, sr); }
        };

        struct adsrs {
            tADSRS obj;

            void init(float attack, float decay, float sustain, float release, LEAF& leaf = main)
            {
                tADSRS_init(&obj, attack, decay, sustain, release, &leaf);
            }
            void initToPool(float attack, float decay, float sustain, float release, tMempool* mempool)
            {
                tADSRS_initToPool(&obj, attack, decay, sustain, release, mempool);
            }
            void free() { tADSRS_free(&obj); }

            float tick() { return tADSRS_tick(&obj); }
            void setAttack(float attack) { tADSRS_setAttack(&obj, attack); }
            void setDecay(float decay) { tADSRS_setDecay(&obj, decay); }
            void setSustain(float sustain) { tADSRS_setSustain(&obj, sustain); }
            void setRelease(float release) { tADSRS_setRelease(&obj, release); }
            void setLeakFactor(float leakFactor) { tADSRS_setLeakFactor(&obj, leakFactor); }
            void on(float velocity) { tADSRS_on(&obj, velocity); }
            void off() { tADSRS_off(&obj); }
            void setSampleRate(float sr) { tADSRS_setSampleRate(&obj, sr); }
        };

        struct ramp {
            tRamp obj;

            void init(float time, int samplesPerTick, LEAF& leaf = main)
            {
                tRamp_init(&obj, time, samplesPerTick, &leaf);
            }
            void initToPool(float time, int samplesPerTick, tMempool* mempool)
            {
                tRamp_initToPool(&obj, time, samplesPerTick, mempool);
            }
            void free() { tRamp_free(&obj); }

            float tick() { return tRamp_tick(&obj); }
            float sample() { return tRamp_sample(&obj); }
            void setTime(float time) { tRamp_setTime(&obj, time); }
            void setDest(float dest) { tRamp_setDest(&obj, dest); }
            void setVal(float val) { tRamp_setVal(&obj, val); }
            void setSampleRate(float sr) { tRamp_setSampleRate(&obj, sr); }
        };

        struct ramp_up_down {
            tRampUpDown obj;

            void init(float upTime, float downTime, int samplesPerTick, LEAF& leaf = main)
            {
                tRampUpDown_init(&obj, upTime, downTime, samplesPerTick, &leaf);
            }
            void initToPool(float upTime, float downTime, int samplesPerTick, tMempool* mempool)
            {
                tRampUpDown_initToPool(&obj, upTime, downTime, samplesPerTick, mempool);
            }
            void free() { tRampUpDown_free(&obj); }

            float tick() { return tRampUpDown_tick(&obj); }
            float sample() { return tRampUpDown_sample(&obj); }
            void setUpTime(float upTime) { tRampUpDown_setUpTime(&obj, upTime); }
            void setDownTime(float downTime) { tRampUpDown_setDownTime(&obj, downTime); }
            void setDest(float dest) { tRampUpDown_setDest(&obj, dest); }
            void setVal(float val) { tRampUpDown_setVal(&obj, val); }
            void setSampleRate(float sr) { tRampUpDown_setSampleRate(&obj, sr); }
        };

        struct slide {
            tSlide obj;

            void init(float upSlide, float downSlide, LEAF& leaf = main)
            {
                tSlide_init(&obj, upSlide, downSlide, &leaf);
            }
            void initToPool(float upSlide, float downSlide, tMempool* mempool)
            {
                tSlide_initToPool(&obj, upSlide, downSlide, mempool);
            }
            void free() { tSlide_free(&obj); }

            float tick(float in) { return tSlide_tick(&obj, in); }
            float tickNoInput() { return tSlide_tickNoInput(&obj); }
            void setUpSlide(float upSlide) { tSlide_setUpSlide(&obj, upSlide); }
            void setDownSlide(float downSlide) { tSlide_setDownSlide(&obj, downSlide); }
            void setDest(float dest) { tSlide_setDest(&obj, dest); }
        };

    } // namespace env

    namespace inst
    {
        struct r808cowbell {
            t808Cowbell obj;

            void init(int useStick, LEAF& leaf = main) { t808Cowbell_init(&obj, useStick, &leaf); }
            void initToPool(int useStick, tMempool* mempool) { t808Cowbell_initToPool(&obj, useStick, mempool); }
            void free() { t808Cowbell_free(&obj); }

            float tick() { return t808Cowbell_tick(&obj); }
            void on(float vel) { t808Cowbell_on(&obj, vel); }
            void setDecay(float decay) { t808Cowbell_setDecay(&obj, decay); }
            void setHighpassFreq(float freq) { t808Cowbell_setHighpassFreq(&obj, freq); }
            void setBandpassFreq(float freq) { t808Cowbell_setBandpassFreq(&obj, freq); }
            void setFreq(float freq) { t808Cowbell_setFreq(&obj, freq); }
            void setOscMix(float oscMix) { t808Cowbell_setOscMix(&obj, oscMix); }
            void setStick(int useStick) { t808Cowbell_setStick(&obj, useStick); }
            void setSampleRate(float sr) { t808Cowbell_setSampleRate(&obj, sr); }
        };

        struct r808hihat {
            t808Hihat obj;

            void init(LEAF& leaf = main) { t808Hihat_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { t808Hihat_initToPool(&obj, mempool); }
            void free() { t808Hihat_free(&obj); }

            float tick() { return t808Hihat_tick(&obj); }
            void on(float vel) { t808Hihat_on(&obj, vel); }
            void setOscNoiseMix(float oscNoiseMix) { t808Hihat_setOscNoiseMix(&obj, oscNoiseMix); }
            void setDecay(float decay) { t808Hihat_setDecay(&obj, decay); }
            void setHighpassFreq(float freq) { t808Hihat_setHighpassFreq(&obj, freq); }
            void setOscBandpassFreq(float freq) { t808Hihat_setOscBandpassFreq(&obj, freq); }
            void setOscBandpassQ(float Q) { t808Hihat_setOscBandpassQ(&obj, Q); }
            void setStickBandPassFreq(float freq) { t808Hihat_setStickBandPassFreq(&obj, freq); }
            void setStickBandPassQ(float Q) { t808Hihat_setStickBandPassQ(&obj, Q); }
            void setOscFreq(float freq) { t808Hihat_setOscFreq(&obj, freq); }
            void setStretch(float stretch) { t808Hihat_setStretch(&obj, stretch); }
            void setFM(float FM_amount) { t808Hihat_setFM(&obj, FM_amount); }
            void setSampleRate(float sr) { t808Hihat_setSampleRate(&obj, sr); }
        };

        struct r808snare {
            t808Snare obj;

            void init(LEAF& leaf = main) { t808Snare_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { t808Snare_initToPool(&obj, mempool); }
            void free() { t808Snare_free(&obj); }

            float tick() { return t808Snare_tick(&obj); }
            void on(float vel) { t808Snare_on(&obj, vel); }
            void setTone1Freq(float freq) { t808Snare_setTone1Freq(&obj, freq); }
            void setTone2Freq(float freq) { t808Snare_setTone2Freq(&obj, freq); }
            void setTone1Decay(float decay) { t808Snare_setTone1Decay(&obj, decay); }
            void setTone2Decay(float decay) { t808Snare_setTone2Decay(&obj, decay); }
            void setNoiseDecay(float decay) { t808Snare_setNoiseDecay(&obj, decay); }
            void setToneNoiseMix(float toneNoiseMix) { t808Snare_setToneNoiseMix(&obj, toneNoiseMix); }
            void setNoiseFilterFreq(float noiseFilterFreq) { t808Snare_setNoiseFilterFreq(&obj, noiseFilterFreq); }
            void setNoiseFilterQ(float noiseFilterQ) { t808Snare_setNoiseFilterQ(&obj, noiseFilterQ); }
            void setSampleRate(float sr) { t808Snare_setSampleRate(&obj, sr); }
        };

        struct r808kick {
            t808Kick obj;

            void init(LEAF& leaf = main) { t808Kick_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { t808Kick_initToPool(&obj, mempool); }
            void free() { t808Kick_free(&obj); }

            float tick() { return t808Kick_tick(&obj); }
            void on(float vel) { t808Kick_on(&obj, vel); }
            void setToneFreq(float freq) { t808Kick_setToneFreq(&obj, freq); }
            void setToneDecay(float decay) { t808Kick_setToneDecay(&obj, decay); }
            void setNoiseDecay(float decay) { t808Kick_setNoiseDecay(&obj, decay); }
            void setSighAmount(float sigh) { t808Kick_setSighAmount(&obj, sigh); }
            void setChirpAmount(float chirp) { t808Kick_setChirpAmount(&obj, chirp); }
            void setToneNoiseMix(float toneNoiseMix) { t808Kick_setToneNoiseMix(&obj, toneNoiseMix); }
            void setNoiseFilterFreq(float noiseFilterFreq) { t808Kick_setNoiseFilterFreq(&obj, noiseFilterFreq); }
            void setNoiseFilterQ(float noiseFilterQ) { t808Kick_setNoiseFilterQ(&obj, noiseFilterQ); }
            void setSampleRate(float sr) { t808Kick_setSampleRate(&obj, sr); }
        };

    } // namespace inst

    namespace electrical
    {
        struct wdf {
            tWDF obj;

            void init(WDFComponentType type, float value, tWDF* rL, tWDF* rR, LEAF& leaf = main)
            {
                tWDF_init(&obj, type, value, rL, rR, &leaf);
            }
            void initToPool(WDFComponentType type, float value, tWDF* rL, tWDF* rR, tMempool* mempool)
            {
                tWDF_initToPool(&obj, type, value, rL, rR, mempool);
            }
            void free() { tWDF_free(&obj); }

            float tick(float sample, tWDF* outputPoint, uint8_t paramsChanged)
            {
                return tWDF_tick(&obj, sample, outputPoint, paramsChanged);
            }

            void setValue(float value) { tWDF_setValue(&obj, value); }
            void setSampleRate(float sample_rate) { tWDF_setSampleRate(&obj, sample_rate); }
            uint8_t isLeaf() { return tWDF_isLeaf(&obj); }

            float getPortResistance() { return tWDF_getPortResistance(&obj); }
            float getReflectedWaveUp(float input) { return tWDF_getReflectedWaveUp(&obj, input); }
            float getReflectedWaveDown(float input, float incident_wave)
            {
                return tWDF_getReflectedWaveDown(&obj, input, incident_wave);
            }
            void setIncidentWave(float incident_wave, float input) { tWDF_setIncidentWave(&obj, incident_wave, input); }

            float getVoltage() { return tWDF_getVoltage(&obj); }
            float getCurrent() { return tWDF_getCurrent(&obj); }
        };

    } // namespace electrical

    namespace delay
    {
        struct delay {
            tDelay obj;

            void init(uint32_t delay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tDelay_init(&obj, delay, maxDelay, &leaf);
            }
            void initToPool(uint32_t delay, uint32_t maxDelay, tMempool* mempool)
            {
                tDelay_initToPool(&obj, delay, maxDelay, mempool);
            }
            void free() { tDelay_free(&obj); }

            void clear() { tDelay_clear(&obj); }
            void setDelay(uint32_t delay) { tDelay_setDelay(&obj, delay); }
            uint32_t getDelay() { return tDelay_getDelay(&obj); }
            void tapIn(float in, uint32_t tapDelay) { tDelay_tapIn(&obj, in, tapDelay); }
            float tapOut(uint32_t tapDelay) { return tDelay_tapOut(&obj, tapDelay); }
            float addTo(float value, uint32_t tapDelay) { return tDelay_addTo(&obj, value, tapDelay); }
            float tick(float sample) { return tDelay_tick(&obj, sample); }
            float getLastOut() { return tDelay_getLastOut(&obj); }
            float getLastIn() { return tDelay_getLastIn(&obj); }
        };

        struct linear_delay {
            tLinearDelay obj;

            void init(float delay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tLinearDelay_init(&obj, delay, maxDelay, &leaf);
            }
            void initToPool(float delay, uint32_t maxDelay, tMempool* mempool)
            {
                tLinearDelay_initToPool(&obj, delay, maxDelay, mempool);
            }
            void free() { tLinearDelay_free(&obj); }

            void clear() { tLinearDelay_clear(&obj); }
            void setDelay(float delay) { tLinearDelay_setDelay(&obj, delay); }
            float getDelay() { return tLinearDelay_getDelay(&obj); }
            void tapIn(float in, uint32_t tapDelay) { tLinearDelay_tapIn(&obj, in, tapDelay); }
            float tapOut(uint32_t tapDelay) { return tLinearDelay_tapOut(&obj, tapDelay); }
            float addTo(float value, uint32_t tapDelay) { return tLinearDelay_addTo(&obj, value, tapDelay); }
            float tick(float sample) { return tLinearDelay_tick(&obj, sample); }
            void tickIn(float input) { tLinearDelay_tickIn(&obj, input); }
            float tickOut() { return tLinearDelay_tickOut(&obj); }
            float getLastOut() { return tLinearDelay_getLastOut(&obj); }
            float getLastIn() { return tLinearDelay_getLastIn(&obj); }
        };

        struct hermite_delay {
            tHermiteDelay obj;

            void init(float delay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tHermiteDelay_init(&obj, delay, maxDelay, &leaf);
            }
            void initToPool(float delay, uint32_t maxDelay, tMempool* mp)
            {
                tHermiteDelay_initToPool(&obj, delay, maxDelay, mp);
            }
            void free() { tHermiteDelay_free(&obj); }

            void clear() { tHermiteDelay_clear(&obj); }
            float tick(float input) { return tHermiteDelay_tick(&obj, input); }
            void tickIn(float input) { tHermiteDelay_tickIn(&obj, input); }
            float tickOut() { return tHermiteDelay_tickOut(&obj); }
            void setDelay(float delay) { tHermiteDelay_setDelay(&obj, delay); }
            float tapOut(uint32_t tapDelay) { return tHermiteDelay_tapOut(&obj, tapDelay); }
            float tapOutInterpolated(uint32_t tapDelay, float alpha)
            {
                return tHermiteDelay_tapOutInterpolated(&obj, tapDelay, alpha);
            }
            void tapIn(float value, uint32_t tapDelay) { tHermiteDelay_tapIn(&obj, value, tapDelay); }
            float addTo(float value, uint32_t tapDelay) { return tHermiteDelay_addTo(&obj, value, tapDelay); }
            float getDelay() { return tHermiteDelay_getDelay(&obj); }
            float getLastOut() { return tHermiteDelay_getLastOut(&obj); }
            float getLastIn() { return tHermiteDelay_getLastIn(&obj); }
            void setGain(float gain) { tHermiteDelay_setGain(&obj, gain); }
            float getGain() { return tHermiteDelay_getGain(&obj); }
        };

        struct allpass_delay {
            tAllpassDelay obj;

            void init(float delay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tAllpassDelay_init(&obj, delay, maxDelay, &leaf);
            }
            void initToPool(float delay, uint32_t maxDelay, tMempool* mempool)
            {
                tAllpassDelay_initToPool(&obj, delay, maxDelay, mempool);
            }
            void free() { tAllpassDelay_free(&obj); }

            void clear() { tAllpassDelay_clear(&obj); }
            void setDelay(float delay) { tAllpassDelay_setDelay(&obj, delay); }
            float getDelay() { return tAllpassDelay_getDelay(&obj); }
            void tapIn(float in, uint32_t tapDelay) { tAllpassDelay_tapIn(&obj, in, tapDelay); }
            float tapOut(uint32_t tapDelay) { return tAllpassDelay_tapOut(&obj, tapDelay); }
            float addTo(float value, uint32_t tapDelay) { return tAllpassDelay_addTo(&obj, value, tapDelay); }
            float tick(float sample) { return tAllpassDelay_tick(&obj, sample); }
            float getLastOut() { return tAllpassDelay_getLastOut(&obj); }
            float getLastIn() { return tAllpassDelay_getLastIn(&obj); }
        };

        struct tape_delay {
            tTapeDelay obj;

            void init(float delay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tTapeDelay_init(&obj, delay, maxDelay, &leaf);
            }
            void initToPool(float delay, uint32_t maxDelay, tMempool* mempool)
            {
                tTapeDelay_initToPool(&obj, delay, maxDelay, mempool);
            }
            void free() { tTapeDelay_free(&obj); }

            void clear() { tTapeDelay_clear(&obj); }
            void setDelay(float delay) { tTapeDelay_setDelay(&obj, delay); }
            float getDelay() { return tTapeDelay_getDelay(&obj); }
            void tapIn(float in, uint32_t tapDelay) { tTapeDelay_tapIn(&obj, in, tapDelay); }
            float tapOut(float tapDelay) { return tTapeDelay_tapOut(&obj, tapDelay); }
            float addTo(float value, uint32_t tapDelay) { return tTapeDelay_addTo(&obj, value, tapDelay); }
            float tick(float sample) { return tTapeDelay_tick(&obj, sample); }
            void incrementInPoint() { tTapeDelay_incrementInPoint(&obj); }
            float getLastOut() { return tTapeDelay_getLastOut(&obj); }
            float getLastIn() { return tTapeDelay_getLastIn(&obj); }
        };

        struct ring_buffer {
            tRingBuffer obj;

            void init(int size, LEAF& leaf = main) { tRingBuffer_init(&obj, size, &leaf); }
            void initToPool(int size, tMempool* mempool) { tRingBuffer_initToPool(&obj, size, mempool); }
            void free() { tRingBuffer_free(&obj); }

            void push(float val) { tRingBuffer_push(&obj, val); }
            float getNewest() { return tRingBuffer_getNewest(&obj); }
            float getOldest() { return tRingBuffer_getOldest(&obj); }
            float get(int index) { return tRingBuffer_get(&obj, index); }
            int getSize() { return tRingBuffer_getSize(&obj); }
        };

    } // namespace delay

    namespace filters
    {
        struct allpass {
            tAllpass obj;

            void init(float initDelay, uint32_t maxDelay, LEAF& leaf = main)
            {
                tAllpass_init(&obj, initDelay, maxDelay, &leaf);
            }
            void initToPool(float initDelay, uint32_t maxDelay, tMempool* mempool)
            {
                tAllpass_initToPool(&obj, initDelay, maxDelay, mempool);
            }
            void free() { tAllpass_free(&obj); }

            float tick(float input) { return tAllpass_tick(&obj, input); }
            void setGain(float gain) { tAllpass_setGain(&obj, gain); }
            void setDelay(float delay) { tAllpass_setDelay(&obj, delay); }
        };

        struct one_pole {
            tOnePole obj;

            void init(float thePole, LEAF& leaf = main) { tOnePole_init(&obj, thePole, &leaf); }
            void initToPool(float thePole, tMempool* mempool) { tOnePole_initToPool(&obj, thePole, mempool); }
            void free() { tOnePole_free(&obj); }

            float tick(float input) { return tOnePole_tick(&obj, input); }
            void setB0(float b0) { tOnePole_setB0(&obj, b0); }
            void setA1(float a1) { tOnePole_setA1(&obj, a1); }
            void setPole(float thePole) { tOnePole_setPole(&obj, thePole); }
            void setFreq(float freq) { tOnePole_setFreq(&obj, freq); }
            void setCoefficients(float b0, float a1) { tOnePole_setCoefficients(&obj, b0, a1); }
            void setGain(float gain) { tOnePole_setGain(&obj, gain); }
            void setSampleRate(float sr) { tOnePole_setSampleRate(&obj, sr); }
        };

        struct two_pole {
            tTwoPole obj;

            void init(LEAF& leaf = main) { tTwoPole_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tTwoPole_initToPool(&obj, mempool); }
            void free() { tTwoPole_free(&obj); }

            float tick(float input) { return tTwoPole_tick(&obj, input); }
            void setB0(float b0) { tTwoPole_setB0(&obj, b0); }
            void setA1(float a1) { tTwoPole_setA1(&obj, a1); }
            void setA2(float a2) { tTwoPole_setA2(&obj, a2); }
            void setResonance(float freq, float radius, int normalize)
            {
                tTwoPole_setResonance(&obj, freq, radius, normalize);
            }
            void setCoefficients(float b0, float a1, float a2) { tTwoPole_setCoefficients(&obj, b0, a1, a2); }
            void setGain(float gain) { tTwoPole_setGain(&obj, gain); }
            void setSampleRate(float sr) { tTwoPole_setSampleRate(&obj, sr); }
        };

        struct one_zero {
            tOneZero obj;

            void init(float theZero, LEAF& leaf = main) { tOneZero_init(&obj, theZero, &leaf); }
            void initToPool(float theZero, tMempool* mempool) { tOneZero_initToPool(&obj, theZero, mempool); }
            void free() { tOneZero_free(&obj); }

            float tick(float input) { return tOneZero_tick(&obj, input); }
            void setB0(float b0) { tOneZero_setB0(&obj, b0); }
            void setB1(float b1) { tOneZero_setB1(&obj, b1); }
            void setZero(float theZero) { tOneZero_setZero(&obj, theZero); }
            void setCoefficients(float b0, float b1) { tOneZero_setCoefficients(&obj, b0, b1); }
            void setGain(float gain) { tOneZero_setGain(&obj, gain); }
            float getPhaseDelay(float frequency) { return tOneZero_getPhaseDelay(&obj, frequency); }
            void setSampleRate(float sr) { tOneZero_setSampleRate(&obj, sr); }
        };

        struct two_zero {
            tTwoZero obj;

            void init(LEAF& leaf = main) { tTwoZero_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tTwoZero_initToPool(&obj, mempool); }
            void free() { tTwoZero_free(&obj); }

            float tick(float input) { return tTwoZero_tick(&obj, input); }
            void setB0(float b0) { tTwoZero_setB0(&obj, b0); }
            void setB1(float b1) { tTwoZero_setB1(&obj, b1); }
            void setB2(float b2) { tTwoZero_setB2(&obj, b2); }
            void setNotch(float frequency, float radius) { tTwoZero_setNotch(&obj, frequency, radius); }
            void setCoefficients(float b0, float b1, float b2) { tTwoZero_setCoefficients(&obj, b0, b1, b2); }
            void setGain(float gain) { tTwoZero_setGain(&obj, gain); }
            void setSampleRate(float sr) { tTwoZero_setSampleRate(&obj, sr); }
        };

        struct pole_zero {
            tPoleZero obj;

            void init(LEAF& leaf = main) { tPoleZero_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tPoleZero_initToPool(&obj, mempool); }
            void free() { tPoleZero_free(&obj); }

            float tick(float input) { return tPoleZero_tick(&obj, input); }
            void setB0(float b0) { tPoleZero_setB0(&obj, b0); }
            void setB1(float b1) { tPoleZero_setB1(&obj, b1); }
            void setA1(float a1) { tPoleZero_setA1(&obj, a1); }
            void setCoefficients(float b0, float b1, float a1) { tPoleZero_setCoefficients(&obj, b0, b1, a1); }
            void setAllpass(float coeff) { tPoleZero_setAllpass(&obj, coeff); }
            void setBlockZero(float thePole) { tPoleZero_setBlockZero(&obj, thePole); }
            void setGain(float gain) { tPoleZero_setGain(&obj, gain); }
        };

        struct bi_quad {
            tBiQuad obj;

            void init(LEAF& leaf = main) { tBiQuad_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tBiQuad_initToPool(&obj, mempool); }
            void free() { tBiQuad_free(&obj); }

            float tick(float input) { return tBiQuad_tick(&obj, input); }
            void setB0(float b0) { tBiQuad_setB0(&obj, b0); }
            void setB1(float b1) { tBiQuad_setB1(&obj, b1); }
            void setB2(float b2) { tBiQuad_setB2(&obj, b2); }
            void setA1(float a1) { tBiQuad_setA1(&obj, a1); }
            void setA2(float a2) { tBiQuad_setA2(&obj, a2); }
            void setNotch(float freq, float radius) { tBiQuad_setNotch(&obj, freq, radius); }
            void setResonance(float freq, float radius, int normalize)
            {
                tBiQuad_setResonance(&obj, freq, radius, normalize);
            }
            void setCoefficients(float b0, float b1, float b2, float a1, float a2)
            {
                tBiQuad_setCoefficients(&obj, b0, b1, b2, a1, a2);
            }
            void setGain(float gain) { tBiQuad_setGain(&obj, gain); }
            void setSampleRate(float sr) { tBiQuad_setSampleRate(&obj, sr); }
        };

        struct svf {
            tSVF obj;

            void init(SVFType type, float freq, float Q, LEAF& leaf = main) { tSVF_init(&obj, type, freq, Q, &leaf); }
            void initToPool(SVFType type, float freq, float Q, tMempool* mempool)
            {
                tSVF_initToPool(&obj, type, freq, Q, mempool);
            }
            void free() { tSVF_free(&obj); }

            float tick(float v0) { return tSVF_tick(&obj, v0); }
            void setFreq(float freq) { tSVF_setFreq(&obj, freq); }
            void setFreqFast(float cutoff) { tSVF_setFreqFast(&obj, cutoff); }
            void setQ(float Q) { tSVF_setQ(&obj, Q); }
            void setFreqAndQ(float freq, float Q) { tSVF_setFreqAndQ(&obj, freq, Q); }
            void setSampleRate(float sr) { tSVF_setSampleRate(&obj, sr); }
        };

        struct efficient_svf {
            tEfficientSVF obj;

            void init(SVFType type, uint16_t input, float Q, LEAF& leaf = main)
            {
                tEfficientSVF_init(&obj, type, input, Q, &leaf);
            }
            void initToPool(SVFType type, uint16_t input, float Q, tMempool* mempool)
            {
                tEfficientSVF_initToPool(&obj, type, input, Q, mempool);
            }
            void free() { tEfficientSVF_free(&obj); }

            float tick(float v0) { return tEfficientSVF_tick(&obj, v0); }
            void setFreq(uint16_t controlFreq) { tEfficientSVF_setFreq(&obj, controlFreq); }
            void setQ(float Q) { tEfficientSVF_setQ(&obj, Q); }
            void setFreqAndQ(uint16_t controlFreq, float Q) { tEfficientSVF_setFreqAndQ(&obj, controlFreq, Q); }
        };

        struct highpass {
            tHighpass obj;

            void init(float freq, LEAF& leaf = main) { tHighpass_init(&obj, freq, &leaf); }
            void initToPool(float freq, tMempool* mempool) { tHighpass_initToPool(&obj, freq, mempool); }
            void free() { tHighpass_free(&obj); }

            float tick(float x) { return tHighpass_tick(&obj, x); }
            void setFreq(float freq) { tHighpass_setFreq(&obj, freq); }
            float getFreq() { return tHighpass_getFreq(&obj); }
            void setSampleRate(float sr) { tHighpass_setSampleRate(&obj, sr); }
        };

        struct butterworth {
            tButterworth obj;

            void init(int N, float f1, float f2, LEAF& leaf = main) { tButterworth_init(&obj, N, f1, f2, &leaf); }
            void initToPool(int N, float f1, float f2, tMempool* mempool)
            {
                tButterworth_initToPool(&obj, N, f1, f2, mempool);
            }
            void free() { tButterworth_free(&obj); }

            float tick(float input) { return tButterworth_tick(&obj, input); }
            void setF1(float in) { tButterworth_setF1(&obj, in); }
            void setF2(float in) { tButterworth_setF2(&obj, in); }
            void setFreqs(float f1, float f2) { tButterworth_setFreqs(&obj, f1, f2); }
            void setSampleRate(float sr) { tButterworth_setSampleRate(&obj, sr); }
        };

        struct fir {
            tFIR obj;

            void init(float* coeffs, int numTaps, LEAF& leaf = main) { tFIR_init(&obj, coeffs, numTaps, &leaf); }
            void initToPool(float* coeffs, int numTaps, tMempool* mempool)
            {
                tFIR_initToPool(&obj, coeffs, numTaps, mempool);
            }
            void free() { tFIR_free(&obj); }

            float tick(float input) { return tFIR_tick(&obj, input); }
        };

        struct median_filter {
            tMedianFilter obj;

            void init(int size, LEAF& leaf = main) { tMedianFilter_init(&obj, size, &leaf); }
            void initToPool(int size, tMempool* mempool) { tMedianFilter_initToPool(&obj, size, mempool); }
            void free() { tMedianFilter_free(&obj); }

            float tick(float input) { return tMedianFilter_tick(&obj, input); }
        };

        struct vzfilter {
            tVZFilter obj;

            void init(VZFilterType type, float freq, float Q, LEAF& leaf = main)
            {
                tVZFilter_init(&obj, type, freq, Q, &leaf);
            }
            void initToPool(VZFilterType type, float freq, float Q, tMempool* mempool)
            {
                tVZFilter_initToPool(&obj, type, freq, Q, mempool);
            }
            void free() { tVZFilter_free(&obj); }

            void setSampleRate(float sampleRate) { tVZFilter_setSampleRate(&obj, sampleRate); }
            float tick(float input) { return tVZFilter_tick(&obj, input); }
            float tickEfficient(float in) { return tVZFilter_tickEfficient(&obj, in); }
            void calcCoeffs() { tVZFilter_calcCoeffs(&obj); }
            void calcCoeffsEfficientBP() { tVZFilter_calcCoeffsEfficientBP(&obj); }
            void setBandwidth(float bandWidth) { tVZFilter_setBandwidth(&obj, bandWidth); }
            void setFreq(float freq) { tVZFilter_setFreq(&obj, freq); }
            void setFreqFast(float cutoff) { tVZFilter_setFreqFast(&obj, cutoff); }
            void setFreqAndBandwidth(float freq, float bw) { tVZFilter_setFreqAndBandwidth(&obj, freq, bw); }
            void setFreqAndBandwidthEfficientBP(float freq, float bw)
            {
                tVZFilter_setFreqAndBandwidthEfficientBP(&obj, freq, bw);
            }
            void setGain(float gain) { tVZFilter_setGain(&obj, gain); }
            void setResonance(float res) { tVZFilter_setResonance(&obj, res); }
            void setFrequencyAndResonance(float freq, float res)
            {
                tVZFilter_setFrequencyAndResonance(&obj, freq, res);
            }
            void setFrequencyAndResonanceAndGain(float freq, float res, float gains)
            {
                tVZFilter_setFrequencyAndResonanceAndGain(&obj, freq, res, gains);
            }
            void setFrequencyAndBandwidthAndGain(float freq, float BW, float gain)
            {
                tVZFilter_setFrequencyAndBandwidthAndGain(&obj, freq, BW, gain);
            }
            void setFrequencyAndResonanceAndMorph(float freq, float res, float morph)
            {
                tVZFilter_setFrequencyAndResonanceAndMorph(&obj, freq, res, morph);
            }
            void setMorphOnly(float morph) { tVZFilter_setMorphOnly(&obj, morph); }
            void setMorph(float morph) { tVZFilter_setMorph(&obj, morph); }
            void setType(VZFilterType type) { tVZFilter_setType(&obj, type); }
            float BandwidthToR(float B) { return tVZFilter_BandwidthToR(&obj, B); }
            float BandwidthToREfficientBP(float B) { return tVZFilter_BandwidthToREfficientBP(&obj, B); }
        };

        struct diode_filter {
            tDiodeFilter obj;

            void init(float freq, float Q, LEAF& leaf = main) { tDiodeFilter_init(&obj, freq, Q, &leaf); }
            void initToPool(float freq, float Q, tMempool* mempool) { tDiodeFilter_initToPool(&obj, freq, Q, mempool); }
            void free() { tDiodeFilter_free(&obj); }

            float tick(float input) { return tDiodeFilter_tick(&obj, input); }
            void setFreq(float cutoff) { tDiodeFilter_setFreq(&obj, cutoff); }
            void setFreqFast(float cutoff) { tDiodeFilter_setFreqFast(&obj, cutoff); }
            void setQ(float resonance) { tDiodeFilter_setQ(&obj, resonance); }
            void setSampleRate(float sr) { tDiodeFilter_setSampleRate(&obj, sr); }
        };

        struct ladder_filter {
            tLadderFilter obj;

            void init(float freq, float Q, LEAF& leaf = main) { tLadderFilter_init(&obj, freq, Q, &leaf); }
            void initToPool(float freq, float Q, tMempool* mempool)
            {
                tLadderFilter_initToPool(&obj, freq, Q, mempool);
            }
            void free() { tLadderFilter_free(&obj); }

            float tick(float input) { return tLadderFilter_tick(&obj, input); }
            void setFreq(float cutoff) { tLadderFilter_setFreq(&obj, cutoff); }
            void setFreqFast(float cutoff) { tLadderFilter_setFreqFast(&obj, cutoff); }
            void setQ(float resonance) { tLadderFilter_setQ(&obj, resonance); }
            void setSampleRate(float sr) { tLadderFilter_setSampleRate(&obj, sr); }
        };

    } // namespace filters

    namespace sampling
    {
        struct buffer {
            tBuffer obj;

            void init(uint32_t length, LEAF& leaf = main) { tBuffer_init(&obj, length, &leaf); }
            void initToPool(uint32_t length, tMempool* mp) { tBuffer_initToPool(&obj, length, mp); }
            void free() { tBuffer_free(&obj); }

            void tick(float sample) { tBuffer_tick(&obj, sample); }
            void read(float* buff, uint32_t len) { tBuffer_read(&obj, buff, len); }
            float get(int idx) { return tBuffer_get(&obj, idx); }
            void record() { tBuffer_record(&obj); }
            void stop() { tBuffer_stop(&obj); }
            void setBuffer(float* externalBuffer, int length, int channels, int sampleRate)
            {
                tBuffer_setBuffer(&obj, externalBuffer, length, channels, sampleRate);
            }
            int getRecordPosition() { return tBuffer_getRecordPosition(&obj); }
            void setRecordPosition(int pos) { tBuffer_setRecordPosition(&obj, pos); }
            void setRecordMode(RecordMode mode) { tBuffer_setRecordMode(&obj, mode); }
            void clear() { tBuffer_clear(&obj); }
            uint32_t getBufferLength() { return tBuffer_getBufferLength(&obj); }
            uint32_t getRecordedLength() { return tBuffer_getRecordedLength(&obj); }
            void setRecordedLength(int length) { tBuffer_setRecordedLength(&obj, length); }
            int isActive() { return tBuffer_isActive(&obj); }
        };

        struct sampler {
            tSampler obj;

            void init(tBuffer* buffer, LEAF& leaf = main) { tSampler_init(&obj, buffer, &leaf); }
            void initToPool(tBuffer* b, tMempool* mp, LEAF& leaf = main) { tSampler_initToPool(&obj, b, mp, &leaf); }
            void free() { tSampler_free(&obj); }

            float tick() { return tSampler_tick(&obj); }
            float tickStereo(float* outputArray) { return tSampler_tickStereo(&obj, outputArray); }
            void setSample(tBuffer* buffer) { tSampler_setSample(&obj, buffer); }
            void setMode(PlayMode mode) { tSampler_setMode(&obj, mode); }
            void play() { tSampler_play(&obj); }
            void stop() { tSampler_stop(&obj); }
            void setStart(int32_t start) { tSampler_setStart(&obj, start); }
            void setEnd(int32_t end) { tSampler_setEnd(&obj, end); }
            void setLength(int32_t length) { tSampler_setLength(&obj, length); }
            void setCrossfadeLength(uint32_t length) { tSampler_setCrossfadeLength(&obj, length); }
            void setRate(float rate) { tSampler_setRate(&obj, rate); }
            void setSampleRate(float sr) { tSampler_setSampleRate(&obj, sr); }
        };

        struct auto_sampler {
            tAutoSampler obj;

            void init(tBuffer* buffer, LEAF& leaf = main) { tAutoSampler_init(&obj, buffer, &leaf); }
            void initToPool(tBuffer* b, tMempool* mp, LEAF& leaf = main)
            {
                tAutoSampler_initToPool(&obj, b, mp, &leaf);
            }
            void free() { tAutoSampler_free(&obj); }

            float tick(float input) { return tAutoSampler_tick(&obj, input); }
            void setBuffer(tBuffer* buffer) { tAutoSampler_setBuffer(&obj, buffer); }
            void setMode(PlayMode mode) { tAutoSampler_setMode(&obj, mode); }
            void play() { tAutoSampler_play(&obj); }
            void stop() { tAutoSampler_stop(&obj); }
            void setThreshold(float thresh) { tAutoSampler_setThreshold(&obj, thresh); }
            void setWindowSize(uint32_t size) { tAutoSampler_setWindowSize(&obj, size); }
            void setCrossfadeLength(uint32_t length) { tAutoSampler_setCrossfadeLength(&obj, length); }
            void setRate(float rate) { tAutoSampler_setRate(&obj, rate); }
            void setSampleRate(float sr) { tAutoSampler_setSampleRate(&obj, sr); }
        };

        struct mbsampler {
            tMBSampler obj;

            void init(tBuffer* buffer, LEAF& leaf = main) { tMBSampler_init(&obj, buffer, &leaf); }
            void initToPool(tBuffer* buffer, tMempool* mempool) { tMBSampler_initToPool(&obj, buffer, mempool); }
            void free() { tMBSampler_free(&obj); }

            float tick() { return tMBSampler_tick(&obj); }
            void setSample(tBuffer* buffer) { tMBSampler_setSample(&obj, buffer); }
            void setMode(PlayMode mode) { tMBSampler_setMode(&obj, mode); }
            void play() { tMBSampler_play(&obj); }
            void stop() { tMBSampler_stop(&obj); }
            void setStart(int32_t start) { tMBSampler_setStart(&obj, start); }
            void setEnd(int32_t end) { tMBSampler_setEnd(&obj, end); }
            void setLength(int32_t length) { tMBSampler_setLength(&obj, length); }
            void setRate(float rate) { tMBSampler_setRate(&obj, rate); }
        };

    } // namespace sampling

    namespace dyn
    {
        struct compressor {
            tCompressor obj;

            void init(LEAF& leaf = main) { tCompressor_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tCompressor_initToPool(&obj, mempool); }
            void free() { tCompressor_free(&obj); }

            float tick(float input) { return tCompressor_tick(&obj, input); }
        };

        struct feedback_leveler {
            tFeedbackLeveler obj;

            void free() { tFeedbackLeveler_free(&obj); }

            float tick(float input) { return tFeedbackLeveler_tick(&obj, input); }
            float sample() { return tFeedbackLeveler_sample(&obj); }
            void setTargetLevel(float TargetLevel) { tFeedbackLeveler_setTargetLevel(&obj, TargetLevel); }
            void setFactor(float factor) { tFeedbackLeveler_setFactor(&obj, factor); }
            void setStrength(float strength) { tFeedbackLeveler_setStrength(&obj, strength); }
        };

        struct threshold {
            tThreshold obj;

            void init(float low, float high, LEAF& leaf = main) { tThreshold_init(&obj, low, high, &leaf); }
            void initToPool(float low, float high, tMempool* mempool)
            {
                tThreshold_initToPool(&obj, low, high, mempool);
            }
            void free() { tThreshold_free(&obj); }

            int tick(float input) { return tThreshold_tick(&obj, input); }
            void setLow(float low) { tThreshold_setLow(&obj, low); }
            void setHigh(float high) { tThreshold_setHigh(&obj, high); }
        };

    } // namespace dyn

    namespace mempool
    {
        struct mempool {
            tMempool obj;

            void init(char* memory, size_t size, LEAF& leaf = main) { tMempool_init(&obj, memory, size, &leaf); }


            void free() { tMempool_free(&obj); }


            void initToPool(char* memory, size_t size, tMempool* mem) { tMempool_initToPool(&obj, memory, size, mem); }
        };

        namespace mpool
        {
            inline void create(char* memory, size_t size, _tMempool* pool) { mpool_create(memory, size, pool); }

            inline char* alloc(size_t size, _tMempool* pool) { return mpool_alloc(size, pool); }
            inline char* calloc(size_t asize, _tMempool* pool) { return mpool_calloc(asize, pool); }

            inline void free(char* ptr, _tMempool* pool) { mpool_free(ptr, pool); }

            inline size_t get_size(_tMempool* pool) { return mpool_get_size(pool); }
            inline size_t get_used(_tMempool* pool) { return mpool_get_used(pool); }
        }; // namespace mpool

        namespace leaf
        {
            inline void pool_init(LEAF& leaf, char* memory, size_t size) { leaf_pool_init(&leaf, memory, size); }

            inline char* alloc(LEAF& leaf, size_t size) { return leaf_alloc(&leaf, size); }
            inline char* calloc(LEAF& leaf, size_t size) { return leaf_calloc(&leaf, size); }

            inline void free(LEAF& leaf, char* ptr) { leaf_free(&leaf, ptr); }

            inline size_t pool_get_size(LEAF& leaf = main) { return leaf_pool_get_size(&leaf); }
            inline size_t pool_get_used(LEAF& leaf = main) { return leaf_pool_get_used(&leaf); }

            inline char* pool_get_pool(LEAF& leaf = main) { return leaf_pool_get_pool(&leaf); }
        }; // namespace leaf

    } // namespace mempool

    namespace analysis
    {
        struct envelope_follower {
            tEnvelopeFollower obj;

            void free() { tEnvelopeFollower_free(&obj); }

            float tick(float sample) { return tEnvelopeFollower_tick(&obj, sample); }
            void setDecayCoefficient(float decayCoefficient)
            {
                tEnvelopeFollower_setDecayCoefficient(&obj, decayCoefficient);
            }
            void setAttackThreshold(float attackThreshold)
            {
                tEnvelopeFollower_setAttackThreshold(&obj, attackThreshold);
            }
        };

        struct zero_crossing_counter {
            tZeroCrossingCounter obj;

            void init(int maxWindowSize, LEAF& leaf = main) { tZeroCrossingCounter_init(&obj, maxWindowSize, &leaf); }
            void initToPool(int maxWindowSize, tMempool* mempool)
            {
                tZeroCrossingCounter_initToPool(&obj, maxWindowSize, mempool);
            }
            void free() { tZeroCrossingCounter_free(&obj); }

            float tick(float input) { return tZeroCrossingCounter_tick(&obj, input); }
            void setWindowSize(float windowSize) { tZeroCrossingCounter_setWindowSize(&obj, windowSize); }
        };

        struct power_follower {
            tPowerFollower obj;

            void init(float factor, LEAF& leaf = main) { tPowerFollower_init(&obj, factor, &leaf); }
            void initToPool(float factor, tMempool* mempool) { tPowerFollower_initToPool(&obj, factor, mempool); }
            void free() { tPowerFollower_free(&obj); }

            float tick(float input) { return tPowerFollower_tick(&obj, input); }
            float getPower() { return tPowerFollower_getPower(&obj); }
            void setFactor(float factor) { tPowerFollower_setFactor(&obj, factor); }
        };

        struct env_pd {
            tEnvPD obj;

            void init(int windowSize, int hopSize, int blockSize, LEAF& leaf = main)
            {
                tEnvPD_init(&obj, windowSize, hopSize, blockSize, &leaf);
            }
            void initToPool(int windowSize, int hopSize, int blockSize, tMempool* mempool)
            {
                tEnvPD_initToPool(&obj, windowSize, hopSize, blockSize, mempool);
            }
            void free() { tEnvPD_free(&obj); }

            float tick() { return tEnvPD_tick(&obj); }
            void processBlock(float* in) { tEnvPD_processBlock(&obj, in); }
        };

        struct attack_detection {
            tAttackDetection obj;

            void init(int blocksize, int atk, int rel, LEAF& leaf = main)
            {
                tAttackDetection_init(&obj, blocksize, atk, rel, &leaf);
            }
            void initToPool(int blocksize, int atk, int rel, tMempool* mempool)
            {
                tAttackDetection_initToPool(&obj, blocksize, atk, rel, mempool);
            }
            void free() { tAttackDetection_free(&obj); }

            void setBlocksize(int size) { tAttackDetection_setBlocksize(&obj, size); }
            void setSamplerate(int inRate) { tAttackDetection_setSamplerate(&obj, inRate); }
            void setAttack(int inAtk) { tAttackDetection_setAttack(&obj, inAtk); }
            void setRelease(int inRel) { tAttackDetection_setRelease(&obj, inRel); }
            void setThreshold(float thres) { tAttackDetection_setThreshold(&obj, thres); }
            int detect(float* in) { return tAttackDetection_detect(&obj, in); }
            void setSampleRate(float sr) { tAttackDetection_setSampleRate(&obj, sr); }
        };

        struct snac {
            tSNAC obj;

            void init(int overlaparg, LEAF& leaf = main) { tSNAC_init(&obj, overlaparg, &leaf); }
            void initToPool(int overlaparg, tMempool* mempool) { tSNAC_initToPool(&obj, overlaparg, mempool); }
            void free() { tSNAC_free(&obj); }

            void ioSamples(float* in, int size) { tSNAC_ioSamples(&obj, in, size); }
            void setOverlap(int lap) { tSNAC_setOverlap(&obj, lap); }
            void setBias(float bias) { tSNAC_setBias(&obj, bias); }
            void setMinRMS(float rms) { tSNAC_setMinRMS(&obj, rms); }

            float getPeriod() { return tSNAC_getPeriod(&obj); }
            float getFidelity() { return tSNAC_getFidelity(&obj); }
        };

        struct period_detection {
            tPeriodDetection obj;

            void init(float* in, int bufSize, int frameSize, LEAF& leaf = main)
            {
                tPeriodDetection_init(&obj, in, bufSize, frameSize, &leaf);
            }
            void initToPool(float* in, int bufSize, int frameSize, tMempool* mempool)
            {
                tPeriodDetection_initToPool(&obj, in, bufSize, frameSize, mempool);
            }
            void free() { tPeriodDetection_free(&obj); }

            float tick(float sample) { return tPeriodDetection_tick(&obj, sample); }
            float getPeriod() { return tPeriodDetection_getPeriod(&obj); }
            float getFidelity() { return tPeriodDetection_getFidelity(&obj); }
            void setHopSize(int hs) { tPeriodDetection_setHopSize(&obj, hs); }
            void setWindowSize(int ws) { tPeriodDetection_setWindowSize(&obj, ws); }
            void setFidelityThreshold(float threshold) { tPeriodDetection_setFidelityThreshold(&obj, threshold); }
            void setAlpha(float alpha) { tPeriodDetection_setAlpha(&obj, alpha); }
            void setTolerance(float tolerance) { tPeriodDetection_setTolerance(&obj, tolerance); }
            void setSampleRate(float sr) { tPeriodDetection_setSampleRate(&obj, sr); }
        };

        struct zero_crossing_info {
            tZeroCrossingInfo obj;

            void init(LEAF& leaf = main) { tZeroCrossingInfo_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tZeroCrossingInfo_initToPool(&obj, mempool); }
            void free() { tZeroCrossingInfo_free(&obj); }

            int tick(float s) { return tZeroCrossingInfo_tick(&obj, s); }
            int getState() { return tZeroCrossingInfo_getState(&obj); }
            void updatePeak(float s, int pos) { tZeroCrossingInfo_updatePeak(&obj, s, pos); }
            int period(tZeroCrossingInfo* next) { return tZeroCrossingInfo_period(&obj, next); }
            float fractionalPeriod(tZeroCrossingInfo* next) { return tZeroCrossingInfo_fractionalPeriod(&obj, next); }
            int getWidth() { return tZeroCrossingInfo_getWidth(&obj); }
        };

        struct zero_crossing_collector {
            tZeroCrossingCollector obj;

            void init(int windowSize, float hysteresis, LEAF& leaf = main)
            {
                tZeroCrossingCollector_init(&obj, windowSize, hysteresis, &leaf);
            }
            void free() { tZeroCrossingCollector_free(&obj); }

            int tick(float s) { return tZeroCrossingCollector_tick(&obj, s); }
            int getState() { return tZeroCrossingCollector_getState(&obj); }

            int getNumEdges() { return tZeroCrossingCollector_getNumEdges(&obj); }
            int getCapacity() { return tZeroCrossingCollector_getCapacity(&obj); }
            int getFrame() { return tZeroCrossingCollector_getFrame(&obj); }
            int getWindowSize() { return tZeroCrossingCollector_getWindowSize(&obj); }

            int isReady() { return tZeroCrossingCollector_isReady(&obj); }
            float getPeak() { return tZeroCrossingCollector_getPeak(&obj); }
            int isReset() { return tZeroCrossingCollector_isReset(&obj); }


            void setHysteresis(float hysteresis) { tZeroCrossingCollector_setHysteresis(&obj, hysteresis); }
        };

        struct bitset {
            tBitset obj;

            void init(int numBits, LEAF& leaf = main) { tBitset_init(&obj, numBits, &leaf); }
            void initToPool(int numBits, tMempool* mempool) { tBitset_initToPool(&obj, numBits, mempool); }
            void free() { tBitset_free(&obj); }

            int get(int index) { return tBitset_get(&obj, index); }

            void set(int index, unsigned int val) { tBitset_set(&obj, index, val); }
            void setMultiple(int index, int n, unsigned int val) { tBitset_setMultiple(&obj, index, n, val); }

            int getSize() { return tBitset_getSize(&obj); }
            void clear() { tBitset_clear(&obj); }
        };

        struct bacf {
            tBACF obj;

            void init(tBitset* bitset, LEAF& leaf = main) { tBACF_init(&obj, bitset, &leaf); }
            void initToPool(tBitset* bitset, tMempool* mempool) { tBACF_initToPool(&obj, bitset, mempool); }
            void free() { tBACF_free(&obj); }

            int getCorrelation(int pos) { return tBACF_getCorrelation(&obj, pos); }
            void set(tBitset* bitset) { tBACF_set(&obj, bitset); }
        };

        struct period_detector {
            tPeriodDetector obj;

            void free() { tPeriodDetector_free(&obj); }

            int tick(float sample) { return tPeriodDetector_tick(&obj, sample); }

            float getPeriod() { return tPeriodDetector_getPeriod(&obj); }
            float getPeriodicity() { return tPeriodDetector_getPeriodicity(&obj); }
            float harmonic(int harmonicIndex) { return tPeriodDetector_harmonic(&obj, harmonicIndex); }
            float predictPeriod() { return tPeriodDetector_predictPeriod(&obj); }
            int isReady() { return tPeriodDetector_isReady(&obj); }
            int isReset() { return tPeriodDetector_isReset(&obj); }

            void setHysteresis(float hysteresis) { tPeriodDetector_setHysteresis(&obj, hysteresis); }
            void setSampleRate(float sr) { tPeriodDetector_setSampleRate(&obj, sr); }
        };

        struct pitch_detector {
            tPitchDetector obj;

            void init(float lowestFreq, float highestFreq, LEAF& leaf = main)
            {
                tPitchDetector_init(&obj, lowestFreq, highestFreq, &leaf);
            }
            void free() { tPitchDetector_free(&obj); }

            int tick(float sample) { return tPitchDetector_tick(&obj, sample); }
            float getFrequency() { return tPitchDetector_getFrequency(&obj); }
            float getPeriodicity() { return tPitchDetector_getPeriodicity(&obj); }
            float harmonic(int harmonicIndex) { return tPitchDetector_harmonic(&obj, harmonicIndex); }
            float predictFrequency() { return tPitchDetector_predictFrequency(&obj); }
            int indeterminate() { return tPitchDetector_indeterminate(&obj); }

            void setHysteresis(float hysteresis) { tPitchDetector_setHysteresis(&obj, hysteresis); }
            void setSampleRate(float sr) { tPitchDetector_setSampleRate(&obj, sr); }
        };

        struct dual_pitch_detector {
            tDualPitchDetector obj;

            void free() { tDualPitchDetector_free(&obj); }

            int tick(float sample) { return tDualPitchDetector_tick(&obj, sample); }
            float getFrequency() { return tDualPitchDetector_getFrequency(&obj); }
            float getPeriodicity() { return tDualPitchDetector_getPeriodicity(&obj); }
            float harmonic(int harmonicIndex) { return tDualPitchDetector_harmonic(&obj, harmonicIndex); }
            float predictFrequency() { return tDualPitchDetector_predictFrequency(&obj); }

            void setHysteresis(float hysteresis) { tDualPitchDetector_setHysteresis(&obj, hysteresis); }
            void setPeriodicityThreshold(float thresh) { tDualPitchDetector_setPeriodicityThreshold(&obj, thresh); }
            void setSampleRate(float sr) { tDualPitchDetector_setSampleRate(&obj, sr); }
        };

    } // namespace analysis

    namespace physical
    {
        struct pluck {
            tPluck obj;

            void init(float lowestFrequency, LEAF leaf); // float* delayBuff[DELAY_LENGTH]) { tPluck_init(&obj,
                                                         // lowestFrequency, delayBuff[DELAY_LENGTH]); }
            void initToPool(float lowestFrequency, tMempool* mempool)
            {
                tPluck_initToPool(&obj, lowestFrequency, mempool);
            }
            void free() { tPluck_free(&obj); }

            float tick() { return tPluck_tick(&obj); }
            void pluck_(float amplitude) { tPluck_pluck(&obj, amplitude); }
            void noteOn(float frequency, float amplitude) { tPluck_noteOn(&obj, frequency, amplitude); }
            void noteOff(float amplitude) { tPluck_noteOff(&obj, amplitude); }
            void setFrequency(float frequency) { tPluck_setFrequency(&obj, frequency); }
            void controlChange(int number, float value) { tPluck_controlChange(&obj, number, value); }
            float getLastOut() { return tPluck_getLastOut(&obj); }
            void setSampleRate(float sr) { tPluck_setSampleRate(&obj, sr); }
        };

        struct karplus_strong {
            tKarplusStrong obj;

            void initToPool(float lowestFrequency, tMempool* mempool)
            {
                tKarplusStrong_initToPool(&obj, lowestFrequency, mempool);
            }
            void free() { tKarplusStrong_free(&obj); }

            float tick() { return tKarplusStrong_tick(&obj); }
            void pluck(float amplitude) { tKarplusStrong_pluck(&obj, amplitude); }
            void noteOn(float frequency, float amplitude) { tKarplusStrong_noteOn(&obj, frequency, amplitude); }
            void noteOff(float amplitude) { tKarplusStrong_noteOff(&obj, amplitude); }
            void setFrequency(float frequency) { tKarplusStrong_setFrequency(&obj, frequency); }
            void controlChange(SKControlType type, float value) { tKarplusStrong_controlChange(&obj, type, value); }
            void setStretch(float stretch) { tKarplusStrong_setStretch(&obj, stretch); }
            void setPickupPosition(float position) { tKarplusStrong_setPickupPosition(&obj, position); }
            void setBaseLoopGain(float aGain) { tKarplusStrong_setBaseLoopGain(&obj, aGain); }
            float getLastOut() { return tKarplusStrong_getLastOut(&obj); }
        };

        struct simple_living_string {
            tSimpleLivingString obj;

            void free() { tSimpleLivingString_free(&obj); }

            float tick(float input) { return tSimpleLivingString_tick(&obj, input); }
            float sample() { return tSimpleLivingString_sample(&obj); }
            void setFreq(float freq) { tSimpleLivingString_setFreq(&obj, freq); }
            void setWaveLength(float waveLength) { tSimpleLivingString_setWaveLength(&obj, waveLength); }
            void setDampFreq(float dampFreq) { tSimpleLivingString_setDampFreq(&obj, dampFreq); }
            void setDecay(float decay) { tSimpleLivingString_setDecay(&obj, decay); }
            void setTargetLev(float targetLev) { tSimpleLivingString_setTargetLev(&obj, targetLev); }
            void setLevSmoothFactor(float levSmoothFactor)
            {
                tSimpleLivingString_setLevSmoothFactor(&obj, levSmoothFactor);
            }
            void setLevStrength(float levStrength) { tSimpleLivingString_setLevStrength(&obj, levStrength); }
            void setLevMode(int levMode) { tSimpleLivingString_setLevMode(&obj, levMode); }
            void setSampleRate(float sr) { tSimpleLivingString_setSampleRate(&obj, sr); }
        };

        struct simple_living_string2 {
            tSimpleLivingString2 obj;

            void free() { tSimpleLivingString2_free(&obj); }

            float tick(float input) { return tSimpleLivingString2_tick(&obj, input); }
            float sample() { return tSimpleLivingString2_sample(&obj); }
            void setFreq(float freq) { tSimpleLivingString2_setFreq(&obj, freq); }
            void setWaveLength(float waveLength) { tSimpleLivingString2_setWaveLength(&obj, waveLength); }
            void setBrightness(float brightness) { tSimpleLivingString2_setBrightness(&obj, brightness); }
            void setDecay(float decay) { tSimpleLivingString2_setDecay(&obj, decay); }
            void setTargetLev(float targetLev) { tSimpleLivingString2_setTargetLev(&obj, targetLev); }
            void setLevSmoothFactor(float levSmoothFactor)
            {
                tSimpleLivingString2_setLevSmoothFactor(&obj, levSmoothFactor);
            }
            void setLevStrength(float levStrength) { tSimpleLivingString2_setLevStrength(&obj, levStrength); }
            void setLevMode(int levMode) { tSimpleLivingString2_setLevMode(&obj, levMode); }
            void setSampleRate(float sr) { tSimpleLivingString2_setSampleRate(&obj, sr); }
        };

        struct living_string {
            tLivingString obj;

            void free() { tLivingString_free(&obj); }

            float tick(float input) { return tLivingString_tick(&obj, input); }
            float sample() { return tLivingString_sample(&obj); }
            void setFreq(float freq) { tLivingString_setFreq(&obj, freq); }
            void setWaveLength(float waveLength) { tLivingString_setWaveLength(&obj, waveLength); }
            void setPickPos(float pickPos) { tLivingString_setPickPos(&obj, pickPos); }
            void setPrepIndex(float prepIndex) { tLivingString_setPrepIndex(&obj, prepIndex); }
            void setDampFreq(float dampFreq) { tLivingString_setDampFreq(&obj, dampFreq); }
            void setDecay(float decay) { tLivingString_setDecay(&obj, decay); }
            void setTargetLev(float targetLev) { tLivingString_setTargetLev(&obj, targetLev); }
            void setLevSmoothFactor(float levSmoothFactor) { tLivingString_setLevSmoothFactor(&obj, levSmoothFactor); }
            void setLevStrength(float levStrength) { tLivingString_setLevStrength(&obj, levStrength); }
            void setLevMode(int levMode) { tLivingString_setLevMode(&obj, levMode); }
            void setSampleRate(float sr) { tLivingString_setSampleRate(&obj, sr); }
        };

        struct living_string2 {
            tLivingString2 obj;

            void free() { tLivingString2_free(&obj); }

            float tick(float input) { return tLivingString2_tick(&obj, input); }
            float tickEfficient(float input) { return tLivingString2_tickEfficient(&obj, input); }

            float sample() { return tLivingString2_sample(&obj); }
            void setFreq(float freq) { tLivingString2_setFreq(&obj, freq); }
            void setWaveLength(float waveLength) { tLivingString2_setWaveLength(&obj, waveLength); }
            void setPickPos(float pickPos) { tLivingString2_setPickPos(&obj, pickPos); }
            void setPrepPos(float prepPos) { tLivingString2_setPrepPos(&obj, prepPos); }
            void setPickupPos(float pickupPos) { tLivingString2_setPickupPos(&obj, pickupPos); }
            void setPrepIndex(float prepIndex) { tLivingString2_setPrepIndex(&obj, prepIndex); }
            void setBrightness(float brightness) { tLivingString2_setBrightness(&obj, brightness); }
            void setDecay(float decay) { tLivingString2_setDecay(&obj, decay); }
            void setTargetLev(float targetLev) { tLivingString2_setTargetLev(&obj, targetLev); }
            void setLevSmoothFactor(float levSmoothFactor) { tLivingString2_setLevSmoothFactor(&obj, levSmoothFactor); }
            void setLevStrength(float levStrength) { tLivingString2_setLevStrength(&obj, levStrength); }
            void setLevMode(int levMode) { tLivingString2_setLevMode(&obj, levMode); }
            void setSampleRate(float sr) { tLivingString2_setSampleRate(&obj, sr); }
        };

        struct complex_living_string {
            tComplexLivingString obj;

            void free() { tComplexLivingString_free(&obj); }

            float tick(float input) { return tComplexLivingString_tick(&obj, input); }
            float sample() { return tComplexLivingString_sample(&obj); }
            void setFreq(float freq) { tComplexLivingString_setFreq(&obj, freq); }
            void setWaveLength(float waveLength) { tComplexLivingString_setWaveLength(&obj, waveLength); }
            void setPickPos(float pickPos) { tComplexLivingString_setPickPos(&obj, pickPos); }
            void setPrepPos(float prepPos) { tComplexLivingString_setPrepPos(&obj, prepPos); }
            void setPrepIndex(float prepIndex) { tComplexLivingString_setPrepIndex(&obj, prepIndex); }
            void setDampFreq(float dampFreq) { tComplexLivingString_setDampFreq(&obj, dampFreq); }
            void setDecay(float decay) { tComplexLivingString_setDecay(&obj, decay); }
            void setTargetLev(float targetLev) { tComplexLivingString_setTargetLev(&obj, targetLev); }
            void setLevSmoothFactor(float levSmoothFactor)
            {
                tComplexLivingString_setLevSmoothFactor(&obj, levSmoothFactor);
            }
            void setLevStrength(float levStrength) { tComplexLivingString_setLevStrength(&obj, levStrength); }
            void setLevMode(int levMode) { tComplexLivingString_setLevMode(&obj, levMode); }
            void setSampleRate(float sr) { tComplexLivingString_setSampleRate(&obj, sr); }
        };

        struct reed_table {
            tReedTable obj;

            void init(float offset, float slope, LEAF& leaf = main) { tReedTable_init(&obj, offset, slope, &leaf); }
            void initToPool(float offset, float slope, tMempool* mempool)
            {
                tReedTable_initToPool(&obj, offset, slope, mempool);
            }
            void free() { tReedTable_free(&obj); }

            float tick(float input) { return tReedTable_tick(&obj, input); }
            void setOffset(float offset) { tReedTable_setOffset(&obj, offset); }
            void setSlope(float slope) { tReedTable_setSlope(&obj, slope); }
        };

    } // namespace physical

    namespace fx
    {
        struct talkbox {
            tTalkbox obj;

            void init(int bufsize, LEAF& leaf = main) { tTalkbox_init(&obj, bufsize, &leaf); }
            void initToPool(int bufsize, tMempool* mempool) { tTalkbox_initToPool(&obj, bufsize, mempool); }
            void free() { tTalkbox_free(&obj); }

            float tick(float synth, float voice) { return tTalkbox_tick(&obj, synth, voice); }
            float tickFrozen(float synth, float voice) { return tTalkbox_tickFrozen(&obj, synth, voice); }
            void update() { tTalkbox_update(&obj); }
            void suspend() { tTalkbox_suspend(&obj); }
            inline void lpcDurbin(float* r, int p, float* k, float* g) { tTalkbox_lpcDurbin(r, p, k, g); }
            void setQuality(float quality) { tTalkbox_setQuality(&obj, quality); }
            void setWarpFactor(float warp) { tTalkbox_setWarpFactor(&obj, warp); }
            void setWarpOn(float warpOn) { tTalkbox_setWarpOn(&obj, warpOn); }
            void setFreeze(float freeze) { tTalkbox_setFreeze(&obj, freeze); }
            void setSampleRate(float sr) { tTalkbox_setSampleRate(&obj, sr); }
        };

        struct talkbox_float {
            tTalkboxFloat obj;

            void init(int bufsize, LEAF& leaf = main) { tTalkboxFloat_init(&obj, bufsize, &leaf); }
            void initToPool(int bufsize, tMempool* mempool) { tTalkboxFloat_initToPool(&obj, bufsize, mempool); }
            void free() { tTalkboxFloat_free(&obj); }

            float tick(float synth, float voice) { return tTalkboxFloat_tick(&obj, synth, voice); }
            float tickFrozen(float synth, float voice) { return tTalkboxFloat_tickFrozen(&obj, synth, voice); }
            void update() { tTalkboxFloat_update(&obj); }
            void suspend() { tTalkboxFloat_suspend(&obj); }
            inline void lpcDurbin(float* r, int p, float* k, float* g) { tTalkboxFloat_lpcDurbin(r, p, k, g); }
            void setQuality(float quality) { tTalkboxFloat_setQuality(&obj, quality); }
            void setWarpFactor(float warp) { tTalkboxFloat_setWarpFactor(&obj, warp); }
            void setWarpOn(int warpOn) { tTalkboxFloat_setWarpOn(&obj, warpOn); }
            void setFreeze(int freeze) { tTalkboxFloat_setFreeze(&obj, freeze); }
            void setSampleRate(float sr) { tTalkboxFloat_setSampleRate(&obj, sr); }
        };

        struct vocoder {
            tVocoder obj;

            void init(LEAF& leaf = main) { tVocoder_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tVocoder_initToPool(&obj, mempool); }
            void free() { tVocoder_free(&obj); }

            float tick(float synth, float voice) { return tVocoder_tick(&obj, synth, voice); }
            void update() { tVocoder_update(&obj); }
            void suspend() { tVocoder_suspend(&obj); }
            void setSampleRate(float sr) { tVocoder_setSampleRate(&obj, sr); }
        };

        struct rosenberg_glottal_pulse {
            tRosenbergGlottalPulse obj;

            void init(LEAF& leaf = main) { tRosenbergGlottalPulse_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tRosenbergGlottalPulse_initToPool(&obj, mempool); }
            void free() { tRosenbergGlottalPulse_free(&obj); }

            float tick() { return tRosenbergGlottalPulse_tick(&obj); }
            float tickHQ() { return tRosenbergGlottalPulse_tickHQ(&obj); }
            void setFreq(float freq) { tRosenbergGlottalPulse_setFreq(&obj, freq); }

            void setOpenLength(float openLength) { tRosenbergGlottalPulse_setOpenLength(&obj, openLength); }

            void setPulseLength(float pulseLength) { tRosenbergGlottalPulse_setPulseLength(&obj, pulseLength); }
            void setSampleRate(float sr) { tRosenbergGlottalPulse_setSampleRate(&obj, sr); }
        };

        struct solad {
            tSOLAD obj;

            void init(int loopSize, LEAF& leaf = main) { tSOLAD_init(&obj, loopSize, &leaf); }
            void initToPool(int loopSize, tMempool* mempool) { tSOLAD_initToPool(&obj, loopSize, mempool); }
            void free() { tSOLAD_free(&obj); }

            void ioSamples(float* in, float* out, int blocksize) { tSOLAD_ioSamples(&obj, in, out, blocksize); }
            void setPeriod(float period) { tSOLAD_setPeriod(&obj, period); }
            void setPitchFactor(float pitchfactor) { tSOLAD_setPitchFactor(&obj, pitchfactor); }
            void setReadLag(float readlag) { tSOLAD_setReadLag(&obj, readlag); }
            void resetState() { tSOLAD_resetState(&obj); }
            void setSampleRate(float sr) { tSOLAD_setSampleRate(&obj, sr); }
        };

        struct pitch_shift {
            tPitchShift obj;

            void init(tDualPitchDetector* dual_pitch_detector, int bufSize, LEAF& leaf = main)
            {
                tPitchShift_init(&obj, dual_pitch_detector, bufSize, &leaf);
            }
            void initToPool(tDualPitchDetector* dual_pitch_detector, int bufSize, tMempool* mempool)
            {
                tPitchShift_initToPool(&obj, dual_pitch_detector, bufSize, mempool);
            }
            void free() { tPitchShift_free(&obj); }

            void shiftBy(float factor, float* in, float* out) { tPitchShift_shiftBy(&obj, factor, in, out); }
            void shiftTo(float freq, float* in, float* out) { tPitchShift_shiftTo(&obj, freq, in, out); }

            void setPickiness(float p) { tPitchShift_setPickiness(&obj, p); }
            void setSampleRate(float sr) { tPitchShift_setSampleRate(&obj, sr); }
        };

        struct simple_retune {
            tSimpleRetune obj;

            void free() { tSimpleRetune_free(&obj); }

            float tick(float sample) { return tSimpleRetune_tick(&obj, sample); }
            void setMode(int mode) { tSimpleRetune_setMode(&obj, mode); }
            void setNumVoices(int numVoices) { tSimpleRetune_setNumVoices(&obj, numVoices); }
            void setPickiness(float p) { tSimpleRetune_setPickiness(&obj, p); }
            void tuneVoices(float* t) { tSimpleRetune_tuneVoices(&obj, t); }
            void tuneVoice(int voice, float t) { tSimpleRetune_tuneVoice(&obj, voice, t); }
            float getInputFrequency() { return tSimpleRetune_getInputFrequency(&obj); }
            void setSampleRate(float sr) { tSimpleRetune_setSampleRate(&obj, sr); }
        };

        struct retune {
            tRetune obj;

            void init(int numVoices, float minInputFreq, float maxInputFreq, int bufSize, LEAF& leaf = main)
            {
                tRetune_init(&obj, numVoices, minInputFreq, maxInputFreq, bufSize, &leaf);
            }
            void free() { tRetune_free(&obj); }

            float* tick(float sample) { return tRetune_tick(&obj, sample); }
            void setMode(int mode) { tRetune_setMode(&obj, mode); }
            void setNumVoices(int numVoices) { tRetune_setNumVoices(&obj, numVoices); }
            void setPickiness(float p) { tRetune_setPickiness(&obj, p); }
            void tuneVoices(float* t) { tRetune_tuneVoices(&obj, t); }
            void tuneVoice(int voice, float t) { tRetune_tuneVoice(&obj, voice, t); }
            float getInputFrequency() { return tRetune_getInputFrequency(&obj); }
            void setSampleRate(float sr) { tRetune_setSampleRate(&obj, sr); }
        };

        struct formant_shifter {
            tFormantShifter obj;

            void init(int order, LEAF& leaf = main) { tFormantShifter_init(&obj, order, &leaf); }
            void initToPool(int order, tMempool* mempool) { tFormantShifter_initToPool(&obj, order, mempool); }
            void free() { tFormantShifter_free(&obj); }

            float tick(float input) { return tFormantShifter_tick(&obj, input); }
            float remove(float input) { return tFormantShifter_remove(&obj, input); }
            float add(float input) { return tFormantShifter_add(&obj, input); }
            void ioSamples(float* in, float* out, int size, float fwarp)
            {
                tFormantShifter_ioSamples(&obj, in, out, size, fwarp);
            }
            void setShiftFactor(float shiftFactor) { tFormantShifter_setShiftFactor(&obj, shiftFactor); }
            void setIntensity(float intensity) { tFormantShifter_setIntensity(&obj, intensity); }
            void setSampleRate(float sr) { tFormantShifter_setSampleRate(&obj, sr); }
        };

    } // namespace fx

    namespace rev
    {
        struct prcreverb {
            tPRCReverb obj;

            void init(float t60, LEAF& leaf = main) { tPRCReverb_init(&obj, t60, &leaf); }
            void initToPool(float t60, tMempool* mempool) { tPRCReverb_initToPool(&obj, t60, mempool); }
            void free() { tPRCReverb_free(&obj); }

            void clear() { tPRCReverb_clear(&obj); }
            float tick(float input) { return tPRCReverb_tick(&obj, input); }
            void setT60(float t60) { tPRCReverb_setT60(&obj, t60); }
            void setMix(float mix) { tPRCReverb_setMix(&obj, mix); }
            void setSampleRate(float sr) { tPRCReverb_setSampleRate(&obj, sr); }
        };

        struct nreverb {
            tNReverb obj;

            void init(float t60, LEAF& leaf = main) { tNReverb_init(&obj, t60, &leaf); }
            void initToPool(float t60, tMempool* mempool) { tNReverb_initToPool(&obj, t60, mempool); }
            void free() { tNReverb_free(&obj); }

            void clear() { tNReverb_clear(&obj); }
            float tick(float input) { return tNReverb_tick(&obj, input); }
            void tickStereo(float input, float* output) { tNReverb_tickStereo(&obj, input, output); }
            void setT60(float t60) { tNReverb_setT60(&obj, t60); }
            void setMix(float mix) { tNReverb_setMix(&obj, mix); }
            void setSampleRate(float sr) { tNReverb_setSampleRate(&obj, sr); }
        };

        struct dattorro_reverb {
            tDattorroReverb obj;

            void init(LEAF& leaf = main) { tDattorroReverb_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tDattorroReverb_initToPool(&obj, mempool); }
            void free() { tDattorroReverb_free(&obj); }

            void clear() { tDattorroReverb_clear(&obj); }
            float tick(float input) { return tDattorroReverb_tick(&obj, input); }
            void tickStereo(float input, float* output) { tDattorroReverb_tickStereo(&obj, input, output); }
            void setMix(float mix) { tDattorroReverb_setMix(&obj, mix); }
            void setFreeze(int freeze) { tDattorroReverb_setFreeze(&obj, freeze); }
            void setHP(float freq) { tDattorroReverb_setHP(&obj, freq); }
            void setSize(float size) { tDattorroReverb_setSize(&obj, size); }
            void setInputDelay(float preDelay) { tDattorroReverb_setInputDelay(&obj, preDelay); }
            void setInputFilter(float freq) { tDattorroReverb_setInputFilter(&obj, freq); }
            void setFeedbackFilter(float freq) { tDattorroReverb_setFeedbackFilter(&obj, freq); }
            void setFeedbackGain(float gain) { tDattorroReverb_setFeedbackGain(&obj, gain); }
            void setSampleRate(float sr) { tDattorroReverb_setSampleRate(&obj, sr); }
        };

    } // namespace rev

    namespace osc
    {
        struct cycle {
            tCycle obj;

            void init(LEAF& leaf = main) { tCycle_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tCycle_initToPool(&obj, mempool); }
            void free() { tCycle_free(&obj); }

            float tick() { return tCycle_tick(&obj); }
            void setFreq(float freq) { tCycle_setFreq(&obj, freq); }
            void setPhase(float phase) { tCycle_setPhase(&obj, phase); }
            void setSampleRate(float sr) { tCycle_setSampleRate(&obj, sr); }
        };

        struct triangle {
            tTriangle obj;

            void init(LEAF& leaf = main) { tTriangle_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tTriangle_initToPool(&obj, mempool); }
            void free() { tTriangle_free(&obj); }

            float tick() { return tTriangle_tick(&obj); }
            void setFreq(float freq) { tTriangle_setFreq(&obj, freq); }
            void setPhase(float phase) { tTriangle_setPhase(&obj, phase); }
            void setSampleRate(float sr) { tTriangle_setSampleRate(&obj, sr); }
        };

        struct square {
            tSquare obj;

            void init(LEAF& leaf = main) { tSquare_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSquare_initToPool(&obj, mempool); }
            void free() { tSquare_free(&obj); }

            float tick() { return tSquare_tick(&obj); }
            void setFreq(float freq) { tSquare_setFreq(&obj, freq); }
            void setPhase(float phase) { tSquare_setPhase(&obj, phase); }
            void setSampleRate(float sr) { tSquare_setSampleRate(&obj, sr); }
        };

        struct sawtooth {
            tSawtooth obj;

            void init(LEAF& leaf = main) { tSawtooth_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSawtooth_initToPool(&obj, mempool); }
            void free() { tSawtooth_free(&obj); }

            float tick() { return tSawtooth_tick(&obj); }
            void setFreq(float freq) { tSawtooth_setFreq(&obj, freq); }
            void setPhase(float phase) { tSawtooth_setPhase(&obj, phase); }
            void setSampleRate(float sr) { tSawtooth_setSampleRate(&obj, sr); }
        };

        struct pbtriangle {
            tPBTriangle obj;

            void init(LEAF& leaf = main) { tPBTriangle_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tPBTriangle_initToPool(&obj, mempool); }
            void free() { tPBTriangle_free(&obj); }

            float tick() { return tPBTriangle_tick(&obj); }
            void setFreq(float freq) { tPBTriangle_setFreq(&obj, freq); }
            void setSkew(float skew) { tPBTriangle_setSkew(&obj, skew); }
            void setSampleRate(float sr) { tPBTriangle_setSampleRate(&obj, sr); }
        };

        struct pbpulse {
            tPBPulse obj;

            void init(LEAF& leaf = main) { tPBPulse_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tPBPulse_initToPool(&obj, mempool); }
            void free() { tPBPulse_free(&obj); }

            float tick() { return tPBPulse_tick(&obj); }
            void setFreq(float freq) { tPBPulse_setFreq(&obj, freq); }
            void setWidth(float width) { tPBPulse_setWidth(&obj, width); }
            void setSampleRate(float sr) { tPBPulse_setSampleRate(&obj, sr); }
        };

        struct pbsaw {
            tPBSaw obj;

            void init(LEAF& leaf = main) { tPBSaw_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tPBSaw_initToPool(&obj, mempool); }
            void free() { tPBSaw_free(&obj); }

            float tick() { return tPBSaw_tick(&obj); }
            void setFreq(float freq) { tPBSaw_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tPBSaw_setSampleRate(&obj, sr); }
        };

        struct phasor {
            tPhasor obj;

            void init(LEAF& leaf = main) { tPhasor_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tPhasor_initToPool(&obj, mempool); }
            void free() { tPhasor_free(&obj); }

            float tick() { return tPhasor_tick(&obj); }
            void setFreq(float freq) { tPhasor_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tPhasor_setSampleRate(&obj, sr); }
        };

        struct noise {
            tNoise obj;

            void init(NoiseType type, LEAF& leaf = main) { tNoise_init(&obj, type, &leaf); }
            void initToPool(NoiseType type, tMempool* mempool) { tNoise_initToPool(&obj, type, mempool); }
            void free() { tNoise_free(&obj); }

            float tick() { return tNoise_tick(&obj); }
        };

        struct neuron {
            tNeuron obj;

            void init(LEAF& leaf = main) { tNeuron_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tNeuron_initToPool(&obj, mempool); }
            void free() { tNeuron_free(&obj); }

            void reset() { tNeuron_reset(&obj); }
            float tick() { return tNeuron_tick(&obj); }
            void setMode(NeuronMode mode) { tNeuron_setMode(&obj, mode); }
            void setCurrent(float current) { tNeuron_setCurrent(&obj, current); }
            void setK(float K) { tNeuron_setK(&obj, K); }
            void setL(float L) { tNeuron_setL(&obj, L); }
            void setN(float N) { tNeuron_setN(&obj, N); }
            void setC(float C) { tNeuron_setC(&obj, C); }
            void setV1(float V1) { tNeuron_setV1(&obj, V1); }
            void setV2(float V2) { tNeuron_setV2(&obj, V2); }
            void setV3(float V3) { tNeuron_setV3(&obj, V3); }
            void setTimeStep(float timestep) { tNeuron_setTimeStep(&obj, timestep); }
            void setSampleRate(float sr) { tNeuron_setSampleRate(&obj, sr); }
        };

        struct mbpulse {
            tMBPulse obj;

            void init(LEAF& leaf = main) { tMBPulse_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tMBPulse_initToPool(&obj, mempool); }
            void free() { tMBPulse_free(&obj); }

            float tick() { return tMBPulse_tick(&obj); }
            void setFreq(float f) { tMBPulse_setFreq(&obj, f); }
            void setWidth(float w) { tMBPulse_setWidth(&obj, w); }
            float sync(float sync) { return tMBPulse_sync(&obj, sync); }
            void setPhase(float phase) { tMBPulse_setPhase(&obj, phase); }
            void setSyncMode(int hardOrSoft) { tMBPulse_setSyncMode(&obj, hardOrSoft); }
            void setBufferOffset(uint32_t offset) { tMBPulse_setBufferOffset(&obj, offset); }
            void setSampleRate(float sr) { tMBPulse_setSampleRate(&obj, sr); }
        };

        struct mbtriangle {
            tMBTriangle obj;

            void init(LEAF& leaf = main) { tMBTriangle_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tMBTriangle_initToPool(&obj, mempool); }
            void free() { tMBTriangle_free(&obj); }

            float tick() { return tMBTriangle_tick(&obj); }
            void setFreq(float f) { tMBTriangle_setFreq(&obj, f); }
            void setWidth(float w) { tMBTriangle_setWidth(&obj, w); }
            float sync(float sync) { return tMBTriangle_sync(&obj, sync); }
            void setPhase(float phase) { tMBTriangle_setPhase(&obj, phase); }
            void setSyncMode(int hardOrSoft) { tMBTriangle_setSyncMode(&obj, hardOrSoft); }
            void setBufferOffset(uint32_t offset) { tMBTriangle_setBufferOffset(&obj, offset); }
            void setSampleRate(float sr) { tMBTriangle_setSampleRate(&obj, sr); }
        };

        struct mbsaw {
            tMBSaw obj;

            void init(LEAF& leaf = main) { tMBSaw_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tMBSaw_initToPool(&obj, mempool); }
            void free() { tMBSaw_free(&obj); }

            float tick() { return tMBSaw_tick(&obj); }
            void setFreq(float f) { tMBSaw_setFreq(&obj, f); }
            float sync(float sync) { return tMBSaw_sync(&obj, sync); }
            void setPhase(float phase) { tMBSaw_setPhase(&obj, phase); }
            void setSyncMode(int hardOrSoft) { tMBSaw_setSyncMode(&obj, hardOrSoft); }
            void setBufferOffset(uint32_t offset) { tMBSaw_setBufferOffset(&obj, offset); }
            void setSampleRate(float sr) { tMBSaw_setSampleRate(&obj, sr); }
        };

        struct mbsaw_pulse {
            tMBSawPulse obj;

            void init(LEAF& leaf = main) { tMBSawPulse_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tMBSawPulse_initToPool(&obj, mempool); }
            void free() { tMBSawPulse_free(&obj); }


            float tick() { return tMBSawPulse_tick(&obj); }
            float sync(float value) { return tMBSawPulse_sync(&obj, value); }
            void setFreq(float f) { tMBSawPulse_setFreq(&obj, f); }
            void setPhase(float phase) { tMBSawPulse_setPhase(&obj, phase); }
            void setShape(float shape) { tMBSawPulse_setShape(&obj, shape); }
            void setSyncMode(int hardOrSoft) { tMBSawPulse_setSyncMode(&obj, hardOrSoft); }
            void setBufferOffset(uint32_t offset) { tMBSawPulse_setBufferOffset(&obj, offset); }
            void setSampleRate(float sr) { tMBSawPulse_setSampleRate(&obj, sr); }
        };

        struct table {
            tTable obj;

            void init(float* table, int size, LEAF& leaf = main) { tTable_init(&obj, table, size, &leaf); }
            void initToPool(float* table, int size, tMempool* mempool)
            {
                tTable_initToPool(&obj, table, size, mempool);
            }
            void free() { tTable_free(&obj); }

            float tick() { return tTable_tick(&obj); }
            void setFreq(float freq) { tTable_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tTable_setSampleRate(&obj, sr); }
        };

        struct wave_table {
            tWaveTable obj;

            void init(float* table, int size, float maxFreq, LEAF& leaf = main)
            {
                tWaveTable_init(&obj, table, size, maxFreq, &leaf);
            }
            void initToPool(float* table, int size, float maxFreq, tMempool* mempool)
            {
                tWaveTable_initToPool(&obj, table, size, maxFreq, mempool);
            }
            void free() { tWaveTable_free(&obj); }
            void setSampleRate(float sr) { tWaveTable_setSampleRate(&obj, sr); }
        };

        struct wave_osc {
            tWaveOsc obj;

            void init(tWaveTable* tables, int numTables, LEAF& leaf = main)
            {
                tWaveOsc_init(&obj, tables, numTables, &leaf);
            }
            void initToPool(tWaveTable* tables, int numTables, tMempool* mp)
            {
                tWaveOsc_initToPool(&obj, tables, numTables, mp);
            }
            void free() { tWaveOsc_free(&obj); }

            float tick() { return tWaveOsc_tick(&obj); }
            void setFreq(float freq) { tWaveOsc_setFreq(&obj, freq); }
            void setAntiAliasing(float aa) { tWaveOsc_setAntiAliasing(&obj, aa); }
            void setIndex(float index) { tWaveOsc_setIndex(&obj, index); }
            void setTables(tWaveTable* tables, int numTables) { tWaveOsc_setTables(&obj, tables, numTables); }
            void setSampleRate(float sr) { tWaveOsc_setSampleRate(&obj, sr); }
        };

        struct wave_table_s {
            tWaveTableS obj;

            void init(float* table, int size, float maxFreq, LEAF& leaf = main)
            {
                tWaveTableS_init(&obj, table, size, maxFreq, &leaf);
            }
            void initToPool(float* table, int size, float maxFreq, tMempool* mempool)
            {
                tWaveTableS_initToPool(&obj, table, size, maxFreq, mempool);
            }
            void free() { tWaveTableS_free(&obj); }
            void setSampleRate(float sr) { tWaveTableS_setSampleRate(&obj, sr); }
        };

        struct wave_osc_s {
            tWaveOscS obj;

            void init(tWaveTableS* tables, int numTables, LEAF& leaf = main)
            {
                tWaveOscS_init(&obj, tables, numTables, &leaf);
            }
            void initToPool(tWaveTableS* tables, int numTables, tMempool* mempool)
            {
                tWaveOscS_initToPool(&obj, tables, numTables, mempool);
            }
            void free() { tWaveOscS_free(&obj); }

            float tick() { return tWaveOscS_tick(&obj); }
            void setFreq(float freq) { tWaveOscS_setFreq(&obj, freq); }
            void setAntiAliasing(float aa) { tWaveOscS_setAntiAliasing(&obj, aa); }
            void setIndex(float index) { tWaveOscS_setIndex(&obj, index); }
            void setSampleRate(float sr) { tWaveOscS_setSampleRate(&obj, sr); }
        };

        struct int_phasor {
            tIntPhasor obj;

            void init(LEAF& leaf = main) { tIntPhasor_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tIntPhasor_initToPool(&obj, mempool); }
            void free() { tIntPhasor_free(&obj); }

            float tick() { return tIntPhasor_tick(&obj); }
            void setFreq(float freq) { tIntPhasor_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tIntPhasor_setSampleRate(&obj, sr); }

            void setPhase(float phase) { tIntPhasor_setPhase(&obj, phase); }
        };

        struct square_lfo {
            tSquareLFO obj;

            void init(LEAF& leaf = main) { tSquareLFO_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSquareLFO_initToPool(&obj, mempool); }
            void free() { tSquareLFO_free(&obj); }

            float tick() { return tSquareLFO_tick(&obj); }
            void setFreq(float freq) { tSquareLFO_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tSquareLFO_setSampleRate(&obj, sr); }
            void setPulseWidth(float pw) { tSquareLFO_setPulseWidth(&obj, pw); }
            void setPhase(float phase) { tSquareLFO_setPhase(&obj, phase); }
        };

        struct saw_square_lfo {
            tSawSquareLFO obj;

            void init(LEAF& leaf = main) { tSawSquareLFO_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSawSquareLFO_initToPool(&obj, mempool); }
            void free() { tSawSquareLFO_free(&obj); }

            float tick() { return tSawSquareLFO_tick(&obj); }
            void setFreq(float freq) { tSawSquareLFO_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tSawSquareLFO_setSampleRate(&obj, sr); }
            void setPhase(float phase) { tSawSquareLFO_setPhase(&obj, phase); }
            void setShape(float shape) { tSawSquareLFO_setShape(&obj, shape); }
        };

        struct tri_lfo {
            tTriLFO obj;

            void init(LEAF& leaf = main) { tTriLFO_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tTriLFO_initToPool(&obj, mempool); }
            void free() { tTriLFO_free(&obj); }

            float tick() { return tTriLFO_tick(&obj); }
            void setFreq(float freq) { tTriLFO_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tTriLFO_setSampleRate(&obj, sr); }

            void setPhase(float phase) { tTriLFO_setPhase(&obj, phase); }
        };

        struct sine_tri_lfo {
            tSineTriLFO obj;

            void init(LEAF& leaf = main) { tSineTriLFO_init(&obj, &leaf); }
            void initToPool(tMempool* mempool) { tSineTriLFO_initToPool(&obj, mempool); }
            void free() { tSineTriLFO_free(&obj); }

            float tick() { return tSineTriLFO_tick(&obj); }
            void setFreq(float freq) { tSineTriLFO_setFreq(&obj, freq); }
            void setSampleRate(float sr) { tSineTriLFO_setSampleRate(&obj, sr); }
            void setPhase(float phase) { tSineTriLFO_setPhase(&obj, phase); }
            void setShape(float shape) { tSineTriLFO_setShape(&obj, shape); }
        };

    } // namespace osc

} // namespace leaf


#endif