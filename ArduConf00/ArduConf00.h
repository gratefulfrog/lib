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

class ArduConf00 {
 private:
  static const byte nbPinsSetings[][2];

  static const byte offOn[], // = {0,255},
    neckPin,  // = 2,
    middlePin, // = 3,
    bridgePins[], // = {4,5},  // bridge, split
    volPins[], //    = {9,10,12},
    tonePin,  //      = 11,
    bridgeSettings[][AC_BRIDGE_SETTINGS], /* = {{0, 255, 255}, // for bridge pins
			    {0,   0, 255}} */
    volSettings[][AC_VOL_SETTINGS], /* = {{0,  12,14,18,27,255},  // volPWM[0]-> volPins[0]
			{255,30,20,15,13,  0},  // volPWM[1]-> volPins[1]
			{0,   0, 0, 0, 0,255}}, // 3rd vactrol for max volume: volPWM[2]-> volPins[2] */
    toneSettings[]; //  =  {255, 90,46,27,17,  0};
    
  static const byte *pinPtr[] ; /* = {&neckPin,  // neck pin
				   &middlePin, // middle pin
				  &bridgePins[0],  // first bridge pin
				  &volPins[0],    // first vol pin
				  &tonePin};      // tone pin
				*/
  static const byte *valPtr[] ; /* = { &offOn[0],  // values for neck start here
				  &offOn[0],  // values for middle start here
				  &bridgeSettings[0][0],  // values for bridge Pins start here
				  &volSettings[0][0],  // values for vol pins start here
				  &toneSettings[0]};  // values of tone settings
			       */
  static void b2a(byte pin, byte val, char *buf);
  // fill the buffer which must be the right length (5) with 
  // the chars to make the message

 public:
  static const byte msgLenNbSettings[][2];

  static const byte  nbControls,
    controlIDs[]; // {0,1,2,3,4}; // neck, middle, bridge, vol, tone

  static const byte wordLen = 5, // a word is 5 chars, no terminator!
    neckI   = 0,
    middleI = 1,
    bridgeI = 2,
    volI    = 3,
    toneI   = 4;  
  
  static const byte presetFileToConfMap[];

  static  void getMsg(byte id, byte val, char *buf);
  // buf must be right lenght exactly;
  // id values come from controlIDs in this class
  // values are numerical integers (byte size) as follows
  // neck, middle : [0,1]
  // bridge       : [0, 1, 2]
  // vol, tone    :  [0, 1, 2, 3, 4, 5]

};

#endif
