/* LEDManager.cpp
 * this is the library class that controls the leds
 * in the ArduStompBox
 */

#include <LEDManager.h>

unsigned int LEDManager::ledArray = 0;

const byte LEDManager::confToLedID[] = {neckID,
					middleID,
					bridgeID,
					volID,
					toneID};
  
const byte LEDManager::nbLeds[] =
  {1, // neck led
   1, // middle led
   2, // bridge leds  00 01 10
   3, // presets leds in binary !!!
   1, // auto led
   // end of first byte
   3, // vol leds in binary !!!
   3, // tone leds in binary !!!
   1, // power led
   1}; // connect led
   // end of second byte!

// This method sends bits to the shift registers:
void LEDManager::registerWrite() {
  // turn off the output so the leds don't light up
  // while you're shifting bits:
  digitalWrite(LATCHPIN, LOW);

  //Serial.println("outgoing[0] = " + String((byte)(LEDManager::ledArray >> 8)));
  //Serial.println("outgoing[1] = " + String((byte)(LEDManager::ledArray & 0B11111111)));  
  
  shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, (byte)(LEDManager::ledArray >> 8));
  shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, (byte)(LEDManager::ledArray & 0B11111111));
  
  // turn on the output so the LEDs can light up:
  digitalWrite(LATCHPIN, HIGH);
}

byte LEDManager::leftShift(byte ledID) {
  byte ret = 0;
  for (byte b = 0; b<ledID;b++,ret+= nbLeds[b]);
  return ret;
}

void LEDManager::init(){
  pinMode(LATCHPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);  
  pinMode(CLOCKPIN, OUTPUT);
  LEDManager::zeroAll();
}

void LEDManager::zeroAll(){
  LEDManager::ledArray = 0;
  LEDManager::registerWrite();
}

void LEDManager::set(byte confID, byte val){
  LEDManager::localSet(LEDManager::confToLedID[confID],val);
}

void LEDManager::localSet(byte ledID, byte val){
  // first clear the bits concerned
  unsigned int mask = ((byte)(pow(2,nbLeds[ledID])-1)) << leftShift(ledID);
  LEDManager::ledArray &= ~mask;
  // now or the val in
  mask = val << leftShift(ledID);
  LEDManager::ledArray |= mask;
  LEDManager::registerWrite();
}

/*
// This method sends bits to the shift registers:
void OLDregisterWrite() {
  // turn off the output so the leds don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  byte outgoing[] = {0,0,0};
  for (int i =0;i<24;i++){
    if (leds[i]){
      outgoing[int(i/8)] |= 1 <<(7 - (i%8));
    }
  }

  //Serial.println("outgoing[0] = " + String(outgoing[0]));
  //Serial.println("outgoing[1] = " + String(outgoing[1]));  
  //Serial.println("outgoing[2] = " + String(outgoing[2]));  
  
  for (int i = 2;i>-1 ;i--){
    shiftOut(dataPin, clockPin, LSBFIRST, outgoing[i]);
  }
  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);
}


const int ledArraySize =24;
boolean leds[ledArraySize];

// these pairs say the LED starting index and how many there are in the button control group
const byte volLedIndex[2] = {1,5},   //{1,3}   
  neckLedIndex[2] = {6,1},           //{4,1}
  middleLedIndex[2] = {7,1},         //{5,
  toneLedIndex[2] = {9,5},   
  bridgeLedIndex[2] = {14,2},   
  presetLedIndex[2] = {17,4},   
  autoLedIndex[2] = {21,1},   
  powerLedIndex[2] = {22,1},   
  connectLedIndex[2] = {23,1};

const int *indexLis[] = {  volLedIndex,
                           neckLedIndex,
                           middleLedIndex,
                           toneLedIndex,
                           bridgeLedIndex,  
                           presetLedIndex,
                           autoLedIndex,
                           powerLedIndex,  
                           connectLedIndex};



*/
