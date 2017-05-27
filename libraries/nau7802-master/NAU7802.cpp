//Readout for nau7802
#include "NAU7802.h"

//Instantiates a new NAU7802 class
//================================
NAU7802::NAU7802() {
}

// Setups the HW
//==============
#if defined NAU7802_SOFTWAREWIRE
  boolean NAU7802::begin(uint8_t sda, uint8_t scl, uint8_t addr) {
    _i2caddr = addr;
    wire = SoftwareWire(sda,scl);
    wire.begin();
    resetSettings();
    return true;
  }
#elif defined ESP8266
  boolean NAU7802::begin(uint8_t sda, uint8_t scl, uint8_t addr) {
    _i2caddr = addr;
    wire = TwoWire();
    wire.begin(sda,scl);
    resetSettings();
    return true;
  }
#else
  boolean NAU7802::begin(uint8_t addr) {
    _i2caddr = addr;
    wire = TwoWire();
    wire.begin();
    resetSettings();
    return true;
  }
#endif //ESP8266|NAU7802_SOFTWAREWIRE|Default


void NAU7802::resetSettings(){
  writeBit(NAU7802_PU_CTRL, NAU7802_RR);        //Reset Registers
  clearBit(NAU7802_PU_CTRL, NAU7802_RR);        //Clear Reset Registers
  writeBit(NAU7802_PU_CTRL, NAU7802_PUD);       //Power up digital
  readUntilTrue(NAU7802_PU_CTRL, NAU7802_PUR);  //Wait until power up
  writeBit(NAU7802_PU_CTRL, NAU7802_PUA);       //Power up analog

  writeBit(NAU7802_ADC_REG, 4);                 //Diasble chopper funcition
  writeBit(NAU7802_ADC_REG, 5);                 //Diasble chopper funcition

  writeBit(NAU7802_PGA_REG, 0);                 //Diasble chopper funcition
  writeBit(NAU7802_PGA_REG, 4);                 //Bypass PGA

  calibrate();                                  //Calibrate

  writeBit(NAU7802_I2C_CTRL, NAU7802_SPE);      //Enable Strong Pullup

  writeBit(NAU7802_PU_CTRL,   NAU7802_CS);      //Start Conversion
}

void NAU7802::calibrate(){
    writeBit(NAU7802_CTRL2, NAU7802_CALS);        //Begin calibration
    readUntilFalse(NAU7802_CTRL2, NAU7802_CALS);  //Wait for calibration to finish
}

//Reads set channel
//======================
long NAU7802::readADC(){
  readUntilTrue(NAU7802_PU_CTRL,NAU7802_CR);
  uint32_t adcVal = read24(NAU7802_ADC_B2);
  writeBit(NAU7802_PU_CTRL, NAU7802_CS);
  if(adcVal & 0x00800000){
    adcVal = ~adcVal+1;
    adcVal = -1*(adcVal & 0x00FFFFFF);
  }
  return adcVal;
}

uint32_t NAU7802::readRawADC(){
  readUntilTrue(NAU7802_PU_CTRL,NAU7802_CR);
  uint32_t adcVal = read24(NAU7802_ADC_B2);
  writeBit(NAU7802_PU_CTRL, NAU7802_CS);
  if(adcVal & 0x00800000){
    adcVal = (~adcVal+1) & 0x00FFFFFF;
    // adcVal += 16777216;
  }
  return adcVal;
}

float NAU7802::readmV(){
  return (_avcc/(float)16777216)*((float)readADC()/_pga_gain);
}

//Select channel
//=============
void NAU7802::selectCh1(){
  clearBit(NAU7802_CTRL2, 7);
  calibrate();
}
void NAU7802::selectCh2(){
  writeBit(NAU7802_CTRL2, 7);
  calibrate();
}
void NAU7802::selectTemp(){
}

//Set Sampling rate
//================
void NAU7802::rate010sps(){
  uint8_t val = 0x00;
  val |= (read(NAU7802_CTRL2) & 0x8f);
  write(NAU7802_CTRL2, val);
}
void NAU7802::rate020sps(){
  uint8_t val = 0x10;
  val |= (read(NAU7802_CTRL2) & 0x8f);
  write(NAU7802_CTRL2, val);
}
void NAU7802::rate040sps(){
  uint8_t val = 0x20;
  val |= (read(NAU7802_CTRL2) & 0x8f);
  write(NAU7802_CTRL2, val);
}
void NAU7802::rate080sps(){
  uint8_t val = 0x30;
  val |= (read(NAU7802_CTRL2) & 0x8f);
  write(NAU7802_CTRL2, val);
}
void NAU7802::rate320sps(){
  uint8_t val = 0x70;
  val |= (read(NAU7802_CTRL2) & 0x8f);
  write(NAU7802_CTRL2, val);
}

//Set Pre-Gain Amplifier
//======================
void NAU7802::pga128x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 128.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x40);
}
void NAU7802::pga64x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 64.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x20);
}
void NAU7802::pga32x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 32.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x10);
}
void NAU7802::pga16x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 16.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x08);
}
void NAU7802::pga8x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 8.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x04);
}
void NAU7802::pga4x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 4.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x02);
}
void NAU7802::pga2x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 2.0;
  write(NAU7802_GCAL1_B2, 0x00);
  write(NAU7802_GCAL1_B3, 0x01);
}
void NAU7802::pga1x(){
  clearBit(NAU7802_PGA_REG, 4);                 //Enable PGA
  _pga_gain = 1.0;
  write(NAU7802_GCAL1_B2, 0x80);
  write(NAU7802_GCAL1_B3, 0x00);
}
void NAU7802::pgaDisable(){
  writeBit(NAU7802_PGA_REG, 4);                 //Bypass PGA
  _pga_gain = 1.0;
}

void NAU7802::noNeg(){
  // write(NAU7802_OCAL1_B0, 0xFF);
  // write(NAU7802_OCAL1_B1, 0xFF);
  write(NAU7802_OCAL1_B2, 0x20);
}

//Set AVCC, Internal LDO
//======================
void NAU7802::extAvcc(float extAvcc){
  clearBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Disable LDO
  _avcc=extAvcc;
}
void NAU7802::avcc2V4(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  writeBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 2.4V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 2.4V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 2.4V
  _avcc=2.4;                                  //Local AVCC variable
}
void NAU7802::avcc2V7(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  writeBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 2.7V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 2.7V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 2.7V
  _avcc=2.7;                                  //Local AVCC variable
}
void NAU7802::avcc3V0(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  writeBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 3.0V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 3.0V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 3.0V
  _avcc=3.0;                                  //Local AVCC variable
}
void NAU7802::avcc3V3(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  writeBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 3.3V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 3.3V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 3.3V
  _avcc=3.3;                                  //Local AVCC variable
}
void NAU7802::avcc3V6(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  clearBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 3.6V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 3.6V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 3.6V
  _avcc=3.6;                                  //Local AVCC variable
}
void NAU7802::avcc3V9(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  clearBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 3.9V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 3.9V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 3.9V
  _avcc=3.9;                                  //Local AVCC variable
}
void NAU7802::avcc4V2(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  clearBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 4.2V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 4.2V
  writeBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 4.2V
  _avcc=4.2;                                  //Local AVCC variable
}
void NAU7802::avcc4V5(){
  writeBit(NAU7802_PU_CTRL, NAU7802_AVDDS);   //Enable LDO
  clearBit(NAU7802_CTRL1, NAU7802_VLDO2);     //Set AVCC to 4.5V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO1);     //Set AVCC to 4.5V
  clearBit(NAU7802_CTRL1, NAU7802_VLDO0);     //Set AVCC to 4.5V
  _avcc=4.5;                                  //Local AVCC variable
}

//Low level read and write procedures
//===================================

void NAU7802::write(uint8_t reg, uint8_t val) {
  wire.beginTransmission(_i2caddr);
  wire.write((uint8_t)reg);
  wire.write(val);
  wire.endTransmission();
}

void NAU7802::writeBit(uint8_t reg, uint8_t bit) {
  uint8_t val = read(reg) | (1<<bit);
  write(reg, val);
}

void NAU7802::clearBit(uint8_t reg, uint8_t bit) {
  uint8_t val = read(reg) & ~(1<<bit);
  write(reg, val);
}

uint8_t NAU7802::read(uint8_t reg) {
  wire.beginTransmission(_i2caddr);
  wire.write((uint8_t)reg);
  wire.endTransmission();

  wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return wire.read();
}

boolean NAU7802::readBit(uint8_t reg, uint8_t bit){
  //create bitmask
  uint8_t bitmask = 1<<bit;
  if(read(reg) & bitmask){
    return true;
  }
  return false;
}

uint32_t NAU7802::read24(uint8_t reg) {
  uint32_t val;

  wire.beginTransmission(_i2caddr);
  wire.write((uint8_t)reg);
  wire.endTransmission();

  wire.requestFrom((uint8_t)_i2caddr, (uint8_t)3);
  val = wire.read();    // receive high byte
  val <<= 8;            // shift byte to make room for new byte
  val |= wire.read();   // receive mid byte
  val <<= 8;            // shift both bytes
  val |= wire.read();   // receive low byte
  return val;
}

uint32_t NAU7802::read32(uint8_t reg) {
  uint32_t val;

  wire.beginTransmission(_i2caddr);
  wire.write((uint8_t)reg);
  wire.endTransmission();

  wire.requestFrom((uint8_t)_i2caddr, (uint8_t)4);
  val = wire.read();    // receive [31:24] byte
  val <<= 8;            // shift byte
  val = wire.read();    // receive [23:16] byte
  val <<= 8;            // shift bytes
  val |= wire.read();   // receive [15:08] byte
  val <<= 8;            // shift bytes
  val |= wire.read();   // receive [07:00] byte
  return val;
}

void NAU7802::readUntilTrue(uint8_t reg, uint8_t bit) {
  //create bitmask
  uint8_t bitmask = 1<<bit;
  bool readUntil = false;
  //Just keep reading until bit requested is true
  while(readUntil == false){
    if(read(reg) & bitmask){
      readUntil=true;
    }
  }
}

void NAU7802::readUntilFalse(uint8_t reg, uint8_t bit) {
  //create bitmask
  uint8_t bitmask = 1<<bit;
  bool readUntil = false;
  //Just keep reading until bit requested is false
  while(readUntil == false){
    if(~read(reg) & bitmask){
      readUntil=true;
    }
  }
}
