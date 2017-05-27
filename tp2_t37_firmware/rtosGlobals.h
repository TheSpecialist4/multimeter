
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

const ScreenPins_t screen_pins = {.rs = 5,
                                  .en = 6,
                                  .d4 = 25,
                                  .d5 = 26,
                                  .d6 = 27,
                                  .d7 = 28,
                                  .brightnessPWM = 19};

const SamplerPins_t sampler_pins = {.gain_low_pin = 15,
                                    .gain_mid_pin = 18,
                                    .gain_high_pin = 17,
                                    .resistor_1_pin = 13,
                                    .resistor_2_pin = 14,
                                    .mirror_switch_pin = 0,
                                    .switch_1_pin = 0,
                                    .neg_pin = 29,
                                    .peizo_pin = 37};

const MiscPins_t misc_pins = {.sample_led_pin = GREEN_LED, // change to 5
                              .status_led_pin = RED_LED,
                              .logger_cs_pin = 18};


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
