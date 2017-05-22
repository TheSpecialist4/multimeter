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

#define LOW_RANGE   0
#define MID_RANGE   1
#define HIGH_RANGE  2

class Sampler
{
  private:
    NAU7802 adc;

    const uint8_t lowRangePin;
    const uint8_t midRangePin;
    const uint8_t highRangePin;

    const uint8_t negPin;
    const uint8_t ledPin;
    const uint8_t peizoPin;

    uint8_t sampleMode;
    uint8_t ampState;

    void setAmpState(uint8_t range);
    
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
    Sampler(const uint8_t low_pin, const uint8_t mid_pin, const uint8_t high_pin,
            const uint8_t neg_pin, const uint8_t peizo_pin, const uint8_t led_pin);

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
