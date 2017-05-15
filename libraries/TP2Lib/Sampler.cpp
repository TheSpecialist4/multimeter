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

Sampler::Sampler(uint8_t low_pin, uint8_t mid_pin, uint8_t high_pin, uint8_t neg_pin, uint8_t peizo_pin, uint8_t led_pin) :
  adc(),
  lowRangePin(low_pin),
  midRangePin(mid_pin),
  highRangePin(high_pin),
  negPin(neg_pin),
  peizoPin(peizo_pin),
  ledPin(led_pin),
  ampState(HIGH_RANGE)
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
}

void Sampler::setSampleMode(uint8_t sample_mode)
{
  sampleMode = sample_mode;
}

Typed_Sample_t Sampler::sample()
{
  digitalWrite(ledPin, HIGH);
  Typed_Sample_t s;
  s.type = sampleMode;
  switch (sampleMode) {
    case DC_VOLTAGE:
      s.sample.floatRep = sign() * getVoltage();
      break;
    case AC_VOLTAGE:
      s.sample.floatRep = getVoltage();
      break;
    case DC_CURRENT:
      s.sample.floatRep = sign() * getCurrent();
      break;
    case AC_CURRENT:
      s.sample.floatRep = getCurrent();
      break;
    case RESISTANCE:
      s.sample.floatRep = getResistance();
      break;
    case CONTINUITY:
      s.sample.floatRep = getResistance();
      break;
    case LOGIC:
      s.sample.floatRep = getLogic();
      break;
  }
  digitalWrite(ledPin, LOW);
  return s;
}

float Sampler::getVoltage() {
  switch (ampState) {
    case LOW_RANGE:
      return sign() * (adc.readADC() / 100000.0);
    case MID_RANGE:
      return sign() * (adc.readADC() / 20000.0);
    case HIGH_RANGE:
      return sign() * (adc.readADC() / 10000.0);
  }
  return -1.0;
}

float Sampler::getCurrent() {
  return -1.0;
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

