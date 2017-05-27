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

#include "Multimeter.h"

// Include application, user and local libraries
#include "rtosGlobals.h"

// Prototypes
unsigned long lastTime = 0;
unsigned long currentTime = 0;

// Add setup code
void sampling_setup()
{
  
}

// Add loop code
void sampling_loop()
{
  currentTime = millis();
  
  if (currentTime - lastTime >= multimeter.getSamplePeriodMillis())
  {
    lastTime = currentTime;
    multimeter.sample();
  }
  
  delay(1);
}
