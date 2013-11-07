//  -*-C++-*-

#include <Arduino.h>
#include <State.h>
#include <SD.h>
#include <SDReader.h>
#include <outQueueStatic.h>
#include <DebounceButton.h>
#include <Actuator.h>
#include <ArduStomp.h>
#include <ArduComOptStatic.h>
#include <RN42autoConnectOpt.h>
#include <ArduConf00.h>


int freeRam (){
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Free RAM: ");
  Serial.println(freeRam());
  ArduStomp::init();
  Actuator::init(ArduStomp::as);
  Serial.print("Free RAM: ");
  Serial.println(freeRam());
}

void loop(){}
