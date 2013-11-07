//  -*-C++-*-

#include <LEDManager.h>
#include <ArduConf00.h>
#include <SPI.h>

void setup(){
  LEDManager::init();
  Serial.begin(115200);
  while (!Serial);
  delay(5000);
  for (byte i = 0;i<ArduConf00::connectID+1; i++){
    Serial.print("Ext ID:\t");
    Serial.println(i);
    LEDManager::zeroAll();
    byte lim = 0;
    if(i==3){
      lim = 4;
    }
    else if (i==4||i==7||i==8){
      lim = 2;
    }
    for (byte ii=0;ii<lim;ii++){
        Serial.print("Control ID:\t");
        Serial.print(i);
        Serial.print("\tValue:\t");
        Serial.print(ii);
        LEDManager::set(i,ii);
        Serial.print("\tLedArray:\t");
        Serial.println(ledArray,BIN);
      }
    for (byte ii = 0; ii<ArduConf00::bufLenNbSettings(i,true); ii++){
      Serial.print("Control ID:\t");
      Serial.print(i);
      Serial.print("\tValue:\t");
      Serial.print(ii);
      LEDManager::set(i,ii);
      Serial.print("\tLedArray:\t");
      Serial.print(ledArray,BIN);
      
      byte chars = ArduConf00::bufLenNbSettings(i) +1;
      if (chars >1){
        char buf[chars];
        //Serial.print("\tChars:\t");
        //Serial.println(chars);
        ArduConf00::getMsg(i,ii,buf);
        buf[chars-1] = '\0';
        Serial.print("\tmsg:\t");
        Serial.println(buf);
      }
    }
    Serial.println();
  }
}

void loop(){
}

/* results 2013 11 06 :
Ext ID:	0
Control ID:	0	Value:	0	LedArray:	0	msg:	02000
Control ID:	0	Value:	1	LedArray:	1	msg:	02255

Ext ID:	1
Control ID:	1	Value:	0	LedArray:	0	msg:	03000
Control ID:	1	Value:	1	LedArray:	2	msg:	03255

Ext ID:	2
Control ID:	2	Value:	0	LedArray:	0	msg:	0400005000
Control ID:	2	Value:	1	LedArray:	4	msg:	0425505000
Control ID:	2	Value:	2	LedArray:	8	msg:	0425505255

Ext ID:	3
Control ID:	3	Value:	0	LedArray:	0
Control ID:	3	Value:	1	LedArray:	16
Control ID:	3	Value:	2	LedArray:	32
Control ID:	3	Value:	3	LedArray:	48

Ext ID:	4
Control ID:	4	Value:	0	LedArray:	0
Control ID:	4	Value:	1	LedArray:	128

Ext ID:	5
Control ID:	5	Value:	0	LedArray:	0	msg:	090001025512000
Control ID:	5	Value:	1	LedArray:	256	msg:	090121003012000
Control ID:	5	Value:	2	LedArray:	512	msg:	090141002012000
Control ID:	5	Value:	3	LedArray:	768	msg:	090181001512000
Control ID:	5	Value:	4	LedArray:	1024	msg:	090271001312000
Control ID:	5	Value:	5	LedArray:	1280	msg:	092551000012255

Ext ID:	6
Control ID:	6	Value:	0	LedArray:	0	msg:	11255
Control ID:	6	Value:	1	LedArray:	2048	msg:	11090
Control ID:	6	Value:	2	LedArray:	4096	msg:	11046
Control ID:	6	Value:	3	LedArray:	6144	msg:	11027
Control ID:	6	Value:	4	LedArray:	8192	msg:	11017
Control ID:	6	Value:	5	LedArray:	10240	msg:	11000

Ext ID:	7
Control ID:	7	Value:	0	LedArray:	0
Control ID:	7	Value:	1	LedArray:	16384

Ext ID:	8
Control ID:	8	Value:	0	LedArray:	0
Control ID:	8	Value:	1	LedArray:	32768


 */
