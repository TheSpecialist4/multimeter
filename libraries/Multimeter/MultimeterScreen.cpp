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
#include "MultimeterScreen.h"
#include <Energia.h>
#include <stdio.h>


#define OHM         ((uint8_t) 5)
#define PLUS_MINUS  ((uint8_t) 6)

MultimeterScreen::MultimeterScreen(const uint8_t rs, const uint8_t en, const uint8_t d4, const uint8_t d5,
                                    const uint8_t d6, const uint8_t d7, const uint8_t pwmPin) :
  lcd(rs, en, d4, d5, d6, d7),
  brightnessPWMPin(pwmPin)
{
  ;
}

void MultimeterScreen::begin(uint8_t default_brightness)
{
  lcd.createChar(0, brightness0);
  lcd.createChar(1, brightness1);
  lcd.createChar(2, brightness2);
  lcd.createChar(3, brightness3);
  lcd.createChar(4, brightness4);
  lcd.createChar(OHM, ohm);
  lcd.createChar(PLUS_MINUS, plusMinus);
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(brightnessPWMPin, OUTPUT);
  displayBrightness(default_brightness);
}

void MultimeterScreen::displaySample(TypedSample_t sample)
{
  lcd.setCursor(0, 0);
  lcd.write("               ");
  lcd.setCursor(0, 0);
  if (sample.value.floatRep == NAN)
  {
    lcd.write("OVER LIMIT");
    return;
  }
  switch(sample.type) {
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      displayVoltage(sample);
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      displayCurrent(sample);
      break;
    case RESISTANCE:
      displayResistance(sample);
      break;
    case CONTINUITY:
      displayContinuity(sample);
      break;
    case LOGIC:
      displayLogic(sample);
      break;
  }
}

void MultimeterScreen::displayVoltage(TypedSample_t sample)
{
  switch(sample.resolution) {
    case 3:
      snprintf(strBuffer, 15, "%fmV", sample.value.floatRep);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.setCursor(13, 1);
      lcd.write(PLUS_MINUS);
      lcd.write("1V");
      break;
    case 2:
      snprintf(strBuffer, 15, "%fV", sample.value.floatRep/1000);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.setCursor(13, 1);
      lcd.write(PLUS_MINUS);
      lcd.write("5V");
      break;
    case 1:
      snprintf(strBuffer, 15, "%fV", sample.value.floatRep/1000);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.setCursor(12, 1);
      lcd.write(PLUS_MINUS);
      lcd.write("12V");
      break;
  }
}

void MultimeterScreen::displayCurrent(TypedSample_t sample)
{
    switch(sample.resolution) {
    case 3:
      snprintf(strBuffer, 15, "%fmA", sample.value.floatRep);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.setCursor(11, 1);
      lcd.write(PLUS_MINUS);
      lcd.write("10mA");
      break;
    case 2:
      snprintf(strBuffer, 15, "%fmA", sample.value.floatRep);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.setCursor(10, 1);
      lcd.write(PLUS_MINUS);
      lcd.write("200mA");
      break;
  }
}

void MultimeterScreen::displayResistance(TypedSample_t sample)
{
  switch(sample.resolution) {
    case 3:
      snprintf(strBuffer, 14, "%f", sample.value.floatRep);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.write(OHM);
      lcd.setCursor(13, 1);
      lcd.write("1k");
      lcd.write(OHM);
      break;
    case 2:
      snprintf(strBuffer, 14, "%f", sample.value.floatRep/1000);
      lcd.setCursor(0, 0);
      lcd.write(strBuffer);
      lcd.write(OHM);
      lcd.setCursor(13, 1);
      lcd.write("1M");
      lcd.write(OHM);
      break;
  }
}

void MultimeterScreen::displayContinuity(TypedSample_t sample)
{
  if (sample.value.floatRep < 5.0) {
    lcd.write("CONNECTED");
  } else {
    lcd.write("DISCONNECTED");
  }
}

void MultimeterScreen::displayLogic(TypedSample_t sample)
{
  lcd.write("hello");
}

void MultimeterScreen::displaySampleMode(uint8_t sample_mode)
{
  lcd.setCursor(0, 1);
  switch(sample_mode) {
    case DC_VOLTAGE:
      lcd.write("DCV");
      break;
    case AC_VOLTAGE:
      lcd.write("ACV");
      break;
    case DC_CURRENT:
      lcd.write("DCC");
      break;
    case AC_CURRENT:
      lcd.write("ACC");
      break;
    case RESISTANCE:
      lcd.write(OHM);
      lcd.write("TR");
      break;
    case CONTINUITY:
      lcd.write("CNT");
      break;
    case LOGIC:
      lcd.write("LGC");
      break;
  }
}

void MultimeterScreen::displaySampleRate(uint8_t sample_period)
{
  lcd.setCursor(4, 1);
  switch(sample_period) {
    case HALF_SEC:
      lcd.write(" 2/sec");
      break;
    case ONE_SEC:
      lcd.write(" 1/sec");
      break;
    case TWO_SEC:
      lcd.write("30/min");
      break;
    case FIVE_SEC:
      lcd.write("12/min");
      break;
    case TEN_SEC:
      lcd.write(" 6/min");
      break;
    case ONE_MIN:
      lcd.write(" 1/min");
      break;
    case TWO_MIN:
      lcd.write("30/hr ");
      break;
    case FIVE_MIN:
      lcd.write("12/hr ");
      break;
    case TEN_MIN:
      lcd.write(" 6/hr ");
      break;
  }
}

void MultimeterScreen::displayBrightness(uint8_t new_brightness)
{
  brightness = new_brightness;
  lcd.setCursor(15, 0);
  lcd.write((uint8_t) brightness);
  analogWrite(brightnessPWMPin, (4 - brightness)*51);
}

void MultimeterScreen::incrementBrightness()
{
  displayBrightness(nextBrightness(brightness));
}

void MultimeterScreen::setLoggingConfigMode(uint8_t sample_mode, uint8_t sample_period)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  switch(sample_mode) {
    case DC_VOLTAGE:
      lcd.write("DCV");
      break;
    case AC_VOLTAGE:
      lcd.write("ACV");
      break;
    case DC_CURRENT:
      lcd.write("DCC");
      break;
    case AC_CURRENT:
      lcd.write("ACC");
      break;
    case RESISTANCE:
      lcd.write("RES");
      break;
    case CONTINUITY:
      lcd.write("CNT");
      break;
    case LOGIC:
      lcd.write("LGC");
      break;
  }

  lcd.setCursor(4, 0);
  
  switch(sample_period) {
    case HALF_SEC:
      lcd.write("2/sec ");
      break;
    case ONE_SEC:
      lcd.write("1/sec ");
      break;
    case TWO_SEC:
      lcd.write("30/min");
      break;
    case FIVE_SEC:
      lcd.write("12/min");
      break;
    case TEN_SEC:
      lcd.write("6/min ");
      break;
    case ONE_MIN:
      lcd.write("1/min ");
      break;
    case TWO_MIN:
      lcd.write("30/hr ");
      break;
    case FIVE_MIN:
      lcd.write("12/hr ");
      break;
    case TEN_MIN:
      lcd.write("6/hr  ");
      break;
  }

  lcd.setCursor(0, 1);
  lcd.write("+1000 +100 +1 GO");
}

uint8_t MultimeterScreen::nextBrightness(uint8_t current_brightness)
{
  return (current_brightness + 1) % 5;
}