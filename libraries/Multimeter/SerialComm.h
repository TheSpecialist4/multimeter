

///
/// @file    SerialComm.h
/// @brief    application-specific wrapper/abstraction of the screen
/// @details  multimeter screen as object manipulating the liquidcrystal object
/// @n
/// @n @b   Project Energia MT 0101E0016
///
/// @author   Team 37
/// @author   Charles Cunningham
///
/// @date   May 14, 2017 15:53
///


#ifndef SerialComm_h
#define SerialComm_h

// Include application, user and local libraries
#include <Energia.h>
#include "sample_defn.h"

#define SERIAL()    Serial1

class SerialComm
{
  private:
    uint8_t connected;
    uint8_t state;

  public:
    ///
    /// @brief      Define the serial communication object
    ///
    SerialComm(uint8_t channel);

    ///
    /// @brief
    ///
    void begin(long baud_rate);

    void sendSample(Typed_Sample_t sample);

    void 
};

#endif
