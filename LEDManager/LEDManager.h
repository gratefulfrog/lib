/* LEDManager.h
 * this is the library class that controls the leds
 * in the ArduStompBox
 */

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>
#include <ArduConf00.h>
#include <SPI.h>

// for led shift register shifting
#define LATCHPIN (13)   // Pin connected to latch pin (ST_CP) of 74HC595
#define CLOCKPIN (12)   // Pin connected to clock pin (SH_CP) of 74HC595
#define DATAPIN (11)    // Pin connected to Data in (DS) of 74HC595

class LEDManager {

private:
  static unsigned int ledArray;  // why not map led states to one unsigned int? 16 bits!
  static void registerWrite();
  static byte leftShift(byte ledID);

public:
  static void init();
  static void zeroAll();
  static void set(byte confID, byte val);
  // for debug
  static unsigned int getArray() { return ledArray;};
};

#endif
