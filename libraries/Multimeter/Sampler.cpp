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

// Include application, user and local libraries
#include "Sampler.h"
#include <Energia.h>

Sampler::Sampler(const uint8_t low_gain_pin, const uint8_t mid_gain_pin, const uint8_t high_gain_pin,
                const uint8_t small_resistor_pin, const uint8_t big_resistor_pin, const uint8_t ohmmeter_pin,
                const uint8_t ampmeter_pin, const uint8_t neg_pin) :
  adc(),
  lowGainPin(low_gain_pin),
  midGainPin(mid_gain_pin),
  highGainPin(high_gain_pin),
  smallResistorPin(small_resistor_pin),
  bigResistorPin(big_resistor_pin),
  ohmmeterPin(ohmmeter_pin),
  ampmeterPin(ampmeter_pin),
  negPin(neg_pin),
  ampState(MIN_GAIN)
{
}

void Sampler::begin()
{
  pinMode(negPin, INPUT_PULLUP);
  pinMode(lowGainPin, OUTPUT);
  pinMode(midGainPin, OUTPUT);
  pinMode(highGainPin, OUTPUT);
  setAmplifierState(MIN_GAIN);

  pinMode(smallResistorPin, OUTPUT);
  pinMode(bigResistorPin, OUTPUT);
  pinMode(ohmmeterPin, OUTPUT);
  setOhmmeterState(OHMMETER_OFF);

  pinMode(ampmeterPin, OUTPUT);
  setAmpmeterState(AMPMETER_OFF);
  pinMode(GREEN_LED, OUTPUT);

  configureADC();
}

void Sampler::configureADC()
{
  adc.begin();
  adc.pgaDisable();
  adc.rate080sps();
}

void Sampler::disableAllModes()
{
  setAmplifierState(MIN_GAIN);
  setOhmmeterState(OHMMETER_OFF);
  setAmpmeterState(AMPMETER_OFF);
}

void Sampler::incrAmp()
{
  setAmplifierState((ampState+1)%4);
}

float Sampler::getADCmV()
{
  uint32_t val = adc.readRawADC();
  return (5000.0/(float)16777216)*val;
}

TypedSample_t Sampler::sample(uint8_t type)
{
  TypedSample_t sample;
  sample.value.floatRep = NAN;
  sample.type = type;
  
  switch (type) {
    case DC_VOLTAGE:
      sample = getDCVoltage();
      break;
    case AC_VOLTAGE:
      sample = getACVoltage();
      break;
    case DC_CURRENT:
      sample = getDCCurrent();
      break;
    case AC_CURRENT:
      sample = getACCurrent();
      break;
    case RESISTANCE:
      sample = getResistance();
      break;
    case CONTINUITY:
      sample = getContinuity();
      break;
    case LOGIC:
      sample = getLogic();
      break;
  }

  return sample;
}

TypedSample_t Sampler::getDCVoltage()
{
  uint8_t last_amp_state;
  TypedSample_t sample;
  //setAmplifierState(LOW_GAIN);  // 5-12V range

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcDCVoltage(getADCmV(), ampState);

    // switch(ampState) {
    //   case LOW_GAIN:
    //     if (sample.value.floatRep < 4750.0) {
    //       setAmplifierState(MID_GAIN);
    //     } else if (sample.value.floatRep > 12000.0) {
    //       sample.value.floatRep = NAN;
    //     }
    //     break;
    //   case MID_GAIN:
    //     if (sample.value.floatRep > 5000.0) {
    //       setAmplifierState(LOW_GAIN);
    //     } else if (sample.value.floatRep < 900.0) {
    //       setAmplifierState(HIGH_GAIN);
    //     }
    //     break;
    //   case HIGH_GAIN:
    //     if (sample.value.floatRep > 1000.0) {
    //       setAmplifierState(MID_GAIN);
    //     }
    //     break;
    // }
  } while((last_amp_state != ampState));

  sample.resolution = ampState;
  //disableAllModes();
  return sample;
}

TypedSample_t Sampler::getACVoltage()
{
  uint8_t last_amp_state;
  TypedSample_t sample;
  setAmplifierState(LOW_GAIN);

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcACVoltage(getADCmV(), ampState);
    switch(ampState) {
      case LOW_GAIN:
        if (sample.value.floatRep < 4750.0) {
          setAmplifierState(MID_GAIN);
        } else if (sample.value.floatRep > 12000.0) {
          sample.value.floatRep = NAN;
        }
        break;
      case MID_GAIN:
        if (sample.value.floatRep > 5000.0) {
          setAmplifierState(LOW_GAIN);
        } else if (sample.value.floatRep < 900.0) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 1000.0) {
          setAmplifierState(MID_GAIN);
        }
        break;
    }
  } while((last_amp_state != ampState));

  sample.resolution = ampState;
  disableAllModes();
  return sample;
}

TypedSample_t Sampler::getDCCurrent()
{
  uint8_t last_amp_state;
  TypedSample_t sample;
  setAmplifierState(MID_GAIN);
  setAmpmeterState(AMPMETER_ON);

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcDCCurrent(getADCmV(), ampState);
    
    switch(ampState) {
      case MID_GAIN:
        if (sample.value.floatRep > 200.0) {
          sample.value.floatRep = NAN;
        } else if (sample.value.floatRep < 9.5) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 10.0) {
          setAmplifierState(MID_GAIN);
        }
        break;
    }
  } while((last_amp_state != ampState));

  sample.resolution = ampState;
  disableAllModes();
  return sample;
}

TypedSample_t Sampler::getACCurrent()
{
  uint8_t last_amp_state;
  TypedSample_t sample;
  setAmplifierState(MID_GAIN);
  setAmpmeterState(AMPMETER_ON);

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcACCurrent(getADCmV(), ampState);
    
    switch(ampState) {
      case MID_GAIN:
        if (sample.value.floatRep > 200.0) {
          sample.value.floatRep = NAN;
        } else if (sample.value.floatRep < 9.5) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 10.0) {
          setAmplifierState(MID_GAIN);
        }
        break;
    }
  } while((last_amp_state != ampState));

  sample.resolution = ampState;
  disableAllModes();
  return sample;
}

TypedSample_t Sampler::getResistance()
{
  uint8_t last_ohmmeter_state;
  TypedSample_t sample;
  setAmplifierState(HIGH_GAIN);
  setOhmmeterState(BIG_RESISTOR);

  do {
    last_ohmmeter_state = ohmmeterState;
    sample.value.floatRep = calcResistance(getADCmV(), ohmmeterState);
    
    switch(ohmmeterState) {
      case BIG_RESISTOR:
        if (sample.value.floatRep < 950.0) {
          setOhmmeterState(SMALL_RESISTOR);
        }
        break;
      case SMALL_RESISTOR:
        if (sample.value.floatRep > 1000000.0) {
          setOhmmeterState(BIG_RESISTOR);
        }
        break;
    }
  } while((last_ohmmeter_state != ohmmeterState));

  sample.resolution = ohmmeterState;
  disableAllModes();
  return sample;
}

TypedSample_t Sampler::getContinuity() {
  return getResistance();
}

TypedSample_t Sampler::getLogic() {
  TypedSample_t sample;
  sample.type = LOGIC;
  sample.value.floatRep = -1.0;
  return sample;
}

int Sampler::sign() {
  if (digitalRead(negPin) == HIGH)
  {
    digitalWrite(GREEN_LED, HIGH);
    return 1;
  }
  digitalWrite(GREEN_LED, LOW);
  return -1;
}

float Sampler::calcDCVoltage(float adc_mv_val, uint8_t amp_state) {
  sign();
  if (true){//sign() == 1) {  // positive
    switch (amp_state) {
      case HIGH_GAIN:
        return (1.0996 * getADCmV()) + 57.437;
      case MID_GAIN:
        return (5.1149 * getADCmV()) - 147.51;
      case LOW_GAIN:
        return (14.308 * getADCmV()) - 342.76;
    }
  } else {  // negative
    switch (amp_state) {
      case HIGH_GAIN:
        return (-1.1236 * getADCmV()) - 12.567;
      case MID_GAIN:
        return (-5.1293 * getADCmV()) + 56.735;
      case LOW_GAIN:
        return (-13.955 * getADCmV()) - 59.686;
    }
  }
  return -1.0;
}

float Sampler::calcACVoltage(float adc_mv_val, uint8_t amp_state) {
  switch (amp_state) {
    case LOW_GAIN:
      return (1.0996 * getADCmV()) + 57.437;
    case MID_GAIN:
      return (5.1149 * getADCmV()) - 147.51;
    case HIGH_GAIN:
      return (14.308 * getADCmV()) - 342.76;
  }
  return -1.0;
}

float Sampler::calcDCCurrent(float adc_mv_val, uint8_t amp_state) {
  switch (amp_state) {
    case HIGH_GAIN:
      return (42.826 * getADCmV()) + 291.0;
    case MID_GAIN:
      return (8.0032 * getADCmV()) + 88.936;
  }
  return -1.0;
}

float Sampler::calcACCurrent(float adc_mv_val, uint8_t amp_state) {
  switch (amp_state) {
    case HIGH_GAIN:
      return (42.826 * getADCmV()) + 291.0;
    case MID_GAIN:
      return (8.0032 * getADCmV()) + 88.936;
  }
  return -1.0;
}

float Sampler::calcResistance(float adc_mv_val, uint8_t ohmeter_state) {
  float val;
  switch(ohmeter_state) {
    case OHMMETER_OFF:
      return -1.0;
    case SMALL_RESISTOR:
      return (0.7174 * getADCmV()) + 210.86;
    case BIG_RESISTOR:
      val = getADCmV();
      return (-0.0004 * val * val) + (0.9803 * val) + 190.32;
  }
  return -1.0;
}

void Sampler::setAmplifierState(uint8_t new_state) {
  ampState = new_state;
  switch(ampState) {
    case MIN_GAIN:
      digitalWrite(lowGainPin, HIGH);
      digitalWrite(midGainPin, HIGH);
      digitalWrite(highGainPin, HIGH);
      break;
    case LOW_GAIN:
      digitalWrite(midGainPin, HIGH);
      digitalWrite(highGainPin, HIGH);
      digitalWrite(lowGainPin, LOW);
      break;
    case MID_GAIN:
      digitalWrite(lowGainPin, HIGH);
      digitalWrite(highGainPin, HIGH);
      digitalWrite(midGainPin, LOW);
      break;
    case HIGH_GAIN:
      digitalWrite(lowGainPin, HIGH);
      digitalWrite(midGainPin, HIGH);
      digitalWrite(highGainPin, LOW);
      break;
  }
}

void Sampler::setOhmmeterState(uint8_t new_state) {
  ohmmeterState = new_state;
  switch(ohmmeterState) {
    case OHMMETER_OFF:
      digitalWrite(ohmmeterPin, LOW);
      digitalWrite(smallResistorPin, HIGH);
      digitalWrite(bigResistorPin, HIGH);
      break;
    case SMALL_RESISTOR:
      digitalWrite(ohmmeterPin, HIGH);
      digitalWrite(smallResistorPin, LOW);
      digitalWrite(bigResistorPin, HIGH);
      break;
    case BIG_RESISTOR:
      digitalWrite(ohmmeterPin, HIGH);
      digitalWrite(smallResistorPin, HIGH);
      digitalWrite(bigResistorPin, LOW);
      break;
  }
}

void Sampler::setAmpmeterState(uint8_t new_state) 
{
  switch(new_state) {
    case AMPMETER_OFF:
      digitalWrite(ampmeterPin, HIGH);
      break;
    default:
      digitalWrite(ampmeterPin, LOW);
      break;
  }
}