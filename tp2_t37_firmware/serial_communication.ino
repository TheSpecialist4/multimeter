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
#include "SerialComm.h"

// Prototypes

// Define variables and constants
TypedSample_t tx_sample;
SerialComm serial_comms;

// Add setup code
void setup_serial_communication()
{
  serial_comms.begin();
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
  }

  if (currentTime - lastTime > 1000) {
    Serial1.write(0xFF);
  }
  
  if (Serial1.available() > 0) {
    serial_comms.receiveByte(Serial1.read());
    if (serial_comms.instructionAvailable()) {
      multimeter.instructionReceived(serial_comms.receivedInstruction());
    }
  }
}
