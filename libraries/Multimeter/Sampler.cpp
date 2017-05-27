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
                const uint8_t ampmeter_pin, const uint8_t neg_pin, const uint8_t peizo_pin) :
  adc(),
  lowGainPin(low_gain_pin),
  midGainPin(mid_gain_pin),
  highGainPin(high_gain_pin),
  smallResistorPin(small_resistor_pin),
  bigResistorPin(big_resistor_pin),
  ohmmeterPin(ohmmeter_pin),
  ampmeterPin(ampmeter_pin),
  negPin(neg_pin),
  peizoPin(peizo_pin),
  ampState(LOW_GAIN)
{
}

void Sampler::begin(uint8_t sample_mode)
{
  pinMode(peizoPin, OUTPUT);
  pinMode(negPin, INPUT_PULLDOWN);
  
  pinMode(lowGainPin, OUTPUT);
  pinMode(midGainPin, OUTPUT);
  pinMode(highGainPin, OUTPUT);

  pinMode(smallResistorPin, OUTPUT);
  pinMode(bigResistorPin, OUTPUT);
  pinMode(ohmmeterPin, OUTPUT);
  pinMode(ampmeterPin, OUTPUT);

  sampleMode = sample_mode;
  setSampleMode(sampleMode);

  configureADC();
}

void Sampler::configureADC()
{
  adc.begin();
  //adc.rate080sps();
  adc.pgaDisable();
}

void Sampler::setSampleMode(uint8_t sample_mode)
{
  sampleMode = sample_mode;
  switch(sampleMode) {
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      setAmpState(HIGH_GAIN);
      setOhmeterState(OHMETER_OFF);
      digitalWrite(ohmmeterPin, LOW);
      digitalWrite(ampmeterPin, HIGH);
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      setAmpState(HIGH_GAIN);
      setOhmeterState(OHMETER_OFF);
      digitalWrite(ohmmeterPin, LOW);
      digitalWrite(ampmeterPin, LOW);
      break;
    case RESISTANCE:
      setAmpState(HIGH_GAIN);
      setOhmeterState(SMALL_RESISTOR);
      digitalWrite(ohmmeterPin, HIGH);
      digitalWrite(ampmeterPin, HIGH);
      break;
    case CONTINUITY:
      break;
    case LOGIC:
      break;
  }
}

uint8_t Sampler::getSampleMode()
{
  return sampleMode;
}

void Sampler::incrementSampleMode()
{
  setSampleMode((getSampleMode() + 1) % 7);
}

float Sampler::getADCmV()
{
  uint32_t val = adc.readRawADC();
  Serial.println(val);
  return (3.3/(float)16777216)*val;
}

TypedSample_t Sampler::sample()
{
  TypedSample_t s;
  s.type = sampleMode;
  switch (sampleMode) {
    case DC_VOLTAGE:
      s.sample.floatRep = getDCVoltage();
      break;
    case AC_VOLTAGE:
      s.sample.floatRep = getACVoltage();
      break;
    case DC_CURRENT:
      s.sample.floatRep = getDCCurrent();
      break;
    case AC_CURRENT:
      s.sample.floatRep = getACCurrent();
      break;
    case RESISTANCE:
      s.sample.floatRep = getResistance();
      break;
    case CONTINUITY:
      s.sample.floatRep = getContinuity();
      break;
    case LOGIC:
      s.sample.floatRep = getLogic();
      break;
  }
  return s;
}

float Sampler::getDCVoltage() {
  return getADCmV();
  if (sign() == 1) {  // positive
    switch (ampState) {
      case LOW_GAIN:
        return (0.9645 * getADCmV()) + 174.06;
      case MID_GAIN:
        return (0.2069 * getADCmV()) + 0.04;
      case HIGH_GAIN:
        return (0.0846 * getADCmV()) + 0.1046;
    }
    return 0.0;
  } else {  // negative
    switch (ampState) {
      case LOW_GAIN:
        return (-0.9522 * getADCmV()) + 180.76;
      case MID_GAIN:
        return (getADCmV());
      case HIGH_GAIN:
        return getADCmV();
    }
    return 0.0;
  }
}

float Sampler::getACVoltage() {
  switch (ampState) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return 0.0;
}

float Sampler::getDCCurrent() {
  switch (ampState) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return 0.0;
}

float Sampler::getACCurrent() {
  switch (ampState) {
    case LOW_GAIN:
      return (0.9645 * getADCmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * getADCmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * getADCmV()) + 0.1046;
  }
  return 0.0;
}

float Sampler::getResistance() {
  float val;
  switch(ohmeterState) {
    case OHMETER_OFF:
      return -1.0;
    case SMALL_RESISTOR:
      return (0.7174 * getADCmV()) + 210.86;
    case BIG_RESISTOR:
      val = getADCmV();
      return (-0.0004 * val * val) + (0.9803 * val) + 190.32;
  }
}

float Sampler::getContinuity() {
  return -1.0;
}

float Sampler::getLogic() {
  return 0;
}

int Sampler::sign() {
  if (digitalRead(negPin) == LOW)
  {
    return 1;
  }
  return -1;
}

void Sampler::setAmpState(uint8_t state) {
  ampState = state;
  switch(ampState) {
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
    default:
      digitalWrite(lowGainPin, HIGH);
      digitalWrite(midGainPin, HIGH);
      digitalWrite(highGainPin, LOW);
      break;
  }
}

void Sampler::setOhmeterState(uint8_t state) {
  ohmeterState = state;
  switch(ohmeterState) {
    case OHMETER_OFF:
      digitalWrite(smallResistorPin, HIGH);
      digitalWrite(bigResistorPin, HIGH);
      break;
    case SMALL_RESISTOR:
      digitalWrite(smallResistorPin, LOW);
      digitalWrite(bigResistorPin, HIGH);
      break;
    case BIG_RESISTOR:
      digitalWrite(smallResistorPin, HIGH);
      digitalWrite(bigResistorPin, LOW);
      break;
  }
}