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

MultimeterScreen::MultimeterScreen(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t pwmPin) :
  lcd(rs, en, d4, d5, d6, d7),
  brightnessPWMPin(pwmPin)
{
  ;
}

void MultimeterScreen::begin()
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
}

void MultimeterScreen::displaySample(Typed_Sample_t sample)
{
  lcd.setCursor(0, 0);
  lcd.write("                ");
  lcd.setCursor(0, 0);
  if (sample.sample.floatRep == NAN)
  {
    lcd.write("OVER LIMIT");
    return;
  }
  switch(sample.type) {
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      snprintf(strBuffer, 10, "%fV", sample.sample.floatRep);
      lcd.write(strBuffer);
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      snprintf(strBuffer, 10, "%fA", sample.sample.floatRep);
      break;
    case RESISTANCE:
      snprintf(strBuffer, 10, "%f", sample.sample.floatRep);
      lcd.write(strBuffer);
      lcd.write(OHM);
      break;
    case CONTINUITY:
      if (sample.sample.floatRep > 1) {
        lcd.write("CONNECTED");
      } else {
        lcd.write("DISCONNECTED");
      }
      break;
    case LOGIC:
      if (sample.sample.floatRep > 1) {
        lcd.write("ON");
      } else {
        lcd.write("OFF");
      }
      break;
  }
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
      lcd.write("RES");
      break;
    case CONTINUITY:
      lcd.write("CNT");
      break;
    case LOGIC:
      lcd.write("LGC");
      break;
  }
}

void MultimeterScreen::displaySampleRate(unsigned long sample_period)
{
  lcd.setCursor(4, 1);
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
}

void MultimeterScreen::displayBrightness(uint8_t brightness)
{
  lcd.setCursor(15, 1);
  lcd.write((uint8_t) brightness);
  analogWrite(brightnessPWMPin, (4 - brightness)*51);
}

void MultimeterScreen::setLoggingConfigMode(uint8_t sample_mode, unsigned long sample_period)
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