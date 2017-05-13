///
/// @file    EMT_event.ino
/// @brief    Main sketch
///
/// @n @a   Developed with [embedXcode+](http://embedXcode.weebly.com)
///


// Core library for code-sense - IDE-based
#include "Energia.h"

// Include application, user and local libraries
#include "rtosGlobals.h"

// Prototypes


// Define variables and constants
Typed_Sample_t sample;

// the setup routine runs once when you press reset:
void setup()
{
  rtosSetup();
}

// the loop routine runs over and over again forever:
void loop()
{
  sampleMailbox.waitFor(sample);
  serialSemaphore.waitFor();
  Serial.println(sample.sample.floatRep);
  serialSemaphore.post();
}

