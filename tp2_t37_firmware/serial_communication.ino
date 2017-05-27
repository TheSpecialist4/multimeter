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
#include <stdio.h>

// Include application, user and local libraries
#include "rtosGlobals.h"
#include "serial_defn.h"

// Prototypes

// Define variables and constants
TypedSample_t tx_sample;
char buffer1[20];
char buffer2[5];
uint8_t stopByte = 0;
uint8_t rxbyte = 0;

// Add setup code
void setup_serial_communication()
{
  Serial1.begin(9600);
}

// Add loop code
void loop_serial_communication()
{
  if (multimeter.samplesAvailable()) {
    tx_sample = multimeter.dequeueSample();
//    Serial1.write((tx_sample.sample.byteRep) & 0xff);
//    Serial1.write((tx_sample.sample.byteRep >> 8) & 0xff);
//    Serial1.write((tx_sample.sample.byteRep >> 16) & 0xff);
//    Serial1.write((tx_sample.sample.byteRep >> 24) & 0xff);
//    Serial1.write((byte) 0x00);
    //Serial.println(tx_sample.value.floatRep);
    //Serial.println("sample");
  }
  
  if (Serial1.available() > 0) {
    serialSemaphore.waitFor();
    Serial.println("rxed");
    serialSemaphore.post();
    rxbyte = Serial1.read();
    Serial1.write(rxbyte);
  }
}
