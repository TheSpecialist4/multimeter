 /*
  The circuit:
 * LCD RS pin to digital pin 5
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 23
 * LCD D5 pin to digital pin 24
 * LCD D6 pin to digital pin 25
 * LCD D7 pin to digital pin 26
 * LCD R/W pin to ground
 */
// include the library code:
#include <LiquidCrystal.h>

#define ANALOG_PIN 29
#define GAIN_PIN_1 7  // 0-1 - orange
#define GAIN_PIN_2 8  // 1-5 - grey
#define GAIN_PIN_3 9  // 5-12 - purple

#define VOLTAGE 0
#define CURRENT 1

#define RANGE5TO12 3
#define RANGE1TO5  2
#define RANGE0TO1  1

#define RESISTOR  10.0

#define button  PUSH1

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 6, 23, 24, 25, 26);

byte customChar1[8] = {
  0b11111,
  0b00001,
  0b11101,
  0b10101,
  0b10101,
  0b10101,
  0b10001,
  0b11111
};

byte customChar2[8] = {
  0b11100,
  0b00010,
  0b01001,
  0b10101,
  0b10101,
  0b10010,
  0b01000,
  0b00111
};

int ADC_read;
float Vread;
int gain;
int gainCount;
int mode;
int button_state;
int last_button_state = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.createChar(0, customChar1);
  lcd.createChar(1, customChar2);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  lcd.write((uint8_t) 0);
  lcd.write((uint8_t) 0);
  lcd.write((uint8_t) 0);
  lcd.write((uint8_t) 1);
  lcd.write((uint8_t) 1);
  lcd.write((uint8_t) 1);

  pinMode(GAIN_PIN_1, OUTPUT);
  pinMode(GAIN_PIN_2, OUTPUT);
  pinMode(GAIN_PIN_3, OUTPUT);
  pinMode(button, INPUT_PULLUP);   

  digitalWrite(GAIN_PIN_1, HIGH);
  digitalWrite(GAIN_PIN_2, HIGH);
  digitalWrite(GAIN_PIN_3, LOW);
  
  ADC_read = 0;
  gain = RANGE5TO12;
  mode = VOLTAGE;
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(8, 0);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  
  int button_read = digitalRead(button);

  if (button_read != last_button_state) {
    lastDebounceTime = millis();
    Serial.print(button_read);
    Serial.print(button_state);
    Serial.print("_\n");
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    button_state = button_read;
    Serial.print(button_state);
    Serial.print("-\n");
  }
  
  if (button_state == HIGH && last_button_state == LOW) {
    switch (mode) {
      case VOLTAGE:
        mode = CURRENT;
        break;

      case CURRENT:
        mode = VOLTAGE;
        break;
    }
    Serial.print(mode);
    Serial.print("\n");
  }
  
  last_button_state = button_state;

  ADC_read = analogRead(ANALOG_PIN);
  Vread = readToVoltage(ADC_read, gain);

  switch (gain) {
    case RANGE5TO12:
      if (Vread < 4.5) {
        gain = RANGE1TO5;
        setGainPins(gain);
      }
      break;

    case RANGE1TO5:
      if (Vread > 4.5) {
        gain = RANGE5TO12;
        setGainPins(gain);
      } else if (Vread < 0.8) {
        gain = RANGE0TO1;
        setGainPins(gain);
      }
      break;
      
    case RANGE0TO1:
      if (Vread > 0.8) {
        gain = RANGE1TO5;
        setGainPins(gain);
      }
      break;
  }

  if (mode == CURRENT) {
    Vread = Vread / RESISTOR;
  }
  printValue(Vread, mode);
  Serial1.print( (int) (Vread * 100) );
}

float readToVoltage(int readVal, int gainVal) {
  if (gainVal == 3) {
    return (readVal / 4095.0) * 12;
  } else if (gainVal == 2) {
    return (readVal / 4095.0) * 5;
  } else if (gainVal == 1) {
    return (readVal / 4095.0) * 1;
  }
}



void printValue(float readVal, int valMode) {
  if (readVal >= 10.0) {
    lcd.setCursor(0, 1);
  } else {
    lcd.setCursor(1, 1);
  }
  lcd.print(readVal);
  
  switch (valMode) {
    case VOLTAGE:
      lcd.print("V ");
      break;

    case CURRENT:
      lcd.print("A ");
      break;
  }
}

void setGainPins(int gain) {
  switch (gain) {
    case 1:
      digitalWrite(GAIN_PIN_1, LOW);
      digitalWrite(GAIN_PIN_2, HIGH);
      digitalWrite(GAIN_PIN_3, HIGH);
      break;

    case 2:
      digitalWrite(GAIN_PIN_1, HIGH);
      digitalWrite(GAIN_PIN_2, LOW);
      digitalWrite(GAIN_PIN_3, HIGH);
      break;
      
    case 3:
      digitalWrite(GAIN_PIN_1, HIGH);
      digitalWrite(GAIN_PIN_2, HIGH);
      digitalWrite(GAIN_PIN_3, LOW);
      break;
  }
}


