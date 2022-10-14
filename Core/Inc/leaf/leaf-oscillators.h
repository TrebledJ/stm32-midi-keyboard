/*==============================================================================
 leaf-oscillators.h
 Created: 20 Jan 2017 12:00:58pm
 Author:  Michael R Mulshine
 ==============================================================================*/

#ifndef LEAF_OSCILLATORS_H_INCLUDED
#define LEAF_OSCILLATORS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================

#include "leaf-distortion.h"
#include "leaf-filters.h"
#include "leaf-math.h"
#include "leaf-mempool.h"
#include "leaf-tables.h"

/*!
 Header.
 @include basic-oscillators.h
 @example basic-oscillators.c
 An example.
 */

//==============================================================================

/*!
 @defgroup tcycle tCycle
 @ingroup oscillators
 @brief Wavetable cycle/sine wave oscillator
 @{

 @fn void    tCycle_init         (tCycle* const osc, LEAF* const leaf)
 @brief Initialize a tCycle to the default mempool of a LEAF instance.
 @param osc A pointer to the tCycle to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tCycle_initToPool   (tCycle* const osc, tMempool* const mempool)
 @brief Initialize a tCycle to a specified mempool.
 @param osc A pointer to the tCycle to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tCycle_free         (tCycle* const osc)
 @brief Free a tCycle from its mempool.
 @param osc A pointer to the tCycle to free.

 @fn float   tCycle_tick         (tCycle* const osc)
 @brief Tick a tCycle oscillator.
 @param osc A pointer to the relevant tCycle.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tCycle_setFreq      (tCycle* const osc, float freq)
 @brief Set the frequency of a tCycle oscillator.
 @param osc A pointer to the relevant tCycle.
 @param freq The frequency to set the oscillator to.

￼￼￼
 @} */

typedef struct _tCycle {
    tMempool mempool;
    // Underlying phasor
    uint32_t phase;
    int32_t inc;
    float freq;
    float invSampleRateTimesTwoTo32;
    uint32_t mask;
} _tCycle;

typedef _tCycle* tCycle;

void tCycle_init(tCycle* const osc, LEAF* const leaf);
void tCycle_initToPool(tCycle* const osc, tMempool* const mempool);
void tCycle_free(tCycle* const osc);

float tCycle_tick(tCycle* const osc);
void tCycle_setFreq(tCycle* const osc, float freq);
void tCycle_setPhase(tCycle* const osc, float phase);
void tCycle_setSampleRate(tCycle* const osc, float sr);

//==============================================================================

/*!
 @defgroup ttriangle tTriangle
 @ingroup oscillators
 @brief Anti-aliased wavetable triangle wave oscillator.
 @{

 @fn void    tTriangle_init         (tTriangle* const osc, LEAF* const leaf)
 @brief Initialize a tTriangle to the default mempool of a LEAF instance.
 @param osc A pointer to the tTriangle to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tTriangle_initToPool   (tTriangle* const osc, tMempool* const mempool)
 @brief Initialize a tTriangle to a specified mempool.
 @param osc A pointer to the tTriangle to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tTriangle_free         (tTriangle* const osc)
 @brief Free a tTriangle from its mempool.
 @param osc A pointer to the tTriangle to free.

 @fn float   tTriangle_tick         (tTriangle* const osc)
 @brief Tick a tTriangle oscillator.
 @param osc A pointer to the relevant tTriangle.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tTriangle_setFreq      (tTriangle* const osc, float freq)
 @brief Set the frequency of a tTriangle oscillator.
 @param osc A pointer to the relevant tTriangle.
 @param freq The frequency to set the oscillator to.

 @} */

typedef struct _tTriangle {
    tMempool mempool;
    // Underlying phasor
    uint32_t phase;
    uint32_t inc;
    float freq;
    int oct;
    float w;
    float invSampleRate;
    float invSampleRateTimesTwoTo32;
    uint32_t mask;
} _tTriangle;

typedef _tTriangle* tTriangle;

void tTriangle_init(tTriangle* const osc, LEAF* const leaf);
void tTriangle_initToPool(tTriangle* const osc, tMempool* const mempool);
void tTriangle_free(tTriangle* const osc);

float tTriangle_tick(tTriangle* const osc);
void tTriangle_setFreq(tTriangle* const osc, float freq);
void tTriangle_setPhase(tTriangle* const osc, float phase);
void tTriangle_setSampleRate(tTriangle* const osc, float sr);

//==============================================================================

/*!
 @defgroup tsquare tSquare
 @ingroup oscillators
 @brief Anti-aliased wavetable square wave oscillator.
 @{

 @fn void    tSquare_init         (tSquare* const osc, LEAF* const leaf)
 @brief Initialize a tSquare to the default mempool of a LEAF instance.
 @param osc A pointer to the tSquare to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tSquare_initToPool   (tSquare* const osc, tMempool* const mempool)
 @brief Initialize a tSquare to a specified mempool.
 @param osc A pointer to the tSquare to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tSquare_free         (tSquare* const osc)
 @brief Free a tSquare from its mempool.
 @param osc A pointer to the tSquare to free.

 @fn float   tSquare_tick         (tSquare* const osc)
 @brief Tick a tSquare oscillator.
 @param osc A pointer to the relevant tSquare.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tSquare_setFreq      (tSquare* const osc, float freq)
 @brief Set the frequency of a tSquare oscillator.
 @param osc A pointer to the relevant tSquare.
 @param freq The frequency to set the oscillator to.
 ￼￼￼
 @} */

typedef struct _tSquare {
    tMempool mempool;
    // Underlying phasor
    uint32_t phase;
    uint32_t inc;
    float freq;
    int oct;
    float w;
    float invSampleRate;
    float invSampleRateTimesTwoTo32;
    uint32_t mask;
} _tSquare;

typedef _tSquare* tSquare;

void tSquare_init(tSquare* const osc, LEAF* const leaf);
void tSquare_initToPool(tSquare* const osc, tMempool* const);
void tSquare_free(tSquare* const osc);

float tSquare_tick(tSquare* const osc);
void tSquare_setFreq(tSquare* const osc, float freq);
void tSquare_setPhase(tSquare* const osc, float phase);
void tSquare_setSampleRate(tSquare* const osc, float sr);

/*!￼￼￼
 @} */

//==============================================================================

/*!
 @defgroup tsawtooth tSawtooth
 @ingroup oscillators
 @brief Anti-aliased wavetable saw wave oscillator.
 @{

 @fn void    tSawtooth_init         (tSawtooth* const osc, LEAF* const leaf)
 @brief Initialize a tSawtooth to the default mempool of a LEAF instance.
 @param osc A pointer to the tSawtooth to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tSawtooth_initToPool   (tSawtooth* const osc, tMempool* const mempool)
 @brief Initialize a tSawtooth to a specified mempool.
 @param osc A pointer to the tSawtooth to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tSawtooth_free         (tSawtooth* const osc)
 @brief Free a tSawtooth from its mempool.
 @param osc A pointer to the tSawtooth to free.

 @fn float   tSawtooth_tick         (tSawtooth* const osc)
 @brief Tick a tSawtooth oscillator.
 @param osc A pointer to the relevant tSawtooth.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tSawtooth_setFreq      (tSawtooth* const osc, float freq)
 @brief Set the frequency of a tSawtooth oscillator.
 @param osc A pointer to the relevant tSawtooth.
 @param freq The frequency to set the oscillator to.
 ￼￼￼
 @} */

typedef struct _tSawtooth {
    tMempool mempool;
    // Underlying phasor
    uint32_t phase;
    uint32_t inc;
    float freq;
    int oct;
    float w;
    float invSampleRate;
    float invSampleRateTimesTwoTo32;
    uint32_t mask;
} _tSawtooth;

typedef _tSawtooth* tSawtooth;

void tSawtooth_init(tSawtooth* const osc, LEAF* const leaf);
void tSawtooth_initToPool(tSawtooth* const osc, tMempool* const mempool);
void tSawtooth_free(tSawtooth* const osc);

float tSawtooth_tick(tSawtooth* const osc);
void tSawtooth_setFreq(tSawtooth* const osc, float freq);
void tSawtooth_setPhase(tSawtooth* const osc, float phase);
void tSawtooth_setSampleRate(tSawtooth* const osc, float sr);

//==============================================================================

/*!
 @defgroup tpbtriangle tPBTriangle
 @ingroup oscillators
 @brief Triangle wave oscillator with polyBLEP anti-aliasing.
 @{

 @fn void   tPBTriangle_init          (tPBTriangle* const osc, LEAF* const leaf)
 @brief Initialize a tPBTriangle to the default mempool of a LEAF instance.
 @param osc A pointer to the tPBTriangle to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tPBTriangle_initToPool    (tPBTriangle* const osc, tMempool* const mempool)
 @brief Initialize a tPBTriangle to a specified mempool.
 @param osc A pointer to the tPBTriangle to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tPBTriangle_free          (tPBTriangle* const osc)
 @brief Free a tTri from its mempool.
 @param osc A pointer to the tPBTriangle to free.

 @fn float   tPBTriangle_tick          (tPBTriangle* const osc)
 @brief
 @param osc A pointer to the relevant tPBTriangle.

 @fn void    tPBTriangle_setFreq       (tPBTriangle* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tPBTriangle.

 @fn void    tPBTriangle_setSkew       (tPBTriangle* const osc, float skew)
 @brief
 @param osc A pointer to the relevant tPBTriangle.
 ￼￼￼
 @} */

typedef struct _tPBTriangle {
    tMempool mempool;
    float phase;
    float inc, freq;
    float skew;
    float lastOut;
    float invSampleRate;
} _tPBTriangle;

typedef _tPBTriangle* tPBTriangle;

void tPBTriangle_init(tPBTriangle* const osc, LEAF* const leaf);
void tPBTriangle_initToPool(tPBTriangle* const osc, tMempool* const mempool);
void tPBTriangle_free(tPBTriangle* const osc);

float tPBTriangle_tick(tPBTriangle* const osc);
void tPBTriangle_setFreq(tPBTriangle* const osc, float freq);
void tPBTriangle_setSkew(tPBTriangle* const osc, float skew);
void tPBTriangle_setSampleRate(tPBTriangle* const osc, float sr);

//==============================================================================

/*!
 @defgroup tpbpulse tPBPulse
 @ingroup oscillators
 @brief Pulse wave oscillator with polyBLEP anti-aliasing.
 @{

 @fn void    tPBPulse_init        (tPBPulse* const osc, LEAF* const leaf)
 @brief Initialize a tPBPulse to the default mempool of a LEAF instance.
 @param osc A pointer to the tPBPulse to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tPBPulse_initToPool  (tPBPulse* const osc, tMempool* const)
 @brief Initialize a tPBPulse to a specified mempool.
 @param osc A pointer to the tPBPulse to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tPBPulse_free        (tPBPulse* const osc)
 @brief Free a tPBPulse from its mempool.
 @param osc A pointer to the tPBPulse to free.

 @fn float   tPBPulse_tick        (tPBPulse* const osc)
 @brief
 @param osc A pointer to the relevant tPBPulse.

 @fn void    tPBPulse_setFreq     (tPBPulse* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tPBPulse.

 @fn void    tPBPulse_setWidth    (tPBPulse* const osc, float width)
 @brief
 @param osc A pointer to the relevant tPBPulse.
 ￼￼￼
 @} */

typedef struct _tPBPulse {
    tMempool mempool;
    float phase;
    float inc, freq;
    float width;
    float invSampleRate;
} _tPBPulse;

typedef _tPBPulse* tPBPulse;

void tPBPulse_init(tPBPulse* const osc, LEAF* const leaf);
void tPBPulse_initToPool(tPBPulse* const osc, tMempool* const);
void tPBPulse_free(tPBPulse* const osc);

float tPBPulse_tick(tPBPulse* const osc);
void tPBPulse_setFreq(tPBPulse* const osc, float freq);
void tPBPulse_setWidth(tPBPulse* const osc, float width);
void tPBPulse_setSampleRate(tPBPulse* const osc, float sr);

//==============================================================================

/*!
 @defgroup tpbsaw tPBSaw
 @ingroup oscillators
 @brief Saw wave oscillator with polyBLEP anti-aliasing.
 @{

 @fn void    tPBSaw_init          (tPBSaw* const osc, LEAF* const leaf)
 @brief Initialize a tPBSaw to the default mempool of a LEAF instance.
 @param osc A pointer to the tPBSaw to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tPBSaw_initToPool    (tPBSaw* const osc, tMempool* const mempool)
 @brief Initialize a tPBSaw to a specified mempool.
 @param osc A pointer to the tPBSaw to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tPBSaw_free          (tPBSaw* const osc)
 @brief Free a tPBSaw from its mempool.
 @param osc A pointer to the tPBSaw to free.

 @fn float   tPBSaw_tick          (tPBSaw* const osc)
 @brief
 @param osc A pointer to the relevant tPBSaw.

 @fn void    tPBSaw_setFreq       (tPBSaw* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tPBSaw.
 ￼￼￼
 @} */

typedef struct _tPBSaw {
    tMempool mempool;
    float phase;
    float inc, freq;
    float invSampleRate;
} _tPBSaw;

typedef _tPBSaw* tPBSaw;

void tPBSaw_init(tPBSaw* const osc, LEAF* const leaf);
void tPBSaw_initToPool(tPBSaw* const osc, tMempool* const mempool);
void tPBSaw_free(tPBSaw* const osc);

float tPBSaw_tick(tPBSaw* const osc);
void tPBSaw_setFreq(tPBSaw* const osc, float freq);
void tPBSaw_setSampleRate(tPBSaw* const osc, float sr);

//==============================================================================

/*!
 @defgroup tphasor tPhasor
 @ingroup oscillators
 @brief Aliasing phasor.
 @{

 @fn void    tPhasor_init        (tPhasor* const osc, LEAF* const leaf)
 @brief Initialize a tPhasor to the default mempool of a LEAF instance.
 @param osc A pointer to the tPhasor to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tPhasor_initToPool  (tPhasor* const osc, tMempool* const)
 @brief Initialize a tPhasor to a specified mempool.
 @param osc A pointer to the tPhasor to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tPhasor_free        (tPhasor* const osc)
 @brief Free a tPhasor from its mempool.
 @param osc A pointer to the tPhasor to free.

 @fn float   tPhasor_tick        (tPhasor* const osc)
 @brief
 @param osc A pointer to the relevant tPhasor.

 @fn void    tPhasor_setFreq     (tPhasor* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tPhasor.
 ￼￼￼
 @} */

typedef struct _tPhasor {
    tMempool mempool;
    float phase;
    float inc, freq;
    uint8_t phaseDidReset;
    float invSampleRate;
} _tPhasor;

typedef _tPhasor* tPhasor;

void tPhasor_init(tPhasor* const osc, LEAF* const leaf);
void tPhasor_initToPool(tPhasor* const osc, tMempool* const);
void tPhasor_free(tPhasor* const osc);

float tPhasor_tick(tPhasor* const osc);
void tPhasor_setFreq(tPhasor* const osc, float freq);
void tPhasor_setSampleRate(tPhasor* const osc, float sr);

//==============================================================================

/*!
 @defgroup tnoise tNoise
 @ingroup oscillators
 @brief Noise generator, capable of producing white or pink noise.
 @{

 @fn void    tNoise_init         (tNoise* const noise, NoiseType type, LEAF* const leaf)
 @brief Initialize a tNoise to the default mempool of a LEAF instance.
 @param noise A pointer to the tNoise to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tNoise_initToPool   (tNoise* const noise, NoiseType type, tMempool* const)
 @brief Initialize a tNoise to a specified mempool.
 @param noise A pointer to the tNoise to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tNoise_free         (tNoise* const noise)
 @brief Free a tNoise from its mempool.
 @param noise A pointer to the tNoise to free.

 @fn float   tNoise_tick         (tNoise* const noise)
 @brief
 @param noise A pointer to the relevant tNoise.
 */

/* tNoise. WhiteNoise, PinkNoise. */
/*!
 * Noise types
 */
enum NoiseType {
    WhiteNoise, //!< White noise. Full spectrum.
    PinkNoise,  //!< Pink noise. Inverse frequency-proportional spectrum.
    NoiseTypeNil,
};

/*!￼￼￼ @} */

typedef enum NoiseType NoiseType;

typedef struct _tNoise {
    tMempool mempool;
    NoiseType type;
    float pinkb0, pinkb1, pinkb2;
    float (*rand)(void);
} _tNoise;

typedef _tNoise* tNoise;

void tNoise_init(tNoise* const noise, NoiseType type, LEAF* const leaf);
void tNoise_initToPool(tNoise* const noise, NoiseType type, tMempool* const);
void tNoise_free(tNoise* const noise);

float tNoise_tick(tNoise* const noise);

//==============================================================================

/*!
 @defgroup tneuron tNeuron
 @ingroup oscillators
 @brief Model of a neuron, adapted to act as an oscillator.
 @{

 @fn void    tNeuron_init        (tNeuron* const neuron, LEAF* const leaf)
 @brief Initialize a tNeuron to the default mempool of a LEAF instance.
 @param neuron A pointer to the tNeuron to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tNeuron_initToPool  (tNeuron* const neuron, tMempool* const mempool)
 @brief Initialize a tNeuron to a specified mempool.
 @param neuron A pointer to the tNeuron to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tNeuron_free        (tNeuron* const neuron)
 @brief Free a tNeuron from its mempool.
 @param neuron A pointer to the tNeuron to free.

 @fn void    tNeuron_reset       (tNeuron* const neuron)
 @brief Reset the neuron model.
 @param neuron A pointer to the relevant tNeuron.

 @fn float   tNeuron_tick        (tNeuron* const neuron)
 @brief Tick a tNeuron oscillator.
 @param neuron A pointer to the relevant tNeuron.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tNeuron_setMode     (tNeuron* const neuron, NeuronMode mode)
 @brief Set the tNeuron shaping mode.
 @param neuron A pointer to the relevant tNeuron.
 @param mode The mode to set the tNeuron to.

 @fn void    tNeuron_setCurrent  (tNeuron* const neuron, float current)
 @brief Set the current.
 @param neuron A pointer to the relevant tNeuron.
 @param current The new current.

 @fn void    tNeuron_setK        (tNeuron* const neuron, float K)
 @brief Set the potassium value.
 @param neuron A pointer to the relevant tNeuron.
 @param K The new potassium.

 @fn void    tNeuron_setL        (tNeuron* const neuron, float L)
 @brief Set the chloride value.
 @param neuron A pointer to the relevant tNeuron.
 @param L The new chloride value.

 @fn void    tNeuron_setN        (tNeuron* const neuron, float N)
 @brief Set the sodium value.
 @param neuron A pointer to the relevant tNeuron.
 @param N The new sodium value.

 @fn void    tNeuron_setC        (tNeuron* const neuron, float C)
 @brief Set the calcium value.
 @param neuron A pointer to the relevant tNeuron.
 @param C The new calcium.

 @fn  void    tNeuron_setV1       (tNeuron* const neuron, float V1)
 @brief Set the V1 value.
 @param neuron A pointer to the relevant tNeuron.
 @param V1 The new V1.

 @fn void    tNeuron_setV2       (tNeuron* const neuron, float V2)
 @brief Set the V2 value.
 @param neuron A pointer to the relevant tNeuron.
 @param V2 The new V2.

 @fn void    tNeuron_setV3       (tNeuron* const neuron, float V3)
 @brief Set the V3 value.
 @param neuron A pointer to the relevant tNeuron.
 @param V3 The new V3.

 @fn void    tNeuron_setTimeStep (tNeuron* const neuron, float timestep)
 @brief Set the time step of the model.
 @param neuron A pointer to the relevant tNeuron.
 @param timestep The new time step.

 @brief Shaping modes for tNeuron output.
 */
enum NeuronMode {
    NeuronNormal,      //!< Normal operation
    NeuronTanh,        //!< Tanh voltage shaping
    NeuronAaltoShaper, //!< Aalto voltage shaping
    NeuronModeNil
};

/*!￼￼￼ @} */

typedef enum NeuronMode NeuronMode;

typedef struct _tNeuron {
    tMempool mempool;

    tPoleZero f;

    NeuronMode mode;

    float voltage, current;
    float timeStep;
    float invSampleRate;
    float alpha[3];
    float beta[3];
    float rate[3];
    float V[3];
    float P[3];
    float gK, gN, gL, C;
} _tNeuron;

typedef _tNeuron* tNeuron;

void tNeuron_init(tNeuron* const neuron, LEAF* const leaf);
void tNeuron_initToPool(tNeuron* const neuron, tMempool* const mempool);
void tNeuron_free(tNeuron* const neuron);

void tNeuron_reset(tNeuron* const neuron);
float tNeuron_tick(tNeuron* const neuron);
void tNeuron_setMode(tNeuron* const neuron, NeuronMode mode);
void tNeuron_setCurrent(tNeuron* const neuron, float current);
void tNeuron_setK(tNeuron* const neuron, float K);
void tNeuron_setL(tNeuron* const neuron, float L);
void tNeuron_setN(tNeuron* const neuron, float N);
void tNeuron_setC(tNeuron* const neuron, float C);
void tNeuron_setV1(tNeuron* const neuron, float V1);
void tNeuron_setV2(tNeuron* const neuron, float V2);
void tNeuron_setV3(tNeuron* const neuron, float V3);
void tNeuron_setTimeStep(tNeuron* const neuron, float timestep);
void tNeuron_setSampleRate(tNeuron* const neuron, float sr);

//==============================================================================


#define FILLEN \
    128 // was 256 in dekrispator code, but it seems like it just needs to be longer than dd step length (72) and
        // probably a power of 2.
// smaller buffer means refilling more often but a less intense load each time it refills


/*!
 @defgroup tmbpulse tMBPulse
 @ingroup oscillators
 @brief Pulse wave oscillator with minBLEP anti-aliasing.
 @{

 @fn void tMBPulse_init(tMBPulse* const osc, LEAF* const leaf)
 @brief Initialize a tMBPulse to the default mempool of a LEAF instance.
 @param osc A pointer to the tMBPulse to initialize.

 @fn void tMBPulse_initToPool(tMBPulse* const osc, tMempool* const mempool)
 @brief Initialize a tMBPulse to a specified mempool.
 @param osc A pointer to the tMBPulse to initialize.

 @fn void tMBPulse_free(tMBPulse* const osc)
 @brief Free a tMBPulse from its mempool.
 @param osc A pointer to the tMBPulse to free.

 @fn float tMBPulse_tick(tMBPulse* const osc)
 @brief
 @param osc A pointer to the relevant tMBPulse.

 @fn void tMBPulse_setFreq(tMBPulse* const osc, float f)
 @brief
 @param osc A pointer to the relevant tMBPulse.

 @fn void tMBPulse_setWidth(tMBPulse* const osc, float w)
 @brief
 @param osc A pointer to the relevant tMBPulse.

 @fn float tMBPulse_sync(tMBPulse* const osc, float sync)
 @brief
 @param osc A pointer to the relevant tMBPulse.

 @fn void tMBPulse_setSyncMode(tMBPulse* const osc, int hardOrSoft)
 @brief Set the sync behavior of the oscillator.
 @param hardOrSoft 0 for hard sync, 1 for soft sync
 ￼￼￼
 @} */

typedef struct _tMBPulse {
    tMempool mempool;
    float out;
    float freq;
    float waveform; // duty cycle, must be in [-1, 1]
    float lastsyncin;
    float sync;
    float syncdir;
    int softsync;
    float _p, _w, _b, _x, _z;
    int _j, _k;
    float _f[FILLEN + STEP_DD_PULSE_LENGTH];
    float invSampleRate;
} _tMBPulse;

typedef _tMBPulse* tMBPulse;

void tMBPulse_init(tMBPulse* const osc, LEAF* const leaf);
void tMBPulse_initToPool(tMBPulse* const osc, tMempool* const mempool);
void tMBPulse_free(tMBPulse* const osc);

float tMBPulse_tick(tMBPulse* const osc);
void tMBPulse_setFreq(tMBPulse* const osc, float f);
void tMBPulse_setWidth(tMBPulse* const osc, float w);
float tMBPulse_sync(tMBPulse* const osc, float sync);
void tMBPulse_setPhase(tMBPulse* const osc, float phase);
void tMBPulse_setSyncMode(tMBPulse* const osc, int hardOrSoft);
void tMBPulse_setBufferOffset(tMBPulse* const osc, uint32_t offset);
void tMBPulse_setSampleRate(tMBPulse* const osc, float sr);

/*!
 @defgroup tmbtriangle tMBTriangle
 @ingroup oscillators
 @brief Triangle wave oscillator with minBLEP anti-aliasing.
 @{

 @fn void tMBTriangle_init(tMBTriangle* const osc, LEAF* const leaf)
 @brief Initialize a tMBTriangle to the default mempool of a LEAF instance.
 @param osc A pointer to the tMBTriangle to initialize.

 @fn void tMBTriangle_initToPool(tMBTriangle* const osc, tMempool* const mempool)
 @brief Initialize a tMBTriangle to a specified mempool.
 @param osc A pointer to the tMBTriangle to initialize.

 @fn void tMBTriangle_free(tMBTriangle* const osc)
 @brief Free a tMBTriangle from its mempool.
 @param osc A pointer to the tMBTriangle to free.

 @fn float tMBTriangle_tick(tMBTriangle* const osc)
 @brief
 @param osc A pointer to the relevant tMBTriangle.

 @fn void tMBTriangle_setFreq(tMBTriangle* const osc, float f)
 @brief
 @param osc A pointer to the relevant tMBTriangle.

 @fn void tMBTriangle_setWidth(tMBTriangle* const osc, float w)
 @brief
 @param osc A pointer to the relevant tMBTriangle.

 @fn float tMBTriangle_sync(tMBTriangle* const osc, float sync)
 @brief
 @param osc A pointer to the relevant tMBTriangle.

 @fn void tMBTriangle_setSyncMode(tMBTriangle* const osc, int hardOrSoft)
 @brief Set the sync behavior of the oscillator.
 @param hardOrSoft 0 for hard sync, 1 for soft sync

 @} */

typedef struct _tMBTriangle {
    tMempool mempool;
    float out;
    float freq;
    float waveform; // duty cycle, must be in [-1, 1]
    float lastsyncin;
    float sync;
    float syncdir;
    int softsync;
    float _p, _w, _b, _z, quarterwaveoffset;
    int _j, _k;
    float _f[FILLEN + LONGEST_DD_PULSE_LENGTH];
    float invSampleRate;
} _tMBTriangle;

typedef _tMBTriangle* tMBTriangle;

void tMBTriangle_init(tMBTriangle* const osc, LEAF* const leaf);
void tMBTriangle_initToPool(tMBTriangle* const osc, tMempool* const mempool);
void tMBTriangle_free(tMBTriangle* const osc);

float tMBTriangle_tick(tMBTriangle* const osc);
void tMBTriangle_setFreq(tMBTriangle* const osc, float f);
void tMBTriangle_setWidth(tMBTriangle* const osc, float w);
float tMBTriangle_sync(tMBTriangle* const osc, float sync);
void tMBTriangle_setPhase(tMBTriangle* const osc, float phase);
void tMBTriangle_setSyncMode(tMBTriangle* const osc, int hardOrSoft);
void tMBTriangle_setBufferOffset(tMBTriangle* const osc, uint32_t offset);
void tMBTriangle_setSampleRate(tMBTriangle* const osc, float sr);


/*!
 @defgroup tmbsaw tMBSaw
 @ingroup oscillators
 @brief Saw wave oscillator with minBLEP anti-aliasing.
 @{

 @fn void tMBSaw_init(tMBSaw* const osc, LEAF* const leaf)
 @brief Initialize a tMBSaw to the default mempool of a LEAF instance.
 @param osc A pointer to the tMBSaw to initialize.

 @fn void tMBSaw_initToPool(tMBSaw* const osc, tMempool* const mempool)
 @brief Initialize a tMBSaw to a specified mempool.
 @param osc A pointer to the tMBSaw to initialize.

 @fn void tMBSaw_free(tMBSaw* const osc)
 @brief Free a tMBSaw from its mempool.
 @param osc A pointer to the tMBSaw to free.

 @fn float tMBSaw_tick(tMBSaw* const osc)
 @brief Tick the oscillator.
 @param osc A pointer to the relevant tMBSaw.
 @return The ticked sample.

 @fn void tMBSaw_setFreq(tMBSaw* const osc, float f)
 @brief Set the frequency of the oscillator.
 @param osc A pointer to the relevant tMBSaw.
 @param freq The new frequency.

 @fn float tMBSaw_sync(tMBSaw* const osc, float sync)
 @brief Sync this oscillator to another signal.
 @param osc A pointer to the relevant tMBSaw.
 @param sync A sample of the signal to sync to.
 @return The passed in sample.

 @fn void tMBSaw_setSyncMode(tMBSaw* const osc, int hardOrSoft)
 @brief Set the sync behavior of the oscillator.
 @param hardOrSoft 0 for hard sync, 1 for soft sync
 ￼￼￼
 @} */

typedef struct _tMBSaw {
    tMempool mempool;
    float out;
    float freq;
    float lastsyncin;
    float sync;
    float syncdir;
    int softsync;
    float _p, _w, _z;
    int _j;
    float _f[FILLEN + STEP_DD_PULSE_LENGTH];
    float invSampleRate;
} _tMBSaw;

typedef _tMBSaw* tMBSaw;

void tMBSaw_init(tMBSaw* const osc, LEAF* const leaf);
void tMBSaw_initToPool(tMBSaw* const osc, tMempool* const mempool);
void tMBSaw_free(tMBSaw* const osc);

float tMBSaw_tick(tMBSaw* const osc);
void tMBSaw_setFreq(tMBSaw* const osc, float f);
float tMBSaw_sync(tMBSaw* const osc, float sync);
void tMBSaw_setPhase(tMBSaw* const osc, float phase);
void tMBSaw_setSyncMode(tMBSaw* const osc, int hardOrSoft);
void tMBSaw_setBufferOffset(tMBSaw* const osc, uint32_t offset);
void tMBSaw_setSampleRate(tMBSaw* const osc, float sr);

//==============================================================================
/*!
 @defgroup tmbsaw tMBSawPulse
 @ingroup oscillators
 @brief Saw wave mixed with Pulse wave oscillator with minBLEP anti-aliasing.
 @{

 @fn void tMBSaw_init(tMBSaw* const osc, LEAF* const leaf)
 @brief Initialize a tMBSaw to the default mempool of a LEAF instance.
 @param osc A pointer to the tMBSaw to initialize.

 @fn void tMBSaw_initToPool(tMBSaw* const osc, tMempool* const mempool)
 @brief Initialize a tMBSaw to a specified mempool.
 @param osc A pointer to the tMBSaw to initialize.

 @fn void tMBSaw_free(tMBSaw* const osc)
 @brief Free a tMBSaw from its mempool.
 @param osc A pointer to the tMBSaw to free.

 @fn float tMBSaw_tick(tMBSaw* const osc)
 @brief Tick the oscillator.
 @param osc A pointer to the relevant tMBSaw.
 @return The ticked sample.

 @fn void tMBSaw_setFreq(tMBSaw* const osc, float f)
 @brief Set the frequency of the oscillator.
 @param osc A pointer to the relevant tMBSaw.
 @param freq The new frequency.

 @fn float tMBSaw_sync(tMBSaw* const osc, float sync)
 @brief Sync this oscillator to another signal.
 @param osc A pointer to the relevant tMBSaw.
 @param sync A sample of the signal to sync to.
 @return The passed in sample.

 @fn void tMBSaw_setSyncMode(tMBSaw* const osc, int hardOrSoft)
 @brief Set the sync behavior of the oscillator.
 @param hardOrSoft 0 for hard sync, 1 for soft sync
 ￼￼￼
 @} */

typedef struct _tMBSawPulse {
    tMempool mempool;
    float out;
    float freq;
    float lastsyncin;
    float sync;
    float syncdir;
    int softsync;
    float waveform;
    float _p, _w, _b, _x, _z, _k;
    int _j;
    float _f[FILLEN + STEP_DD_PULSE_LENGTH];
    float invSampleRate;
    float shape;
} _tMBSawPulse;

typedef _tMBSawPulse* tMBSawPulse;

void tMBSawPulse_init(tMBSawPulse* const osc, LEAF* const leaf);
void tMBSawPulse_initToPool(tMBSawPulse* const osc, tMempool* const mempool);
void tMBSawPulse_free(tMBSawPulse* const osc);


float tMBSawPulse_tick(tMBSawPulse* const osc);
float tMBSawPulse_sync(tMBSawPulse* const osc, float value);
void tMBSawPulse_setFreq(tMBSawPulse* const osc, float f);
float tMBSawPulse_sync(tMBSawPulse* const osc, float sync);
void tMBSawPulse_setPhase(tMBSawPulse* const osc, float phase);
void tMBSawPulse_setShape(tMBSawPulse* const osc, float shape);
void tMBSawPulse_setSyncMode(tMBSawPulse* const osc, int hardOrSoft);
void tMBSawPulse_setBufferOffset(tMBSawPulse* const osc, uint32_t offset);
void tMBSawPulse_setSampleRate(tMBSawPulse* const osc, float sr);

//==============================================================================
/*!
 @defgroup ttable tTable
 @ingroup oscillators
 @brief Simple aliasing wavetable oscillator.
 @{

 @fn void    tTable_init  (tTable* const osc, float* table, int size, LEAF* const leaf)
 @brief Initialize a tTable to the default mempool of a LEAF instance.
 @param osc A pointer to the tTable to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wavetable.
 @param leaf A pointer to the leaf instance.

 @fn void    tTable_initToPool   (tTable* const osc, float* table, int size, tMempool* const mempool)
 @brief Initialize a tTable to a specified mempool.
 @param osc A pointer to the tTable to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wave table.
 @param mempool A pointer to the tMempool to use.

 @fn void    tTable_free         (tTable* const osc)
 @brief Free a tTable from its mempool.
 @param osc A pointer to the tTable to free.

 @fn float   tTable_tick         (tTable* const osc)
 @brief Tick a tTable oscillator.
 @param osc A pointer to the relevant tTable.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tTable_setFreq      (tTable* const osc, float freq)
 @brief Set the frequency of a tTable oscillator.
 @param osc A pointer to the relevant tTable.
 @param freq The frequency to set the oscillator to.

 @} */

typedef struct _tTable {
    tMempool mempool;

    float* waveTable;
    int size;
    float inc, freq;
    float phase;
    float invSampleRate;
} _tTable;

typedef _tTable* tTable;

void tTable_init(tTable* const osc, float* table, int size, LEAF* const leaf);
void tTable_initToPool(tTable* const osc, float* table, int size, tMempool* const mempool);
void tTable_free(tTable* const osc);

float tTable_tick(tTable* const osc);
void tTable_setFreq(tTable* const osc, float freq);
void tTable_setSampleRate(tTable* const osc, float sr);

//==============================================================================

/*!
 @defgroup twavetable tWaveTable
 @ingroup oscillators
 @brief Anti-aliased wavetable generator.
 @{

 @fn void    tWaveTable_init  (tWaveTable* const osc, float* table, int size, float maxFreq, LEAF* const leaf)
 @brief Initialize a tWaveTable to the default mempool of a LEAF instance.
 @param osc A pointer to the tWaveTable to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wavetable.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param leaf A pointer to the leaf instance.

 @fn void    tWaveTable_initToPool   (tWaveTable* const osc, float* table, int size, float maxFreq, tMempool* const
 mempool)
 @brief Initialize a tWaveTable to a specified mempool.
 @param osc A pointer to the tWaveTable to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wave table.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param mempool A pointer to the tMempool to use.

 @fn void    tWaveTable_free         (tWaveTable* const osc)
 @brief Free a tWaveTable from its mempool.
 @param osc A pointer to the tWaveTable to free.

 @} */

#define LEAF_NUM_WAVETABLE_FILTER_PASSES 5

typedef struct _tWaveTable {
    tMempool mempool;

    float* baseTable;
    float** tables;
    int size;
    int sizeMask;
    int numTables;
    float maxFreq;
    float baseFreq, invBaseFreq;
    tButterworth bl;
    float sampleRate;
} _tWaveTable;

typedef _tWaveTable* tWaveTable;

void tWaveTable_init(tWaveTable* const osc, float* table, int size, float maxFreq, LEAF* const leaf);
void tWaveTable_initToPool(tWaveTable* const osc, float* table, int size, float maxFreq, tMempool* const mempool);
void tWaveTable_free(tWaveTable* const osc);
void tWaveTable_setSampleRate(tWaveTable* const osc, float sr);

//==============================================================================

/*!
 @defgroup twaveosc tWaveOsc
 @ingroup oscillators
 @brief Set of anti-aliased wavetable oscillators that can be faded between.
 @{

 @fn void    tWaveOsc_init(tWaveOsc* const osc, const float** tables, int n, int size, float maxFreq, LEAF* const leaf)
 @brief Initialize a tWaveOsc to the default mempool of a LEAF instance.
 @param osc A pointer to the tWaveOsc to initialize.
 @param tables An array of pointers to wavetable data.
 @param n The number of wavetables.
 @param leaf A pointer to the leaf instance.

 @fn void  tWaveOsc_initToPool(tWaveOsc* const osc, const float** tables, int n, int size, float maxFreq, tMempool*
 const mempool)
 @brief Initialize a tWaveOsc to a specified mempool.
 @param osc A pointer to the tWaveTable to initialize.
 @param tables An array of pointers to wavetable data.
 @param n The number of wavetables.
 @param size The number of samples in each of the wavetables.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param mempool A pointer to the tMempool to use.

 @fn void    tWaveOsc_free         (tWaveOsc* const osc)
 @brief Free a tWaveOsc from its mempool.
 @param osc A pointer to the tWaveOsc to free.

 @fn float   tWaveOsc_tick         (tWaveOsc* const osc)
 @brief Tick a tWaveOsc oscillator.
 @param osc A pointer to the relevant tWaveOsc.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tWaveOsc_setFreq      (tWaveOsc* const osc, float freq)
 @brief Set the frequency of a tWaveOsc oscillator.
 @param osc A pointer to the relevant tWaveOsc.
 @param freq The frequency to set the oscillator to.

 @fn void    tWaveOsc_setIndex(tWaveOsc* const osc, float index)
 @brief Set the output index of the wavetable set.
 @param index The new index from 0.0 to 1.0 as a smooth fade from the first wavetable in the set to the last.

 @} */

typedef struct _tWaveOsc {
    tMempool mempool;
    tWaveTable* tables;
    int numTables;
    float index;
    float maxFreq;
    int o1;
    int o2;
    float mix;
    uint32_t phase;
    uint32_t inc;
    float freq;
    float invSampleRateTimesTwoTo32;
    int oct;
    int size;

    // Determine base frequency
    float baseFreq;
    float invBaseFreq;
    float sampleRate;
    float w;
    float aa;
    int numSubTables;

} _tWaveOsc;

typedef _tWaveOsc* tWaveOsc;

void tWaveOsc_init(tWaveOsc* const cy, tWaveTable* tables, int numTables, LEAF* const leaf);
void tWaveOsc_initToPool(tWaveOsc* const cy, tWaveTable* tables, int numTables, tMempool* const mp);
void tWaveOsc_free(tWaveOsc* const osc);

float tWaveOsc_tick(tWaveOsc* const osc);
void tWaveOsc_setFreq(tWaveOsc* const cy, float freq);
void tWaveOsc_setAntiAliasing(tWaveOsc* const osc, float aa);
void tWaveOsc_setIndex(tWaveOsc* const osc, float index);
void tWaveOsc_setTables(tWaveOsc* const cy, tWaveTable* tables, int numTables);
void tWaveOsc_setSampleRate(tWaveOsc* const osc, float sr);

//==============================================================================

/*!
 @defgroup ttwavetables tWaveTableS
 @ingroup oscillators
 @brief A more space-efficient anti-aliased wavetable generator than tWaveTable but with slightly worse fidelity.
 @{

 @fn void    tWaveTableS_init  (tWaveTableS* const osc, float* table, int size, float maxFreq, LEAF* const leaf)
 @brief Initialize a tWaveTableS to the default mempool of a LEAF instance.
 @param osc A pointer to the tWaveTableS to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wavetable.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param leaf A pointer to the leaf instance.

 @fn void    tWaveTableS_initToPool   (tWaveTableS* const osc, float* table, int size, float maxFreq, tMempool* const
 mempool)
 @brief Initialize a tWaveTableS to a specified mempool.
 @param osc A pointer to the tWaveTableS to initialize.
 @param table A pointer to the wavetable data.
 @param size The number of samples in the wave table.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param mempool A pointer to the tMempool to use.

 @fn void    tWaveTableS_free         (tWaveTableS* const osc)
 @brief Free a tWaveTableS from its mempool.
 @param osc A pointer to the tWaveTableS to free.

 @} */

typedef struct _tWaveTableS {
    tMempool mempool;

    float* baseTable;
    float** tables;
    int numTables;
    int* sizes;
    int* sizeMasks;
    float maxFreq;
    float baseFreq, invBaseFreq;
    tButterworth bl;
    float dsBuffer[2];
    tOversampler ds;
    float sampleRate;
} _tWaveTableS;

typedef _tWaveTableS* tWaveTableS;

void tWaveTableS_init(tWaveTableS* const osc, float* table, int size, float maxFreq, LEAF* const leaf);
void tWaveTableS_initToPool(tWaveTableS* const osc, float* table, int size, float maxFreq, tMempool* const mempool);
void tWaveTableS_free(tWaveTableS* const osc);
void tWaveTableS_setSampleRate(tWaveTableS* const osc, float sr);


//==============================================================================
/*!
 @defgroup twaveoscs tWaveOscS
 @ingroup oscillators
 @brief Set of anti-aliased wavetable oscillators that can be faded between.
 @{

 @fn void    tWaveOscS_init(tWaveOsc* const osc, const float** tables, int n, int size, float maxFreq, LEAF* const leaf)
 @brief Initialize a tWaveOscS to the default mempool of a LEAF instance.
 @param osc A pointer to the tWaveOscS to initialize.
 @param tables An array of pointers to wavetable data.
 @param n The number of wavetables.
 @param leaf A pointer to the leaf instance.

 @fn void  tWaveOscS_initToPool(tWaveOsc* const osc, const float** tables, int n, int size, float maxFreq, tMempool*
 const mempool)
 @brief Initialize a tWaveOscS to a specified mempool.
 @param osc A pointer to the tWaveTableS to initialize.
 @param tables An array of pointers to wavetable data.
 @param n The number of wavetables.
 @param size The number of samples in each of the wavetables.
 @param maxFreq The maximum expected frequency of the oscillator. The higher this is, the more memory will be needed.
 @param mempool A pointer to the tMempool to use.

 @fn void    tWaveOscS_free         (tWaveOsc* const osc)
 @brief Free a tWaveOscS from its mempool.
 @param osc A pointer to the tWaveOscS to free.

 @fn float   tWaveOscS_tick         (tWaveOsc* const osc)
 @brief Tick a tWaveOscS oscillator.
 @param osc A pointer to the relevant tWaveOscS.
 @return The ticked sample as a float from -1 to 1.

 @fn void    tWaveOscS_setFreq      (tWaveOsc* const osc, float freq)
 @brief Set the frequency of a tWaveOscS oscillator.
 @param osc A pointer to the relevant tWaveOscS.
 @param freq The frequency to set the oscillator to.

 @fn void    tWaveOscS_setIndex(tWaveOsc* const osc, float index)
 @brief Set the output index of the wavetable set.
 @param index The new index from 0.0 to 1.0 as a smooth fade from the first wavetable in the set to the last.

 @} */

typedef struct _tWaveOscS {
    tMempool mempool;

    // tWaveTableS* tables;

    tWaveTableS* tables;

    int numTables;
    float index;
    float maxFreq;
    int o1;
    int o2;
    float mix;
    uint32_t phase;
    uint32_t inc;
    float freq;
    float invSampleRateTimesTwoTo32;
    int oct;
    int size;

    // Determine base frequency
    float baseFreq;
    float invBaseFreq;
    float sampleRate;
    float w;
    float aa;
    int numSubTables;

} _tWaveOscS;

typedef _tWaveOscS* tWaveOscS;

void tWaveOscS_init(tWaveOscS* const cy, tWaveTableS* tables, int numTables, LEAF* const leaf);
void tWaveOscS_initToPool(tWaveOscS* const osc, tWaveTableS* tables, int numTables, tMempool* const mempool);
void tWaveOscS_free(tWaveOscS* const osc);

float tWaveOscS_tick(tWaveOscS* const osc);
void tWaveOscS_setFreq(tWaveOscS* const osc, float freq);
void tWaveOscS_setAntiAliasing(tWaveOscS* const osc, float aa);
void tWaveOscS_setIndex(tWaveOscS* const osc, float index);
//    void    tWaveOscS_setIndexTable(tWaveOscS* const osc, int i, float* table, int size);
void tWaveOscS_setSampleRate(tWaveOscS* const osc, float sr);


//==============================================================================

/*!
 @defgroup tIntphasor tIntPhasor
 @ingroup oscillators
 @brief Aliasing phasor.
 @{

 @fn void    tIntPhasor_init        (tIntPhasor* const osc, LEAF* const leaf)
 @brief Initialize a tIntPhasor to the default mempool of a LEAF instance.
 @param osc A pointer to the tIntPhasor to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tIntPhasor_initToPool  (tIntPhasor* const osc, tMempool* const)
 @brief Initialize a tIntPhasor to a specified mempool.
 @param osc A pointer to the tIntPhasor to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tIntPhasor_free        (tIntPhasor* const osc)
 @brief Free a tIntPhasor from its mempool.
 @param osc A pointer to the tIntPhasor to free.

 @fn float   tIntPhasor_tick        (tIntPhasor* const osc)
 @brief
 @param osc A pointer to the relevant tIntPhasor.

 @fn void    tIntPhasor_setFreq     (tIntPhasor* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tIntPhasor.
 ￼￼￼
 @} */

typedef struct _tIntPhasor {
    tMempool mempool;
    uint32_t phase;
    uint32_t inc;
    float freq;
    int32_t mask;
    uint8_t phaseDidReset;
    float invSampleRateTimesTwoTo32;
} _tIntPhasor;

typedef _tIntPhasor* tIntPhasor;

void tIntPhasor_init(tIntPhasor* const osc, LEAF* const leaf);
void tIntPhasor_initToPool(tIntPhasor* const osc, tMempool* const);
void tIntPhasor_free(tIntPhasor* const osc);

float tIntPhasor_tick(tIntPhasor* const osc);
void tIntPhasor_setFreq(tIntPhasor* const osc, float freq);
void tIntPhasor_setSampleRate(tIntPhasor* const osc, float sr);

void tIntPhasor_setPhase(tIntPhasor* const cy, float phase);

//==============================================================================

/*!
 @defgroup tSquareLFO tSquareLFO
 @ingroup oscillators
 @brief Aliasing phasor.
 @{

 @fn void    tSquareLFO_init        (tSquareLFO* const osc, LEAF* const leaf)
 @brief Initialize a tSquareLFO to the default mempool of a LEAF instance.
 @param osc A pointer to the tSquareLFO to initialize.
 @param leaf A pointer to the leaf instance.

 @fn void    tSquareLFO_initToPool  (tSquareLFO* const osc, tMempool* const)
 @brief Initialize a tSquareLFO to a specified mempool.
 @param osc A pointer to the tSquareLFO to initialize.
 @param mempool A pointer to the tMempool to use.

 @fn void    tSquareLFO_free        (tSquareLFO* const osc)
 @brief Free a tSquareLFO from its mempool.
 @param osc A pointer to the tSquareLFO to free.

 @fn float   tSquareLFO_tick        (tSquareLFO* const osc)
 @brief
 @param osc A pointer to the relevant tSquareLFO.

 @fn void    tSquareLFO_setFreq     (tSquareLFO* const osc, float freq)
 @brief
 @param osc A pointer to the relevant tSquareLFO.
 ￼￼￼
 @} */

typedef struct _tSquareLFO {
    tMempool mempool;
    float pulsewidth;
    tIntPhasor phasor;
    tIntPhasor invPhasor;
} _tSquareLFO;

typedef _tSquareLFO* tSquareLFO;

void tSquareLFO_init(tSquareLFO* const osc, LEAF* const leaf);
void tSquareLFO_initToPool(tSquareLFO* const osc, tMempool* const);
void tSquareLFO_free(tSquareLFO* const osc);

float tSquareLFO_tick(tSquareLFO* const osc);
void tSquareLFO_setFreq(tSquareLFO* const osc, float freq);
void tSquareLFO_setSampleRate(tSquareLFO* const osc, float sr);
void tSquareLFO_setPulseWidth(tSquareLFO* const cy, float pw);
void tSquareLFO_setPhase(tSquareLFO* const cy, float phase);

typedef struct _tSawSquareLFO {
    tMempool mempool;
    float shape;
    tIntPhasor saw;
    tSquareLFO square;
} _tSawSquareLFO;

typedef _tSawSquareLFO* tSawSquareLFO;

void tSawSquareLFO_init(tSawSquareLFO* const osc, LEAF* const leaf);
void tSawSquareLFO_initToPool(tSawSquareLFO* const osc, tMempool* const);
void tSawSquareLFO_free(tSawSquareLFO* const osc);

float tSawSquareLFO_tick(tSawSquareLFO* const osc);
void tSawSquareLFO_setFreq(tSawSquareLFO* const osc, float freq);
void tSawSquareLFO_setSampleRate(tSawSquareLFO* const osc, float sr);
void tSawSquareLFO_setPhase(tSawSquareLFO* const cy, float phase);
void tSawSquareLFO_setShape(tSawSquareLFO* const cy, float shape);

//==============================================================================
/*!
    @defgroup tTriLFO tTriLFO
    @ingroup oscillators
    @brief Aliasing phasor.
    @{

    @fn void    tTriLFO_init        (tTriLFO* const osc, LEAF* const leaf)
    @brief Initialize a tTriLFO to the default mempool of a LEAF instance.
    @param osc A pointer to the tTriLFO to initialize.
    @param leaf A pointer to the leaf instance.

    @fn void    tTriLFO_initToPool  (tTriLFO* const osc, tMempool* const)
    @brief Initialize a tTriLFO to a specified mempool.
    @param osc A pointer to the tTriLFO to initialize.
    @param mempool A pointer to the tMempool to use.

    @fn void    tTriLFO_free        (tTriLFO* const osc)
    @brief Free a tTriLFO from its mempool.
    @param osc A pointer to the tTriLFO to free.

    @fn float   tTriLFO_tick        (tTriLFO* const osc)
    @brief
    @param osc A pointer to the relevant tTriLFO.

    @fn void    tTriLFO_setFreq     (tTriLFO* const osc, float freq)
    @brief
    @param osc A pointer to the relevant tTriLFO.
    ￼￼￼
    @} */

typedef struct _tTriLFO {
    tMempool mempool;
    int32_t phase;
    int32_t inc;
    float freq;
    float invSampleRate;
    float invSampleRateTimesTwoTo32;
} _tTriLFO;

typedef _tTriLFO* tTriLFO;

void tTriLFO_init(tTriLFO* const osc, LEAF* const leaf);
void tTriLFO_initToPool(tTriLFO* const osc, tMempool* const);
void tTriLFO_free(tTriLFO* const osc);

float tTriLFO_tick(tTriLFO* const osc);
void tTriLFO_setFreq(tTriLFO* const osc, float freq);
void tTriLFO_setSampleRate(tTriLFO* const osc, float sr);

void tTriLFO_setPhase(tTriLFO* const cy, float phase);

typedef struct _tSineTriLFO {
    tMempool mempool;
    float shape;
    tTriLFO tri;
    tCycle sine;
} _tSineTriLFO;

typedef _tSineTriLFO* tSineTriLFO;

void tSineTriLFO_init(tSineTriLFO* const osc, LEAF* const leaf);
void tSineTriLFO_initToPool(tSineTriLFO* const osc, tMempool* const);
void tSineTriLFO_free(tSineTriLFO* const osc);

float tSineTriLFO_tick(tSineTriLFO* const osc);
void tSineTriLFO_setFreq(tSineTriLFO* const osc, float freq);
void tSineTriLFO_setSampleRate(tSineTriLFO* const osc, float sr);
void tSineTriLFO_setPhase(tSineTriLFO* const cy, float phase);
void tSineTriLFO_setShape(tSineTriLFO* const cy, float shape);
#ifdef __cplusplus
}
#endif

#endif // LEAF_OSCILLATORS_H_INCLUDED

//==============================================================================
