
#ifndef sample_defn_h
#define sample_defn_h

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

// sample mode state definitions
#define DC_VOLTAGE  0
#define AC_VOLTAGE  1
#define DC_CURRENT  2
#define AC_CURRENT  3
#define RESISTANCE  4
#define CONTINUITY  5
#define LOGIC       6

typedef union Sample_t {
  uint32_t byteRep;
  float floatRep;
};

typedef struct Typed_Sample_t {
  Sample_t sample;
  uint8_t type;
};

#endif
