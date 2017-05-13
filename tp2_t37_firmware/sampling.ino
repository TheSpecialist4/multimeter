///
/// @mainpage  EMT-ClockLibrary
///
/// @details  Clock Library for Energia MT
/// @n
/// @n
/// @n @a   Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author   Rei Vilo
/// @author   http://embeddedcomputing.weebly.com
/// @date   18/06/2015 09:49
/// @version  <#version#>
///
/// @copyright  (c) Rei Vilo, 2015
/// @copyright  CC = BY SA NC
///
/// @see    ReadMe.txt for references
///


///
/// @file   EMT_ClockLibrary.ino
/// @brief    Main sketch
///
/// @details  <#details#>
/// @n @a   Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author   Rei Vilo
/// @author   http://embeddedcomputing.weebly.com
/// @date   18/06/2015 09:49
/// @version  <#version#>
///
/// @copyright  (c) Rei Vilo, 2015
/// @copyright  CC = BY SA NC
///
/// @see    ReadMe.txt for references
/// @n
/// @example    ReadMe.txt
///


// Core library for code-sense - IDE-based
#include "Energia.h"

#include <NAU7802.h>

// Include application, user and local libraries
#include "rtosGlobals.h"

// Prototypes

// Define variables and constants
NAU7802 adc;

unsigned long samplePeriod = HALF_SEC;
unsigned long lastTime = 0;
unsigned long currentTime = 0;

uint8_t sampleMode = DC_VOLTAGE;

Typed_Sample_t newSample;

// Add setup code
void setup()
{
    adc.begin();
}

// Add loop code
void loop()
{
    currentTime = millis();
    if (currentTime - lastTime >= samplePeriod)
    {
        lastTime = currentTime;
        sample.type = sampleMode;
        switch(sampleMode) {
          case DC_VOLTAGE:
          case AC_VOLTAGE:
            sample.sample.floatRep = 1.0;
            sampleMailbox.post(sample, BIOS_NO_WAIT);
            break;
          case DC_CURRENT:
          case AC_CURRENT:
            break;
          case RESISTANCE:
            break;
          case CONTINUITY:
            break;
          case LOGIC:
            break;
        }
        
    }
}

