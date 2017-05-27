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
  lcd.write("                ");
  lcd.setCursor(0, 0);
  if (sample.value.floatRep == NAN)
  {
    lcd.write("OVER LIMIT");
    return;
  }
  switch(sample.type) {
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      snprintf(strBuffer, 10, "%fV", sample.value.floatRep);
      lcd.write(strBuffer);
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      snprintf(strBuffer, 10, "%fA", sample.value.floatRep);
      break;
    case RESISTANCE:
      snprintf(strBuffer, 10, "%f", sample.value.floatRep);
      lcd.write(strBuffer);
      lcd.write(OHM);
      break;
    case CONTINUITY:
      if (sample.value.floatRep < 5.0) {
        lcd.write("CONNECTED");
      } else {
        lcd.write("DISCONNECTED");
      }
      break;
    case LOGIC:
      if (sample.value.floatRep > 1.0) {
        lcd.write("ON");
      } else {
        lcd.write("OFF");
      }
      break;
  }
}

void MultimeterScreen::displayResolution(uint8_t type, uint8_t resolution)
{
  lcd.setCursor(10, 1);
  lcd.print("     ");
  lcd.setCursor(10, 1);
  switch(type){
    case DC_VOLTAGE:
    case AC_VOLTAGE:
      lcd.print(PLUS_MINUS);
      switch(resolution) {
        case 1:
          lcd.print("12");
          break;
        case 2:
          lcd.print("5");
          break;
        case 3:
          lcd.print("1");
          break;
      }
      lcd.print("V");
      break;
    case DC_CURRENT:
    case AC_CURRENT:
      lcd.print(PLUS_MINUS);
      switch(resolution) {
        case 2:
          lcd.print("200");
          break;
        case 3:
          lcd.print("10");
          break;
      }
      lcd.print("mA");
      break;
    case RESISTANCE:
      lcd.print(PLUS_MINUS);
      switch(resolution) {
        case 1:
          lcd.print("1k");
          break;
        case 2:
          lcd.print("1M");
          break;
      }
      lcd.print(OHM);
      break;
      break;
    case CONTINUITY:
      break;
    case LOGIC:
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

void MultimeterScreen::displaySampleRate(uint8_t sample_period)
{
  lcd.setCursor(4, 1);
  switch(sample_period) {
    case HALF_SEC:
      lcd.write("2/s ");
      break;
    case ONE_SEC:
      lcd.write("1/s ");
      break;
    case TWO_SEC:
      lcd.write("30/m");
      break;
    case FIVE_SEC:
      lcd.write("12/m");
      break;
    case TEN_SEC:
      lcd.write("6/m ");
      break;
    case ONE_MIN:
      lcd.write("1/m ");
      break;
    case TWO_MIN:
      lcd.write("30/h");
      break;
    case FIVE_MIN:
      lcd.write("12/h");
      break;
    case TEN_MIN:
      lcd.write("6/h ");
      break;
  }
}

void MultimeterScreen::displayBrightness(uint8_t new_brightness)
{
  brightness = new_brightness;
  lcd.setCursor(15, 1);
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