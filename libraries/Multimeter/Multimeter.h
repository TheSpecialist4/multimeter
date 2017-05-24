///
/// @file    Multimeter.h
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


#ifndef Multimeter_h
#define Multimeter_h

// Include application, user and local libraries
#include "Energia.h"
#include "Semaphore.h"
#include "Mailbox.h"

#include "Sampler.h"
#include "MultimeterScreen.h"
#include "Logger.h"

#include "sample_defn.h"
#include "serial_defn.h"

// generic button press event definitions
#define BUTTON_1_PRESS  Event_Id_00
#define BUTTON_2_PRESS  Event_Id_01
#define BUTTON_3_PRESS  Event_Id_02
#define BUTTON_4_PRESS  Event_Id_03

typedef struct ScreenPins_t {
    uint8_t rs;
    uint8_t en;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
    uint8_t brightnessPWM;
};

typedef struct SamplerPins_t {
    uint8_t low_pin;
    uint8_t mid_pin;
    uint8_t high_pin;
    uint8_t neg_pin;
    uint8_t peizo_pin;
    uint8_t led_pin;
};

class Multimeter
{
  private:
    Sampler sampler;
    MultimeterScreen screen;
    Logger logger;

    Semaphore samplerSemaphore;
    Semaphore screenSemaphore;
    Semaphore periodSemaphore;

    Mailbox<TypedSample_t> serialTxMailbox;

    const unsigned long sample_period_millis[9] = {500, 1000, 2000, 5000,
                                10000, 60000, 120000, 300000, 600000};

    uint8_t sample_period;
    uint8_t logging_state;

    uint8_t nextSamplePeriod(uint8_t current_period);

  public:
    ///
    /// @brief      Define the sampler
    ///
    Multimeter(const ScreenPins_t screen_pins, const SamplerPins_t sampler_pins, const uint8_t logger_cs_pin);

    ///
    /// @brief      Create the event
    ///
    void begin(uint8_t sample_mode, uint8_t sample_period, uint8_t brightness);

    void buttonPressed(uint32_t button_event);

    void instructionReceived(Instruction_t instruction);

    void sample();

    unsigned long getSamplePeriodMillis();

    bool samplesAvailable();

    TypedSample_t dequeueSample();
};

#endif
