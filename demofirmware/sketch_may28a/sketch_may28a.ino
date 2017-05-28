#include "Energia.h"
#include "NAU7802.h"

NAU7802 adc;

void setup() {
  adc.begin();

}

void loop() {
  // put your main code here, to run repeatedly: 
    Serial.println(adc.readmV());
}
