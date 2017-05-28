

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
#include "Mailbox.h"
#include "serial_defn.h"

class SerialComm
{
  private:
    enum RxStates {no_rx, type_received, value_receiving};

    bool connected;
    RxStates rxState;

    Mailbox<Instruction_t> instructionsReceived;

    Instruction_t currentInstruction;

  public:
    ///
    /// @brief      Define the serial communication object
    ///
    SerialComm();

    ///
    /// @brief
    ///
    void begin();

    void receiveByte(uint8_t byte);

    Instruction_t receivedInstruction();

    bool instructionAvailable();
};

#endif
