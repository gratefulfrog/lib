/* LEDManager.cpp
 * this is the library class that controls the leds
 * in the ArduStompBox
 */

#include <LEDManager.h>

unsigned int LEDManager::ledArray = 0;

// This method sends bits to the shift registers:
void LEDManager::registerWrite() {
  // turn off the output so the leds don't light up
  // while you're shifting bits:
  digitalWrite(LATCHPIN, LOW);
  delay(5);
  //Serial.print("Called LEDManager::registerWrite:\t" );
  //Serial.println(ledArray,BIN);
  SPI.transfer((byte)(LEDManager::ledArray >> 8));
  SPI.transfer((byte)(LEDManager::ledArray & 0B11111111));
  
  // turn on the output so the LEDs can light up:
  digitalWrite(LATCHPIN, HIGH);
}

byte LEDManager::leftShift(byte ledID) {
  byte ret = 0;
  for (byte b = 0; b<ledID;b++){
    ret+= ArduConf00::nbLeds[b];
  }
  return ret;
}

void LEDManager::init(){
  pinMode(LATCHPIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
  LEDManager::zeroAll();
}

void LEDManager::zeroAll(){
  LEDManager::ledArray = 0;
  LEDManager::registerWrite();
}

/*
void LEDManager::zeroAll(boolean reallyAll){
  if (reallyAll){
    LEDManager::ledArray = 0;
  }
  else{ // leave connect and power
    LEDManager::ledArray &= ((1 << leftShift(ArduConf00::connectID)) | (1<< leftShift(ArduConf00::powerID)));
  }
  LEDManager::registerWrite();
}
*/

void LEDManager::setAll(){
  unsigned int zero = 0;
  LEDManager::ledArray = ~zero;
  LEDManager::registerWrite();
}
void LEDManager::flashLeds(){
  unsigned int leds = LEDManager::ledArray;
  for (byte b=0;b<NB_LED_FLASHES;b++){
    LEDManager::setAll();
    delay(LED_FLASH_DELAY);
    LEDManager::zeroAll();
    delay(LED_FLASH_DELAY);
  }
  LEDManager::ledArray = leds;
  LEDManager::registerWrite();
}

void LEDManager::set(byte confID, byte val){
  /*
    Serial.print("\tConfID:\t");
    Serial.print(confID);
    Serial.print("\tval:\t");
    Serial.print(val);
    Serial.print("\tnbLeds:\t");
    Serial.print(ArduConf00::nbLeds[confID]);
    Serial.print("\tleftShift:\t");
    Serial.print(leftShift(confID));
  */
  // first clear the bits concerned
  unsigned int mask = (unsigned int)((1<<ArduConf00::nbLeds[confID])-1) << (unsigned int)leftShift(confID);
  /*
  Serial.print("\tunshiftedMask:\t");
  Serial.print((unsigned int)((1<<ArduConf00::nbLeds[confID])-1));
  Serial.print("\tMask:\t");
  Serial.print(mask,BIN);
  Serial.print("\tLedArray:\t");
  Serial.println(LEDManager::ledArray,BIN);
  */
  LEDManager::ledArray &= ~mask;
  // now or the val in
  mask = val << leftShift(confID);
  LEDManager::ledArray |= mask;
  //Serial.print("Calling LEDManager::registerWrite:\t" );
  //Serial.println(ledArray,BIN);
  LEDManager::registerWrite();
}
