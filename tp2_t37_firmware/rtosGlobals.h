
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
#include <Semaphore.h>
#include <Event.h>
#include <Mailbox.h>
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

const SamplerPins_t sampler_pins = {.low_pin = 40,
                                    .mid_pin = 39,
                                    .high_pin = 38,
                                    .neg_pin = 8,
                                    .peizo_pin = 37,
                                    .led_pin = GREEN_LED};

const uint8_t logger_cs_pin = 18;

Multimeter multimeter(screen_pins, sampler_pins, logger_cs_pin);

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
