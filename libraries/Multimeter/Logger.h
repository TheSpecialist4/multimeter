///
/// @file    MultimeterScreen.h
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


#ifndef Logger_h
#define Logger_h


// Include application, user and local libraries
#include <Energia.h>
#include <SD.h>
#include "sample_defn.h"

class Logger
{
  private:
    uint8_t fileNumber;
    File currentFile;
    const uint8_t csPin;
    char writeBuffer[12];
    uint8_t logMode;

  public:
    ///
    /// @brief      Define the logger
    ///
    Logger(uint8_t cs_pin);

    ///
    /// @brief      Create the event
    ///
    void begin(uint8_t default_mode);

    bool newLogFile(uint8_t log_mode);

    bool writeSample(TypedSample_t sample);
};

#endif
