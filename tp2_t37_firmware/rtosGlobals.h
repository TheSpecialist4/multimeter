
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

// sample mode state definitions
#define DC_VOLTAGE  0
#define AC_VOLTAGE  1
#define DC_CURRENT  2
#define AC_CURRENT  3
#define RESISTANCE  4
#define CONTINUITY  5
#define LOGIC       6

// sample rate definitions
#define HALF_SEC  500
#define ONE_SEC   1000
#define TWO_SEC   2000
#define FIVE_SEC  5000
#define TEN_SEC   10000
#define ONE_MIN   60000
#define TWO_MIN   120000
#define FIVE_MIN  300000
#define TEN_MIN   600000

// instruction types
#define BRIGHTNESS      0
#define SAMPLE_MODE     1
#define SAMPLE_RATE     2
#define LOGGING_MODE    3
#define LOGGING_RATE    4
#define LOGGING_SIZE    5
#define LOGGING_TOGGLE  6
#define SAMPLE          7

typedef union Sample_t {
  uint32_t byteRep;
  float floatRep;
};

typedef struct Typed_Sample_t {
  Sample_t sample;
  uint8_t type;
};

// instruction utility macros
#define INSTR_BYTE(type, value) ( ((type) << 5) & ((value) & 0x1f) )
#define INSTR_MODE(instr_byte)  ( ((instr_byte) >> 5) & 0x03 )
#define INSTR_VALUE(instr_byte) ( (instr_byte) & 0x1f )

// normal operation button press event definitions
#define SAMPLE_MODE_BUTTON_PRESS  Event_Id_00
#define SAMPLE_RATE_BUTTON_PRESS  Event_Id_01
#define BRIGHTNESS_BUTTON_PRESS   Event_Id_02
#define LOGGING_BUTTON_PRESS      Event_Id_03

// logging input button press event definitions
#define PLUS_100_INPUT  Event_Id_00
#define PLUS_10_INPUT   Event_Id_01
#define PLUS_1_INPUT    Event_Id_02

// sample config change event definitions 
#define SAMPLE_MODE_CHANGE  Event_Id_00
#define SAMPLE_RATE_CHANGE  Event_Id_01

///
/// @brief  event for button presses
///
Semaphore serialSemaphore;
Event sampleConfigChange;
Mailbox<Typed_Sample_t> sampleMailbox;

// Add optional rtosSetup function
void rtosSetup()
{
    Serial.begin(115200);
    serialSemaphore.begin(1);
    
    serialSemaphore.waitFor();
    Serial.println("rtosSetup");
    serialSemaphore.post();
    
    sampleConfigChange.begin();
    sampleMailbox.begin(5);
}

#endif
