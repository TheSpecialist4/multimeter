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


#ifndef MultimeterScreen_h
#define MultimeterScreen_h


// Include application, user and local libraries
#include <LiquidCrystal.h>
#include "sample_defn.h"

#define OHM         ((uint8_t) 5)
#define PLUS_MINUS  ((uint8_t) 6)

class MultimeterScreen
{
  private:
    LiquidCrystal lcd;
        
    uint8_t brightness0[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b10000,
      0b10000
    };
    
    uint8_t brightness1[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b01000,
      0b11000,
      0b11000
    };
    
    uint8_t brightness2[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00100,
      0b01100,
      0b11100,
      0b11100
    };
    
    uint8_t brightness3[8] = {
      0b00000,
      0b00000,
      0b00000,
      0b00010,
      0b00110,
      0b01110,
      0b11110,
      0b11110
    };
    
    uint8_t brightness4[8] = {
      0b00000,
      0b00000,
      0b00001,
      0b00011,
      0b00111,
      0b01111,
      0b11111,
      0b11111
    };
    
    uint8_t ohm[8] = {
      0b00000,
      0b01110,
      0b10001,
      0b10001,
      0b10001,
      0b01010,
      0b11011,
      0b00000
    };
    
    uint8_t plusMinus[8] = {
      0b00100,
      0b00100,
      0b11111,
      0b00100,
      0b00100,
      0b00000,
      0b11111,
      0b00000
    };

    const uint8_t brightnessPWMPin;
    char strBuffer[15];

    uint8_t brightness;

    uint8_t nextBrightness(uint8_t current_brightness);

    void displayVoltage(TypedSample_t sample);
    void displayCurrent(TypedSample_t sample);
    void displayResistance(TypedSample_t sample);
    void displayContinuity(TypedSample_t sample);
    void displayLogic(TypedSample_t sample);

  public:
    ///
    /// @brief      Define the screen
    ///
    MultimeterScreen(const uint8_t rs, const uint8_t en, const uint8_t d4, const uint8_t d5,
                      const uint8_t d6, const uint8_t d7, const uint8_t pwmPin);

    ///
    /// @brief      Create the event
    ///
    void begin(uint8_t default_brightness);

    void displaySample(TypedSample_t sample);

    void displaySampleMode(uint8_t sample_mode);

    void displaySampleRate(uint8_t sample_period);

    void displayBrightness(uint8_t new_brightness);

    void incrementBrightness();

    void setLoggingConfigMode(uint8_t sample_mode, uint8_t sample_period);
};

#endif
