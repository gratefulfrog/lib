// ardustomp_tester.ino

#include <Arduino.h>
#include <ArduConf00.h>
#include <State.h>
#include <DebounceButton.h>
#include <outQueueStatic.h>
#include <ArduComOptStatic.h>
#include <RN42autoConnectOpt.h>
#include <SD.h>
#include <SDReader.h>
#include <Actuator.h>
#include <ArduStomp.h>

int freeRam (){
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void setup(){
  ArduStomp::init();
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Free Ram: ");
  Serial.println(freeRam());

}

void loop(){}
