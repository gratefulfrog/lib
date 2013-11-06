/* ArduConf00.h
 * a general purpose value holder class
 * such that Zero is the min
 * max is assgined const at instatiation
 * inc can take positive and negative values but always incs by the step of One
 */
 
/* from data.tsv
 * name	  vol	tone	neck	middle	bNorth	bBoth
 * Rock	  6	11	0	1	0	1
 * Woman  10	4	1	0	0	1
 * Jazz	  11	1	1	0	0	0
 * Comp	  8	11	0	1	0	1
 * Auto	  0	0	0	0	0	0
 *
 * from conf.pde
 * volPins[] = {"09","10","12"},
 * tonePin   = "11",
 * selectorPins[]   = {"02", //neck
                       "03", // middlePin
		       "05",  // splitPin
	               "04"}, //bridgePin
 * onOff[] = {"000", "255"}, // 0, 1
 * // vol levels: [0,1,2,3,4,5]
   volPWM[][]  = {{"000","012","014","018","027","255"},
                   // volPWM[0]-> volPins[0]
		  {"255","030","020","015","013","000"},
	           // volPWM[1]-> volPins[1]
		  {"000","000","000","000","000","255"}},  
	           // 3rd vactrol for max volume: volPWM[2]-> volPins[2]
 * tonePWM[] = {"255","090","046","027","017","000"};
                // tone levels: [0,1,2,3,4,5]
 */
 
#ifndef ARDUCONF00_H
#define ARDUCONF00_H

#include <Arduino.h>

#define AC_NECK_PINS (1)
#define AC_MIDDLE_PINS (1)
#define AC_BRIDGE_PINS (2)
#define AC_VOL_PINS (3)
#define AC_TONE_PINS (1)

#define AC_NECK_SETTINGS (2)
#define AC_MIDDLE_SETTINGS (2)
#define AC_BRIDGE_SETTINGS (3)
#define AC_VOL_SETTINGS (6)
#define AC_TONE_SETTINGS (6)

#define AC_NB_MAPS (5)

class ArduConf00 {
private:
  static const byte nbPinsSetings[][2],
    offOn[], 
    neckPin,  
    middlePin, 
    bridgePins[], 
    volPins[],
    tonePin,  
    bridgeSettings[][AC_BRIDGE_SETTINGS],
    volSettings[][AC_VOL_SETTINGS], 
    toneSettings[],
    IDMap[][3],
    msgLenNbSettings[][2],
    *pinPtr[],
    *valPtr[],
    wordLen = 5; // a word is 5 chars, no terminator!
  static void b2a(byte pin, byte val, char *buf);
  static boolean mapExtID(byte extID, byte *result, boolean mapPreset = false);
  
public:
  static const byte nbLeds[], // nb of leds for the id in index
    neckID = 0,
    middleID  = 1,
    bridgeID  = 2,
    presetID  = 3,
    autoID    = 4,
    volID     = 5,
    toneID    = 6,
    powerID   = 7,
    connectID = 8;
  static byte bufLenNbSettings(byte extID, boolean settings = false);
  static void getMsg(byte extID, byte val, char *buf);
};

#endif
