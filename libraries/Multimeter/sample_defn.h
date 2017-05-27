
#ifndef sample_defn_h
#define sample_defn_h

// sample rate definitions
#define HALF_SEC  0
#define ONE_SEC   1
#define TWO_SEC   2
#define FIVE_SEC  3
#define TEN_SEC   4
#define ONE_MIN   5
#define TWO_MIN   6
#define FIVE_MIN  7
#define TEN_MIN   8

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

typedef struct TypedSample_t {
  Sample_t value;
  uint8_t type;
  uint8_t resolution;
};

#endif
