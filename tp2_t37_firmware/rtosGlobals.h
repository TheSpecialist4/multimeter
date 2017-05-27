
///
/// @file    rtosGlobals.h
/// @brief    Header
/// @details  Global variables for Energia MT project
/// @n  
/// @n @b   Project MultiBlink_eX
/// @n @a   Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author   Rei Vilo
/// @author   http://embeddedcomputing.weebly.com
///
/// @date   Jun 05, 2015 13:34
/// @version  101
/// 
/// @copyright  (c) Rei Vilo, 2015
/// @copyright  CC = BY SA NC
///
/// @see    ReadMe.txt for references
///



#ifndef rtosGlobals_h
#define rtosGlobals_h

// Core library
#include "Energia.h"

// Include application, user and local libraries
#include "Semaphore.h"
#include "Event.h"
#include "Mailbox.h"
#include "sample_defn.h"
#include "serial_defn.h"
#include "Multimeter.h"

// starting default configs
#define DEFAULT_SAMPLE_MODE   DC_VOLTAGE
#define DEFAULT_SAMPLE_PERIOD HALF_SEC
#define DEFAULT_BRIGHTNESS    2
#define MODALITY              BIOS_NO_WAIT

// semaphore for debug serial
Semaphore serialSemaphore;

const ScreenPins_t screen_pins = {.rs = 11,
                                  .en = 12,
                                  .d4 = 13,
                                  .d5 = 34,
                                  .d6 = 33,
                                  .d7 = 32,
                                  .brightnessPWM = 31};

const SamplerPins_t sampler_pins = {.gain_low_pin = 25,
                                    .gain_mid_pin = 26,
                                    .gain_high_pin = 27,
                                    .small_resistor_pin = 23,
                                    .big_resistor_pin = 24,
                                    .ohmmeter_pin = 8,
                                    .ampmeter_pin = 30,
                                    .neg_pin = 29};

const MiscPins_t misc_pins = {.sample_led_pin = GREEN_LED, // 5
                              .status_led_pin = RED_LED,  // 6
                              .logger_cs_pin = 18,
                              .buzzer_pin = 35};


Multimeter multimeter(screen_pins, sampler_pins, misc_pins);

// Add optional rtosSetup function
void rtosSetup()
{
    Serial.begin(115200);
    serialSemaphore.begin(1);
    
    serialSemaphore.waitFor();
    Serial.println("rtosSetup");
    serialSemaphore.post();

    multimeter.begin(DEFAULT_SAMPLE_MODE, DEFAULT_SAMPLE_PERIOD, DEFAULT_BRIGHTNESS);
    
    serialSemaphore.waitFor();
    Serial.println("rtosSetup done");
    serialSemaphore.post();
}

#endif
