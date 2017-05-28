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

#define DEBOUNCE_TIME 100

#define BUTTON_1  40  //PUSH1
#define BUTTON_2  39  //PUSH2
#define BUTTON_3  38
#define BUTTON_4  37

// Define variables and constants

// debouncing variables
volatile unsigned long push1Time;
volatile unsigned long push1LastTime = 0;
volatile unsigned long push2Time;
volatile unsigned long push2LastTime = 0;
volatile unsigned long push3Time;
volatile unsigned long push3LastTime = 0;
volatile unsigned long push4Time;
volatile unsigned long push4LastTime = 0;

Event buttonPressEvent;

TypedSample_t sample;

bool alive;

// the setup routine runs once when you press reset:
void setup()
{
  rtosSetup();
  buttonPressEvent.begin();

  pinMode(BUTTON_1, INPUT);
  attachInterrupt(BUTTON_1, functionPUSH1, FALLING);

  pinMode(BUTTON_2, INPUT);
  attachInterrupt(BUTTON_2, functionPUSH2, FALLING);

  pinMode(BUTTON_3, INPUT);
  attachInterrupt(BUTTON_3, functionPUSH3, FALLING);

  pinMode(BUTTON_4, INPUT);
  attachInterrupt(BUTTON_4, functionPUSH4, FALLING);

  alive = true;
}

// the loop routine runs over and over again forever:
void loop()
{
  uint32_t events = buttonPressEvent.waitFor(Event_Id_NONE, BUTTON_1_PRESS
                    + BUTTON_2_PRESS
                    + BUTTON_3_PRESS
                    + BUTTON_4_PRESS
                    + BUTTON_5_PRESS);

  
  multimeter.buttonPressed(events);

  delay(1);
}

void functionPUSH1()
{
  push1Time = millis();
  if (push1Time - push1LastTime > DEBOUNCE_TIME)
  {
    buttonPressEvent.send(BUTTON_1_PRESS);
  }
  push1LastTime = push1Time;
}

void functionPUSH2()
{
  push2Time = millis();
  if (push2Time - push2LastTime > DEBOUNCE_TIME)
  {
    buttonPressEvent.send(BUTTON_2_PRESS);
  }
  push2LastTime = push2Time;
}

void functionPUSH3()
{
  push3Time = millis();
  if (push3Time - push3LastTime > DEBOUNCE_TIME)
  {
    buttonPressEvent.send(BUTTON_3_PRESS);
  }
  push3LastTime = push3Time;
}

void functionPUSH4()
{
  push4Time = millis();
  if (push4Time - push4LastTime > DEBOUNCE_TIME)
  {
    buttonPressEvent.send(BUTTON_4_PRESS);
  }
  push4LastTime = push4Time;
}


