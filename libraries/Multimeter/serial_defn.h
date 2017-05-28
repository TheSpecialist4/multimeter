
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

// instruction struct
typedef struct Instruction_t {
    uint8_t type;
    uint16_t value;
};

#endif