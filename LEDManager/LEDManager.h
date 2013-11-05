/* LEDManager.h
 * this is the library class that controls the leds
 * in the ArduStompBox
 */

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>
#include <ArduConf00.h>

// for led shift register shifting
//Pin connected to latch pin (ST_CP) of 74HC595
#define LATCHPIN (13) 
//Pin connected to clock pin (SH_CP) of 74HC595
#define CLOCKPIN (12)
////Pin connected to Data in (DS) of 74HC595
#define DATAPIN (11)

class LEDManager {

private:
  // why not map the leds to one unsigned int? 16 bits!
  static unsigned int ledArray;
  static const byte confToLedID[] ;
  static const byte nbLeds[]; // nb of leds for the id in index
  static void registerWrite();
  static byte leftShift(byte ledID);

public:
  static const byte  neckID = 0,
    middleID  = 1,
    bridgeID  = 2,
    presetID  = 3,
    autoID    = 4,
    volID     = 5,
    toneID    = 6,
    powerID   = 7,
    connectID = 8;
  
  static void init();
  static void zeroAll();
  static void set(byte ledID, byte val);
  static void localSet(byte ledID, byte val);
};

#endif
