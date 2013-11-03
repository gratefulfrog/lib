#include <ArduConf00.h>

const byte ArduConf00::offOn[] = {0,255},
  ArduConf00::neckPin          = 2,
  ArduConf00::middlePin        = 3,
  ArduConf00::bridgePins[]     = {4,5},  // bridge, split
  ArduConf00::volPins[]        = {9,10,12},
  ArduConf00::tonePin          = 11,
  ArduConf00::bridgeSettings[][AC_BRIDGE_SETTINGS]  = {{0, 255, 255},
						       {0,   0, 255}},
  ArduConf00::volSettings[][AC_VOL_SETTINGS] = {{0,  12,14,18,27,255},  // volPWM[0]-> volPins[0]
						{255,30,20,15,13,  0},  // volPWM[1]-> volPins[1]
						{0,   0, 0, 0, 0,255}}, // 3rd vactrol for max volume: volPWM[2]-> volPins[2]
  ArduConf00::toneSettings[]        = {255,90,46,27,17,  0};

const byte   *ArduConf00::pinPtr[]  = {&ArduConf00::neckPin,  // neck pin
				       &ArduConf00::middlePin, // middle pin
				       &ArduConf00::bridgePins[0],  // fbridge both pin
				       &ArduConf00::volPins[0],    // 1st vol pin
				       &ArduConf00::tonePin};      // tone pin
const byte  *ArduConf00::valPtr[] = { &ArduConf00::offOn[0],  // values for neck start here
				      &ArduConf00::offOn[0],  // values for middle start here
				      &ArduConf00::bridgeSettings[0][0],  // values for bridge Pins start here
				      &ArduConf00::volSettings[0][0],  // values for vol pins start here
				      &ArduConf00::toneSettings[0]};  // values of tone settings

const byte ArduConf00::msgLenNbSettings[][2] = {{AC_NECK_PINS,AC_NECK_SETTINGS},  // neck 1 pin, 2 settings
						{AC_MIDDLE_PINS,AC_MIDDLE_SETTINGS},  // middle 1 pin, 2 settings
						{AC_BRIDGE_PINS,AC_BRIDGE_SETTINGS},  // bridge 2 pins, 3 settings
						{AC_VOL_PINS,AC_VOL_SETTINGS},  // vol  3 pins, 6 settings
						{AC_TONE_PINS,AC_TONE_SETTINGS}}; // tone 1 pin, 6 settings
const byte  ArduConf00::nbControls =  5,
  ArduConf00::controlIDs[] = {0,1,2,3,4}; // neck, middle, bridge, vol, tone
  

void ArduConf00::b2a(byte pin, byte val, char *buf){
  // fill the buffer which must be the right length (5) with 
  // the chars to make the message
  buf[0] = 48+ pin/10;
  buf[1] = 48+ pin%10;
  buf[2] = 48+ val/100;
  buf[3] = 48+ (val%100)/10;
  buf[4] = 48+ val%10;
}

void ArduConf00::getMsg(byte id, byte v, char *buf){ 
  //buf must be right lenght exactly;
  // id values come from controlIDs in this class
  // values are numerical integers (byte size) as follows
  // neck, middle : [0,1]
  // bridge       : [0, 1, 2]
  // vol, tone    :  [0, 1, 2, 3, 4, 5]
  for (byte b=0;b<msgLenNbSettings[id][0];b++){ 
    const byte *pin = pinPtr[id];
    //Serial.print("Pin start: ");
    //Serial.println(*pin);
    pin += b; 
    //Serial.print("Pin end: ");    
    //Serial.println(*pin);
    const byte *val = valPtr[id];
    //Serial.print("Val start: ");
    //Serial.println(*val);
    val += v + b*msgLenNbSettings[id][1]; // for (byte cc= 0;cc <v;cc++,val++); // += (const byte*)b;
    //Serial.print("Val end: ");
    //Serial.println(*val);
    b2a(*pin,*val,&buf[b*wordLen]);
  }
}


/*  Test results from ~/lib/ArduConfOO/examples/arduconf00_tester/arduconf00_tester.ino

Control ID: 0 Value: 0 msg: 02000
Control ID: 0 Value: 1 msg: 02255
Control ID: 1 Value: 0 msg: 03000
Control ID: 1 Value: 1 msg: 03255
Control ID: 2 Value: 0 msg: 0400005000
Control ID: 2 Value: 1 msg: 0425505000
Control ID: 2 Value: 2 msg: 0425505255
Control ID: 3 Value: 0 msg: 090001025512000
Control ID: 3 Value: 1 msg: 090121003012000
Control ID: 3 Value: 2 msg: 090141002012000
Control ID: 3 Value: 3 msg: 090181001512000
Control ID: 3 Value: 4 msg: 090271001312000
Control ID: 3 Value: 5 msg: 092551000012255
Control ID: 4 Value: 0 msg: 11255
Control ID: 4 Value: 1 msg: 11090
Control ID: 4 Value: 2 msg: 11046
Control ID: 4 Value: 3 msg: 11027
Control ID: 4 Value: 4 msg: 11017
Control ID: 4 Value: 5 msg: 11000

*/
