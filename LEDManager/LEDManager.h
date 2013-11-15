// -*- c++ -*-
/* LEDManager.h
 * this is the library class that controls the leds
 * in the ArduStompBox
 */

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>
#include <ArduConf00.h>
#include <SPI.h>

#define NB_LED_FLASHES  (3)
#define LED_FLASH_DELAY  (200)

// for led shift register shifting
#define LATCHPIN (13)   // Pin connected to latch pin (ST_CP) of 74HC595
//#define CLOCKPIN (12)   // Pin connected to clock pin (SH_CP) of 74HC595
//#define DATAPIN (11)    // Pin connected to Data in (DS) of 74HC595

class LEDManager {

private:

  static void registerWrite();
  static byte leftShift(byte ledID);

public:
  static void init();
  //static void zeroAll(boolean reallyAll = false); // only for special cases!
  static void zeroAll(); // 
  static void setAll();
  static void flashLeds();
  static void set(byte confID, byte val);
  static unsigned int ledArray;  // led states map to one unsigned int 16 bits!
};

#endif
