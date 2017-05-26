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
                const uint8_t resistor_1_pin, const uint8_t resistor_2_pin, const uint8_t mirror_switch_pin,
                const uint8_t switch_1_pin, const uint8_t neg_pin, const uint8_t peizo_pin, const uint8_t led_pin) :
  adc(),
  lowGainPin(low_gain_pin),
  midGainPin(mid_gain_pin),
  highGainPin(high_gain_pin),
  resistor1Pin(resistor_1_pin),
  resistor2Pin(resistor_2_pin),
  mirrorSwitchPin(mirror_switch_pin),
  switch1Pin(switch_1_pin),
  negPin(neg_pin),
  peizoPin(peizo_pin),
  ledPin(led_pin),
  ampState(LOW_GAIN)
{
}

void Sampler::begin(uint8_t sample_mode)
{
  pinMode(ledPin, OUTPUT);
  pinMode(peizoPin, OUTPUT);
  pinMode(negPin, INPUT_PULLDOWN);
  
  pinMode(lowGainPin, OUTPUT);
  pinMode(midGainPin, OUTPUT);
  pinMode(highGainPin, OUTPUT);

  pinMode(resistor1Pin, OUTPUT);
  pinMode(resistor2Pin, OUTPUT);
  pinMode(mirrorSwitchPin, OUTPUT);
  pinMode(switch1Pin, OUTPUT);

  sampleMode = sample_mode;
  setSampleMode(sampleMode);
  adc.begin();
  adc.noNeg();
  //adc.pga2x();
}

void Sampler::setSampleMode(uint8_t sample_mode)
{
  sampleMode = sample_mode;
  switch(sampleMode) {
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      setAmpState(HIGH_GAIN);
      setOhmeterState(OHMETER_OFF);
      digitalWrite(mirrorSwitchPin, LOW);
      digitalWrite(switch1Pin, HIGH);
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      setAmpState(HIGH_GAIN);
      setOhmeterState(OHMETER_OFF);
      digitalWrite(mirrorSwitchPin, LOW);
      digitalWrite(switch1Pin, LOW);
      break;
    case RESISTANCE:
      setAmpState(HIGH_GAIN);
      setOhmeterState(SMALL_RESISTOR);
      digitalWrite(mirrorSwitchPin, HIGH);
      digitalWrite(switch1Pin, HIGH);
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
      case LOW_GAIN:
        return (0.9645 * adc.readmV()) + 174.06;
      case MID_GAIN:
        return (0.2069 * adc.readmV()) + 0.04;
      case HIGH_GAIN:
        return (0.0846 * adc.readmV()) + 0.1046;
    }
    return 0.0;
  } else {  // negative
    switch (ampState) {
      case LOW_GAIN:
        return (-0.9522 * adc.readmV()) + 180.76;
      case MID_GAIN:
        return (adc.readmV());
      case HIGH_GAIN:
        return adc.readmV();
    }
    return 0.0;
  }
}

float Sampler::getACVoltage() {
  switch (ampState) {
    case LOW_GAIN:
      return (0.9645 * adc.readmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * adc.readmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * adc.readmV()) + 0.1046;
  }
  return 0.0;
}

float Sampler::getDCCurrent() {
  if (sign() == 1) {  // positive
    switch (ampState) {
      case LOW_GAIN:
        return (0.9645 * adc.readmV()) + 174.06;
      case MID_GAIN:
        return (0.2069 * adc.readmV()) + 0.04;
      case HIGH_GAIN:
        return (0.0846 * adc.readmV()) + 0.1046;
    }
    return 0.0;
  } else {  // negative
    switch (ampState) {
      case LOW_GAIN:
        return adc.readmV();
      case MID_GAIN:
        return adc.readmV();
      case HIGH_GAIN:
        return adc.readmV();
    }
    return 0.0;
  }
}

float Sampler::getACCurrent() {
  switch (ampState) {
    case LOW_GAIN:
      return (0.9645 * adc.readmV()) + 174.06;
    case MID_GAIN:
      return (0.2069 * adc.readmV()) + 0.04;
    case HIGH_GAIN:
      return (0.0846 * adc.readmV()) + 0.1046;
  }
  return 0.0;
}

float Sampler::getResistance() {
  switch(ohmeterState) {
    case OHMETER_OFF:
      return -1.0;
    case SMALL_RESISTOR:
      return -1.0;
    case BIG_RESISTOR:
      return -1.0;
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
      digitalWrite(resistor1Pin, HIGH);
      digitalWrite(resistor2Pin, HIGH);
      break;
    case SMALL_RESISTOR:
      digitalWrite(resistor1Pin, LOW);
      digitalWrite(resistor2Pin, HIGH);
      break;
    case BIG_RESISTOR:
      digitalWrite(resistor1Pin, HIGH);
      digitalWrite(resistor2Pin, LOW);
      break;
  }
}