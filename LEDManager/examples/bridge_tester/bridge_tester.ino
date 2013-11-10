//  -*-C++-*-

#include <Arduino.h>
#include <ArduConf00.h>
#include <LEDManager.h>
#include <SPI.h>

byte cur = 0,
  state[] ={0,1,2};
char *msgVal[] = {"0400005000",
		  "0425505000",
		  "0425505255"};

void setup(){
  LEDManager::init();
  Serial.begin(115200);
  while(!Serial);
  delay(5000);
}

void loop(){
  if(Serial.read()>0){
    Serial.print("State:\t");
    Serial.print(state[cur]);
    Serial.print("\tledVal:\t");
    byte ledVal = ArduConf00::bridgeState2LedVal(state[cur]); //==1 ? 3: state[cur];  
    Serial.print(ledVal);
    Serial.print("\tMessage:\t");
    Serial.print(msgVal[cur]);
    Serial.print("\tArduConf00::getMsg:\t");
    char buf[11];
    buf[10] = '\0';
    ArduConf00::getMsg(ArduConf00::bridgeID,state[cur],buf);
    Serial.println(buf);
    LEDManager::zeroAll();
    LEDManager::set(ArduConf00::bridgeID, ledVal);
    cur = (cur +1) % 3;
  }
}


/* results 2013 11 10
It should look like this:

State    LedVal    Msg          Semantic
0        0         0400005000   none
1        3         0425505000   both
2        2         0425505255   North Only

This is what we got:
State:	0	ledVal:	0	Message:	0400005000	ArduConf00::getMsg:	0400005000
State:	1	ledVal:	3	Message:	0425505000	ArduConf00::getMsg:	0425505000
State:	2	ledVal:	2	Message:	0425505255	ArduConf00::getMsg:	0425505255
State:	0	ledVal:	0	Message:	0400005000	ArduConf00::getMsg:	0400005000
State:	1	ledVal:	3	Message:	0425505000	ArduConf00::getMsg:	0425505000
State:	2	ledVal:	2	Message:	0425505255	ArduConf00::getMsg:	0425505255
State:	0	ledVal:	0	Message:	0400005000	ArduConf00::getMsg:	0400005000
State:	1	ledVal:	3	Message:	0425505000	ArduConf00::getMsg:	0425505000
State:	2	ledVal:	2	Message:	0425505255	ArduConf00::getMsg:	0425505255
State:	0	ledVal:	0	Message:	0400005000	ArduConf00::getMsg:	0400005000

With Visual Confirmation, AFTER UPDATE OF WIRED CONNECTIONS !!!

 */
