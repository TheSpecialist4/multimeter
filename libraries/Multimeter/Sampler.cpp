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

  configureADC();
}

void Sampler::configureADC()
{
  adc.begin();
  //adc.pgaDisable();
  adc.rate320sps();
}

void Sampler::disableAllModes()
{
  setAmplifierState(MIN_GAIN);
  setOhmmeterState(OHMMETER_OFF);
  setAmpmeterState(AMPMETER_OFF);
}

float Sampler::getADCmV()
{
  uint32_t val = adc.readRawADC();
  return (3.3/(float)16777216)*val;
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
  sample.resolution = ampState;
  sample.value.floatRep = getADCmV();
  return sample;
  setAmplifierState(LOW_GAIN);

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcDCVoltage(ampState);

    switch(ampState) {
      case LOW_GAIN:
        if (sample.value.floatRep < 4.75) {
          setAmplifierState(MID_GAIN);
        } else if (sample.value.floatRep > 12) {
          sample.value.floatRep = NAN;
        }
        break;
      case MID_GAIN:
        if (sample.value.floatRep > 5) {
          setAmplifierState(LOW_GAIN);
        } else if (sample.value.floatRep < 0.9) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 1) {
          setAmplifierState(MID_GAIN);
        }
        break;
    }
  } while((last_amp_state != ampState));

  sample.resolution = ampState;
  disableAllModes();
  return sample;
}

TypedSample_t Sampler::getACVoltage()
{
  uint8_t last_amp_state;
  TypedSample_t sample;
  setAmplifierState(LOW_GAIN);

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcACVoltage(ampState);
    
    switch(ampState) {
      case LOW_GAIN:
        if (sample.value.floatRep < 4.75) {
          setAmplifierState(MID_GAIN);
        } else if (sample.value.floatRep > 12) {
          sample.value.floatRep = NAN;
        }
        break;
      case MID_GAIN:
        if (sample.value.floatRep > 5) {
          setAmplifierState(LOW_GAIN);
        } else if (sample.value.floatRep < 0.9) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 1) {
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

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcDCCurrent(ampState);
    
    switch(ampState) {
      case MID_GAIN:
        if (sample.value.floatRep > 200) {
          sample.value.floatRep = NAN;
        } else if (sample.value.floatRep < 9.5) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 10) {
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

  do {
    last_amp_state = ampState;
    sample.value.floatRep = calcACCurrent(ampState);
    
    switch(ampState) {
      case MID_GAIN:
        if (sample.value.floatRep > 200) {
          sample.value.floatRep = NAN;
        } else if (sample.value.floatRep < 9.5) {
          setAmplifierState(HIGH_GAIN);
        }
        break;
      case HIGH_GAIN:
        if (sample.value.floatRep > 10) {
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
    sample.value.floatRep = calcResistance(ohmmeterState);
    
    switch(ohmmeterState) {
      case BIG_RESISTOR:
        if (sample.value.floatRep < 950) {
          setOhmmeterState(SMALL_RESISTOR);
        }
        break;
      case SMALL_RESISTOR:
        if (sample.value.floatRep > 1000000) {
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
    return 1;
  }
  return -1;
}

float Sampler::calcDCVoltage(uint8_t amp_state) {
  if (sign() == 1) {  // positive
    switch (amp_state) {
      case HIGH_GAIN:
        return (0.9645 * getADCmV()) + 174.06;
      case MID_GAIN:
        return (0.2069 * getADCmV()) + 0.04;
      case LOW_GAIN:
        return (0.0846 * getADCmV()) + 0.1046;
    }
  } else {  // negative
    switch (amp_state) {
      case HIGH_GAIN:
        return (-0.9522 * getADCmV()) + 180.76;
      case MID_GAIN:
        return (getADCmV());
      case LOW_GAIN:
        return getADCmV();
    }
  }
  return -1.0;
}

float Sampler::calcACVoltage(uint8_t amp_state) {
  switch (amp_state) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return -1.0;
}

float Sampler::calcDCCurrent(uint8_t amp_state) {
  switch (amp_state) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return -1.0;
}

float Sampler::calcACCurrent(uint8_t amp_state) {
  switch (amp_state) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return -1.0;
}

float Sampler::calcResistance(uint8_t ohmeter_state) {
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
    case LOW_GAIN:
      digitalWrite(lowGainPin, LOW);
      digitalWrite(midGainPin, HIGH);
      digitalWrite(highGainPin, HIGH);
      break;
    case MID_GAIN:
      digitalWrite(lowGainPin, HIGH);
      digitalWrite(midGainPin, LOW);
      digitalWrite(highGainPin, HIGH);
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