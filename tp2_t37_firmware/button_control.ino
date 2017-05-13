///
/// @file    button_control.ino
/// @brief    Handle button press events
///
/// @n @a   Developed with [embedXcode+](http://embedXcode.weebly.com)
///


// Core library for code-sense - IDE-based
#include "Energia.h"

// Include application, user and local libraries
#include "rtosGlobals.h"

// Prototypes
// Define variables and constants
Event buttonPressEvent;

// the setup routine runs once when you press reset:
void button_control_setup()
{
    buttonPressEvent.begin();

    // Solution 2: Energia interrupt
    pinMode(PUSH1, INPUT_PULLUP);
    attachInterrupt(PUSH1, functionPUSH1, FALLING);
    
    // Solution 2: Energia interrupt
    pinMode(PUSH2, INPUT_PULLUP);
    attachInterrupt(PUSH2, functionPUSH2, FALLING);
}

// the loop routine runs over and over again forever:
void button_control_loop()
{
  uint32_t events = buttonPressEvent.waitFor(Event_Id_NONE, SAMPLE_MODE_BUTTON_PRESS
                                                          + SAMPLE_RATE_BUTTON_PRESS
                                                          + BRIGHTNESS_BUTTON_PRESS
                                                          + LOGGING_BUTTON_PRESS );

  switch(events) {
    case SAMPLE_MODE_BUTTON_PRESS:
      serialSemaphore.waitFor();
      Serial.println("sample mode press");
      serialSemaphore.post();
      break;
    case SAMPLE_RATE_BUTTON_PRESS:
      serialSemaphore.waitFor();
      Serial.println("sample rate press");
      serialSemaphore.post();
      break;
    case BRIGHTNESS_BUTTON_PRESS:
      break;
    case LOGGING_BUTTON_PRESS:
      break;
  }
}

void functionPUSH1()
{
    buttonPressEvent.send(SAMPLE_MODE_BUTTON_PRESS);
}

void functionPUSH2()
{
    buttonPressEvent.send(SAMPLE_RATE_BUTTON_PRESS);
}

