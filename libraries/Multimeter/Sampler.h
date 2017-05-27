///
/// @file    MultimeterScreen.h
/// @brief    application-specific wrapper/abstraction of the screen
/// @details  multimeter screen as object manipulating the liquidcrystal object
/// @n
/// @n @b   Project Energia MT 0101E0016
///
/// @author   Team 37
/// @author   Charles Cunningham
///
/// @date   May 14, 2017 15:53
///


#ifndef Sampler_h
#define Sampler_h

// Include application, user and local libraries
#include <NAU7802.h>
#include "sample_defn.h"

#define LOW_GAIN   0
#define MID_GAIN   1
#define HIGH_GAIN  2

#define OHMETER_OFF     0
#define SMALL_RESISTOR  1
#define BIG_RESISTOR    2

class Sampler
{
  private:
    NAU7802 adc;

    const uint8_t lowGainPin;
    const uint8_t midGainPin;
    const uint8_t highGainPin;

    const uint8_t smallResistorPin;
    const uint8_t bigResistorPin;

    const uint8_t ohmmeterPin;
    const uint8_t ampmeterPin;

    const uint8_t negPin;
    const uint8_t peizoPin;

    uint8_t sampleMode;
    uint8_t ampState;
    uint8_t ohmeterState;

    void configureADC();

    void setAmpState(uint8_t state);
    void setOhmeterState(uint8_t state);

    float getADCmV();
    
    float getDCVoltage();
    float getACVoltage();
    float getDCCurrent();
    float getACCurrent();
    float getResistance();
    float getContinuity();
    float getLogic();

    int sign();

  public:
    ///
    /// @brief      Define the sampler
    ///
    Sampler(const uint8_t low_gain_pin, const uint8_t mid_gain_pin, const uint8_t high_gain_pin,
            const uint8_t small_resistor_pin, const uint8_t big_resistor_pin, const uint8_t ohmmeter_pin,
            const uint8_t ampmeter_pin, const uint8_t neg_pin, const uint8_t peizo_pin);

    ///
    /// @brief      Create the event
    ///
    void begin(uint8_t sample_mode);

    uint8_t getSampleMode();

    void setSampleMode(uint8_t mode);
    
    void incrementSampleMode();

    TypedSample_t sample();
};

#endif
