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
#include "SerialComm.h"
#include <stdio.h>

SerialComm::SerialComm() :
    connected(false),
    rxState(no_rx)
{
    ;
}

void SerialComm::begin()
{
    instructionsReceived.begin(10);
    currentInstruction.type = 0xFF;
    currentInstruction.value = 0;
}

void SerialComm::receiveByte(uint8_t byte)
{
    switch(rxState) {
        case no_rx:
            currentInstruction.type = byte;
            rxState = type_received;
            break;
        case type_received:
            currentInstruction.value = byte;
            currentInstruction.value <<= 8;
            rxState = value_receiving;
            break;
        case value_receiving:
            currentInstruction.value |= byte;
            instructionsReceived.post(currentInstruction);
            rxState = no_rx;
            break;
    }
}

Instruction_t SerialComm::receivedInstruction()
{
    Instruction_t instruction;
    instructionsReceived.waitFor(instruction);
    return instruction;
}

bool SerialComm::instructionAvailable()
{
    return instructionsReceived.available() > 0;
}