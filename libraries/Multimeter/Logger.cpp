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
#include "Logger.h"
#include <stdio.h>

Logger::Logger(uint8_t cs_pin) :
    fileNumber(0),
    currentFile(),
    csPin(cs_pin)
{
    ;
}

void Logger::begin(uint8_t default_mode)
{
    logMode = default_mode;
    pinMode(csPin, OUTPUT);
}

bool Logger::newLogFile(uint8_t log_mode)
{
    if (!SD.begin(csPin)) {
        return false;
    }

    sprintf(writeBuffer, "%i.txt");
    currentFile = SD.open(writeBuffer, FILE_WRITE);

    if (currentFile) {
        return false;
    }

    logMode = log_mode;

    return true;
}

bool Logger::writeSample(TypedSample_t sample)
{
    if (currentFile) {
        return false;
    }

    return true;
}