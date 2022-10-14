/*==============================================================================

    leaf-dynamics.c
    Created: 30 Nov 2018 11:56:49am
    Author:  airship

==============================================================================*/

#if _WIN32 || _WIN64

#include "..\Inc\leaf-dynamics.h"

#else

#include "leaf/leaf-dynamics.h"

#endif

//==============================================================================

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Compressor ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //

/*
 tCompressor*    tCompressorInit(int tauAttack, int tauRelease)
 {
 tCompressor* c = &leaf.tCompressorRegistry[leaf.registryIndex[T_COMPRESSOR]++];

 c->tauAttack = tauAttack;
 c->tauRelease = tauRelease;

 c->x_G[0] = 0.0f, c->x_G[1] = 0.0f,
 c->y_G[0] = 0.0f, c->y_G[1] = 0.0f,
 c->x_T[0] = 0.0f, c->x_T[1] = 0.0f,
 c->y_T[0] = 0.0f, c->y_T[1] = 0.0f;

 c->T = 0.0f; // Threshold
 c->R = 1.0f; // compression Ratio
 c->M = 0.0f; // decibel Make-up gain
 c->W = 0.0f; // decibel Width of knee transition

 return c;
 }
 */
void tCompressor_init(tCompressor* const comp, LEAF* const leaf) { tCompressor_initToPool(comp, &leaf->mempool); }

void tCompressor_initToPool(tCompressor* const comp, tMempool* const mp)
{
    _tMempool* m    = *mp;
    _tCompressor* c = *comp = (_tCompressor*)mpool_alloc(sizeof(_tCompressor), m);
    c->mempool              = m;
    LEAF* leaf              = c->mempool->leaf;
    c->sampleRate           = leaf->sampleRate;

    c->tauAttack  = expf(-1.0f / (0.001f * 50.0f * c->sampleRate));
    c->tauRelease = expf(-1.0f / (0.001f * 100.0f * c->sampleRate));

    c->isActive = 0;
    c->x_G[0] = 0.0f, c->x_G[1] = 0.0f, c->y_G[0] = 0.0f, c->y_G[1] = 0.0f, c->x_T[0] = 0.0f, c->x_T[1] = 0.0f,
    c->y_T[0] = 0.0f, c->y_T[1] = 0.0f;
    c->T = 0.0f; // Threshold
    c->R = 0.5f; // compression Ratio
    c->M = 3.0f; // decibel Make-up gain
    c->W = 1.0f; // decibel Width of knee transition
}

void tCompressor_free(tCompressor* const comp)
{
    _tCompressor* c = *comp;

    mpool_free((char*)c, c->mempool);
}

float tCompressor_tick(tCompressor* const comp, float in)
{
    _tCompressor* c = *comp;

    float slope, overshoot;

    float in_db  = LEAF_clip(-90.0f, atodb(fabsf(in)), 0.0f);
    float out_db = 0.0f;

    c->y_T[1] = c->y_T[0];

    slope = 1.0f - (1.0f / c->R); // feed-forward topology;

    overshoot = in_db - c->T;

    /*
    if (overshoot <= -(c->W * 0.5f))
    {
        out_db = in_db;
        c->isActive = 0;
    }

    else if ((overshoot > -(c->W * 0.5f)) && (overshoot < (c->W * 0.5f)))
    {
        float squareit = (overshoot + c->W*0.5f);
        out_db = in_db + slope * ((squareit * squareit) / (2.0f * c->W)); // .^ 2 ???
        c->isActive = 1;
    }
    else if (overshoot >= (c->W * 0.5f))
    {
        out_db = in_db + slope * overshoot;
        c->isActive = 1;
    }
    */
    if (overshoot <= 0.0f) {
        out_db      = in_db;
        c->isActive = 0;
    } else {
        out_db      = in_db + slope * overshoot;
        c->isActive = 1;
    }

    c->x_T[0] = out_db - in_db;


    if (c->x_T[0] > c->y_T[1])
        c->y_T[0] = c->tauAttack * c->y_T[1] + (1.0f - c->tauAttack) * c->x_T[0];
    else
        c->y_T[0] = c->tauRelease * c->y_T[1] + (1.0f - c->tauRelease) * c->x_T[0];
    if (isnan(c->y_T[0])) {
        out_db = 1.0f;
    }

    float attenuation = fasterdbtoa(c->M - c->y_T[0]);
    if (isnan(attenuation)) {
        out_db = 1.0f;
    }
    // float attenuation = 1.0f;
    return attenuation * in;
}

////c->tauAttack = 100;
// c->tauRelease = 100;
//
// c->isActive = 0;
//
// c->T = 0.0f; // Threshold
// c->R = 0.5f; // compression Ratio
// c->W = 3.0f; // decibel Width of knee transition
// c->M = 1.0f; // decibel Make-up gain
void tCompressor_setParams(tCompressor* const comp, float thresh, float ratio, float knee, float makeup, float attack,
                           float release)
{
    _tCompressor* c = *comp;
    c->T            = thresh;
    c->R            = ratio;
    c->W            = knee;
    c->M            = makeup;
    c->tauAttack    = expf(-1.0f / (0.001f * attack * c->sampleRate));
    c->tauRelease   = expf(-1.0f / (0.001f * release * c->sampleRate));
}

/* Feedback Leveler */

void tFeedbackLeveler_init(tFeedbackLeveler* const fb, float targetLevel, float factor, float strength, int mode,
                           LEAF* const leaf)
{
    tFeedbackLeveler_initToPool(fb, targetLevel, factor, strength, mode, &leaf->mempool);
}

void tFeedbackLeveler_initToPool(tFeedbackLeveler* const fb, float targetLevel, float factor, float strength, int mode,
                                 tMempool* const mp)
{
    _tMempool* m         = *mp;
    _tFeedbackLeveler* p = *fb = (_tFeedbackLeveler*)mpool_alloc(sizeof(_tFeedbackLeveler), m);
    p->mempool                 = m;

    p->curr        = 0.0f;
    p->targetLevel = targetLevel;
    tPowerFollower_initToPool(&p->pwrFlw, factor, mp);
    p->mode     = mode;
    p->strength = strength;
}

void tFeedbackLeveler_free(tFeedbackLeveler* const fb)
{
    _tFeedbackLeveler* p = *fb;

    tPowerFollower_free(&p->pwrFlw);
    mpool_free((char*)p, p->mempool);
}

void tFeedbackLeveler_setStrength(tFeedbackLeveler* const fb, float strength)
{ // strength is how strongly level diff is affecting the amp ratio
    // try 0.125 for a start
    _tFeedbackLeveler* p = *fb;
    p->strength          = strength;
}

void tFeedbackLeveler_setFactor(tFeedbackLeveler* const fb, float factor)
{
    _tFeedbackLeveler* p = *fb;
    tPowerFollower_setFactor(&p->pwrFlw, factor);
}

void tFeedbackLeveler_setMode(tFeedbackLeveler* const fb, int mode)
{ // 0 for decaying with upwards lev limiting, 1 for constrained absolute level (also downwards limiting)
    _tFeedbackLeveler* p = *fb;
    p->mode              = mode;
}

float tFeedbackLeveler_tick(tFeedbackLeveler* const fb, float input)
{
    _tFeedbackLeveler* p = *fb;
    float levdiff        = (tPowerFollower_tick(&p->pwrFlw, input) - p->targetLevel);
    if (p->mode == 0 && levdiff < 0.0f)
        levdiff = 0.0f;
    p->curr = input * (1.0f - p->strength * levdiff);
    return p->curr;
}

float tFeedbackLeveler_sample(tFeedbackLeveler* const fb)
{
    _tFeedbackLeveler* p = *fb;
    return p->curr;
}


void tFeedbackLeveler_setTargetLevel(tFeedbackLeveler* const fb, float TargetLevel)
{
    _tFeedbackLeveler* p = *fb;
    p->targetLevel       = TargetLevel;
}


void tThreshold_init(tThreshold* const th, float low, float high, LEAF* const leaf)
{
    tThreshold_initToPool(th, low, high, &leaf->mempool);
}

void tThreshold_initToPool(tThreshold* const th, float low, float high, tMempool* const mp)
{
    _tMempool* m   = *mp;
    _tThreshold* t = *th = (_tThreshold*)mpool_alloc(sizeof(_tThreshold), m);
    t->mempool           = m;

    t->highThresh = high;
    t->lowThresh  = low;

    t->currentValue = 0;
}

void tThreshold_free(tThreshold* const th)
{
    _tThreshold* t = *th;

    mpool_free((char*)t, t->mempool);
}

int tThreshold_tick(tThreshold* const th, float in)
{
    _tThreshold* t = *th;

    if (in >= t->highThresh) {
        t->currentValue = 1;
    } else if (in <= t->lowThresh) {
        t->currentValue = 0;
    }

    return t->currentValue;
}

void tThreshold_setLow(tThreshold* const th, float low)
{
    _tThreshold* t = *th;

    t->lowThresh = low;
}

void tThreshold_setHigh(tThreshold* const th, float high)
{
    _tThreshold* t = *th;

    t->highThresh = high;
}
