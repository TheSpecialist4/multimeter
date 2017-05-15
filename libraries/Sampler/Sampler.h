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

    uint8_t lowRangePin;
    uint8_t midRangePin;
    uint8_t highRangePin;

    uint8_t negPin;
    uint8_t ledPin;
    uint8_t peizoPin;

    uint8_t sampleMode;
    uint8_t ampState;

    void setAmpState(uint8_t range);
    
    float getVoltage();
    float getCurrent();
    float getResistance();
    float getContinuity();
    float getLogic();

    int sign();

  public:
    ///
    /// @brief      Define the sampler
    ///
    Sampler(uint8_t low_pin, uint8_t mid_pin, uint8_t high_pin, uint8_t neg_pin, uint8_t peizo_pin, uint8_t led_pin);

    ///
    /// @brief      Create the event
    ///
    void begin(uint8_t sample_mode);

    void setSampleMode(uint8_t mode);

    Typed_Sample_t sample();
};

#endif
