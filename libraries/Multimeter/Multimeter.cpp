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

Multimeter::Multimeter(ScreenPins_t screen_pins, SamplerPins_t sampler_pins, MiscPins_t misc_pins) :
    screen(screen_pins.rs, screen_pins.en,
            screen_pins.d4, screen_pins.d5,
            screen_pins.d6, screen_pins.d7,
            screen_pins.brightnessPWM),
    sampler(sampler_pins.gain_low_pin, sampler_pins.gain_mid_pin,
            sampler_pins.gain_high_pin, sampler_pins.small_resistor_pin,
            sampler_pins.big_resistor_pin, sampler_pins.ohmmeter_pin,
            sampler_pins.ampmeter_pin, sampler_pins.neg_pin),
    sampleLedPin(misc_pins.sample_led_pin),
    statusLedPin(misc_pins.status_led_pin),
    buzzerPin(misc_pins.buzzer_pin)
//    logger(misc_pins.logger_cs_pin)
{
}

void Multimeter::begin(uint8_t default_sample_mode, uint8_t default_sample_period, uint8_t default_brightness)
{
    pinMode(sampleLedPin, OUTPUT);
    pinMode(statusLedPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    samplerSemaphore.begin();
    screenSemaphore.begin();
    loggerSemaphore.begin();

    sampleMode = default_sample_mode;
    samplePeriod = default_sample_period;

    screenSemaphore.waitFor();
    screen.begin(default_brightness);
    screen.displaySampleMode(sampleMode);
    screen.displaySampleRate(samplePeriod);
    screenSemaphore.post();

    samplerSemaphore.waitFor();
    sampler.begin();
    samplerSemaphore.post();

    serialTxMailbox.begin(10);
}

void Multimeter::buttonPressed(uint32_t button_event)
{
    switch(loggingState) {
        case NOT_LOGGING:
            switch(button_event) {
                case SAMPLE_MODE_BUTTON_PRESS:
                    //sampleMode = nextSampleMode(sampleMode);
                    samplerSemaphore.waitFor();
                    sampler.incrAmp();
                    samplerSemaphore.post();
                    screenSemaphore.waitFor();
                    screen.displaySampleMode(sampleMode);
                    screenSemaphore.post();
                    break;
                case SAMPLE_RATE_BUTTON_PRESS:
                    samplePeriod = nextSamplePeriod(samplePeriod);

                    
                    screenSemaphore.waitFor();
                    screen.displaySampleRate(samplePeriod);
                    screenSemaphore.post();
                    break;
                case BRIGHTNESS_BUTTON_PRESS:
                    screenSemaphore.waitFor();
                    screen.incrementBrightness();
                    screenSemaphore.post();
                    break;
                case LOGGING_BUTTON_PRESS:
                    loggingState = LOGGING_INPUT;
                    screenSemaphore.waitFor();
                    screen.setLoggingConfigMode(sampleMode, samplePeriod);
                    screenSemaphore.post();
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
                    loggingState = LOGGING_ACTIVE;
                    break;
            }
            break;
        case LOGGING_ACTIVE:
            switch(button_event) {
                case LOGGING_BUTTON_PRESS:
                    loggingState = NOT_LOGGING;
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
            sampleMode = instruction.value;
            break;
        case SAMPLE_RATE:
            samplePeriod = instruction.value;
            break;
        case BRIGHTNESS:
            screenSemaphore.waitFor();
            screen.displayBrightness(instruction.value);
            screenSemaphore.post();
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
    digitalWrite(statusLedPin, HIGH);
    digitalWrite(sampleLedPin, HIGH);
    samplerSemaphore.waitFor();
    TypedSample_t new_sample = sampler.sample(sampleMode);
    samplerSemaphore.post();

    screenSemaphore.waitFor();
    screen.displaySample(new_sample);
    screenSemaphore.post();
    if (serialTxMailbox.available() < 10) {
        serialTxMailbox.post(new_sample, BIOS_NO_WAIT);
    }
    digitalWrite(statusLedPin, LOW);
    digitalWrite(sampleLedPin, LOW);
}

uint8_t Multimeter::nextSamplePeriod(uint8_t current_period)
{
    return (current_period + 1) % 9;
}

uint8_t Multimeter::nextSampleMode(uint8_t current_mode)
{
    return (current_mode + 1) % 7;
}

unsigned long Multimeter::getSamplePeriodMillis()
{
    return sample_period_millis[samplePeriod];
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