///
/// @file    MultimeterS.h
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

// Include application, user and local libraries
#include <Energia.h>
#include "Multimeter.h"
#include "sample_defn.h"

// normal operation button press event definitions
#define SAMPLE_MODE_BUTTON_PRESS  BUTTON_1_PRESS
#define SAMPLE_RATE_BUTTON_PRESS  BUTTON_2_PRESS
#define BRIGHTNESS_BUTTON_PRESS   BUTTON_3_PRESS
#define LOGGING_BUTTON_PRESS      BUTTON_4_PRESS

// logging input button press event definitions
#define PLUS_100_INPUT  BUTTON_1_PRESS
#define PLUS_10_INPUT   BUTTON_2_PRESS
#define PLUS_1_INPUT    BUTTON_3_PRESS

#define NOT_LOGGING     0
#define LOGGING_INPUT   1
#define LOGGING_ACTIVE  2

Multimeter::Multimeter(ScreenPins_t screen_pins, SamplerPins_t sampler_pins) :
    screen(screen_pins.rs, screen_pins.en,
            screen_pins.d4, screen_pins.d5,
            screen_pins.d6, screen_pins.d7,
            screen_pins.brightnessPWM),
    sampler(sampler_pins.low_pin, sampler_pins.mid_pin,
            sampler_pins.high_pin, sampler_pins.neg_pin,
            sampler_pins.peizo_pin, sampler_pins.led_pin)
{
}

void Multimeter::begin(uint8_t default_sample_mode, uint8_t default_sample_period, uint8_t default_brightness)
{
    screen.begin(default_brightness);
    sampler.begin(default_sample_mode);
    sample_period = default_sample_period;
    serialTxMailbox.begin(10);
}

void Multimeter::buttonPressed(uint32_t button_event)
{
    switch(logging_state) {
        case NOT_LOGGING:
            switch(button_event) {
                case SAMPLE_MODE_BUTTON_PRESS:
                    sampler.incrementSampleMode();
                    screen.displaySampleMode(sampler.getSampleMode());
                    break;
                case SAMPLE_RATE_BUTTON_PRESS:
                    sample_period = nextSamplePeriod(sample_period);
                    screen.displaySampleRate(sample_period);
                    break;
                case BRIGHTNESS_BUTTON_PRESS:
                    screen.incrementBrightness();
                    break;
                case LOGGING_BUTTON_PRESS:
                    logging_state = LOGGING_INPUT;
                    screen.setLoggingConfigMode(sampler.getSampleMode(), sample_period);
                    break;
            }
            break;
        case LOGGING_INPUT:
            switch(button_event) {
                case PLUS_100_INPUT:
                    break;
                case PLUS_10_INPUT:
                    break;
                case PLUS_1_INPUT:
                    break;
                case LOGGING_BUTTON_PRESS:
                    logging_state = LOGGING_ACTIVE;
                    break;
            }
            break;
        case LOGGING_ACTIVE:
            switch(button_event) {
                case LOGGING_BUTTON_PRESS:
                    logging_state = NOT_LOGGING;
                    break;
            }
            break;
    }
}

void Multimeter::instructionReceived(Instruction_t instruction)
{
    switch(instruction.type)
    {
        case SAMPLE_MODE:
            sampler.setSampleMode(instruction.value);
            break;
        case SAMPLE_RATE:
            sample_period = instruction.value;
            break;
        case BRIGHTNESS:
            screen.displayBrightness(instruction.value);
            break;
        case LOGGING_MODE:
            break;
        case LOGGING_RATE:
            break;
        case LOGGING_SIZE:
            break;
        case LOGGING_TOGGLE:
            break;
    }
}

void Multimeter::sample()
{
    TypedSample_t new_sample = sampler.sample();
    screen.displaySample(new_sample);
    if (serialTxMailbox.available() < 10) {
        serialTxMailbox.post(new_sample, BIOS_NO_WAIT);
    }
}

uint8_t Multimeter::nextSamplePeriod(uint8_t current_period)
{
    return (current_period + 1) % 9;
}

unsigned long Multimeter::getSamplePeriodMillis()
{
    return sample_period_millis[sample_period];
}

bool Multimeter::samplesAvailable()
{
    return serialTxMailbox.available() > 0;
}

TypedSample_t Multimeter::dequeueSample()
{
    TypedSample_t sample;
    serialTxMailbox.waitFor(sample);
    return sample;
}