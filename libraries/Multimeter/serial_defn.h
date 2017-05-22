
#ifndef serial_defn_h
#define serial_defn_h

// instruction types
#define BRIGHTNESS      0
#define SAMPLE_MODE     1
#define SAMPLE_RATE     2
#define LOGGING_MODE    3
#define LOGGING_RATE    4
#define LOGGING_SIZE    5
#define LOGGING_TOGGLE  6
#define SAMPLE          7

// instruction utility macros
#define INSTR_BYTE(type, value) ( ((type) << 5) & ((value) & 0x1f) )
#define INSTR_MODE(instr_byte)  ( ((instr_byte) >> 5) & 0x03 )
#define INSTR_VALUE(instr_byte) ( (instr_byte) & 0x1f )

// instruction struct
typedef struct Instruction_t {
    uint8_t type;
    uint16_t value;
};

#endif