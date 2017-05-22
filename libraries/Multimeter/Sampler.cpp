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

Sampler::Sampler(const uint8_t low_pin, const uint8_t mid_pin, const uint8_t high_pin,
                  const uint8_t neg_pin, const uint8_t peizo_pin, const uint8_t led_pin) :
  adc(),
  lowRangePin(low_pin),
  midRangePin(mid_pin),
  highRangePin(high_pin),
  negPin(neg_pin),
  peizoPin(peizo_pin),
  ledPin(led_pin),
  ampState(LOW_RANGE)
{
}

void Sampler::begin(uint8_t sample_mode)
{
  pinMode(ledPin, OUTPUT);
  pinMode(peizoPin, OUTPUT);
  pinMode(negPin, INPUT_PULLDOWN);
  
  pinMode(lowRangePin, OUTPUT);
  pinMode(midRangePin, OUTPUT);
  pinMode(highRangePin, OUTPUT);
  sampleMode = sample_mode;
  setAmpState(ampState);
  adc.begin();
  adc.noNeg();
  //adc.pga2x();
}

void Sampler::setSampleMode(uint8_t sample_mode)
{
  sampleMode = sample_mode;
}

uint8_t Sampler::getSampleMode()
{
  return sampleMode;
}

void Sampler::incrementSampleMode()
{
  setSampleMode((getSampleMode() + 1) % 7);
}

TypedSample_t Sampler::sample()
{
  digitalWrite(ledPin, HIGH);
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
  digitalWrite(ledPin, LOW);
  return s;
}

float Sampler::getDCVoltage() {
  if (sign() == 1) {  // positive
    switch (ampState) {
      case LOW_RANGE:
        return adc.readmV();
      case MID_RANGE:
        return (0.2069 * (adc.readmV()/1000)) + 0.04;
      case HIGH_RANGE:
        return (0.0846 * (adc.readmV()/1000)) + 0.1046;
    }
    return 0.0;
  } else {  // negative
    switch (ampState) {
      case LOW_RANGE:
        return adc.readmV();
      case MID_RANGE:
        return adc.readmV();
      case HIGH_RANGE:
        return adc.readmV();
    }
    return 0.0;
  }
}

float Sampler::getACVoltage() {
  switch (ampState) {
    case LOW_RANGE:
      return (0.9645 * adc.readmV()) + 174.06;
    case MID_RANGE:
      return (0.2069 * (adc.readmV()/1000)) + 0.04;
    case HIGH_RANGE:
      return (0.0846 * (adc.readmV()/1000)) + 0.1046;
  }
  return 0.0;
}

float Sampler::getDCCurrent() {
  if (sign() == 1) {  // positive
    switch (ampState) {
      case LOW_RANGE:
        return (0.9645 * adc.readmV()) + 174.06;
      case MID_RANGE:
        return (0.2069 * (adc.readmV()/1000)) + 0.04;
      case HIGH_RANGE:
        return (0.0846 * (adc.readmV()/1000)) + 0.1046;
    }
    return 0.0;
  } else {  // negative
    switch (ampState) {
      case LOW_RANGE:
        return adc.readmV();
      case MID_RANGE:
        return adc.readmV();
      case HIGH_RANGE:
        return adc.readmV();
    }
    return 0.0;
  }
}

float Sampler::getACCurrent() {
  switch (ampState) {
    case LOW_RANGE:
      return (0.9645 * adc.readmV()) + 174.06;
    case MID_RANGE:
      return (0.2069 * (adc.readmV()/1000)) + 0.04;
    case HIGH_RANGE:
      return (0.0846 * (adc.readmV()/1000)) + 0.1046;
  }
  return 0.0;
}

float Sampler::getResistance() {
  return -1.0;
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

void Sampler::setAmpState(uint8_t range) {
  ampState = range;
  switch(ampState) {
    case LOW_RANGE:
      digitalWrite(lowRangePin, LOW);
      digitalWrite(midRangePin, HIGH);
      digitalWrite(highRangePin, HIGH);
      break;
    case MID_RANGE:
      digitalWrite(lowRangePin, HIGH);
      digitalWrite(midRangePin, LOW);
      digitalWrite(highRangePin, HIGH);
      break;
    case HIGH_RANGE:
    default:
      digitalWrite(lowRangePin, HIGH);
      digitalWrite(midRangePin, HIGH);
      digitalWrite(highRangePin, LOW);
      break;
  }
}

