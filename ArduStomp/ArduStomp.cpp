/* ArduStomp.cpp
 * this is the library class that is the top level for the 
 * ArduStompBox
 */

#include <ArduStomp.h>

ArduStomp *ArduStomp::as;
LEDManager *ArduStomp::lm;

ArduStomp::ArduStomp(ArduComOptStaticMaster *cc,
		     PresetClass *pp,
		     AutoClass   *aa): 
  com(cc),
  p(pp),
  a(aa){
  curPresetIndex = 0;
}

void ArduStomp::init(){
  lm = new LEDManager();
  pinMode(LATCHPIN,OUTPUT);
  pinMode(CLOCKPIN,OUTPUT);
  pinMode(DATAPIN,OUTPUT);
  RN42autoConnectOpt(&Serial1).setupRN42AndConnect();
  //msg("rn42 auto connect ok!");
  ArduComOptStaticMaster *_c = new ArduComOptStaticMaster(&Serial1,
							  ARDUCOMOPTSTATIC_MSGSIZE);
  //msg("c instatiated");
  _c->doInit();
  // read the presets or not
  PresetClass *_p = new PresetClass(PFILE);
  //Serial.print("preset file id: ");
  //Serial.println((int)p);
  if(!_p->parse()){
    delete _p;
    _p = NULL;
  }
  // read the auto or not
  AutoClass *_a = new AutoClass(AFILE,_p);
  //Serial.print("auto file id: ");
  //Serial.println((int)a);
  if (!_a->parse()){
    delete _a;
    _a = NULL;
  }
  as = new ArduStomp(_c,_p,_a);
  as->doPreset()  
}

void ArduStomp::doPreset(){
  if (p == NULL) {
    return;
  }
  LEDManager::zeroAll();
  for (byte key=0; b<SDReader::nbKeys-2;key++){ // not the bridge key
    byte confVal = p->presetValue(curPresetIndex,key);
    Actuator::doMsg(ArduConf00::presetFileToConfMap[key], confVal);
    lm->set(LEDManager::Leds[ArduConf00::presetFileToConfMap[key]], confVal);
  }
  // now do the bridge:
  byte bridgeConfVal = p->presetValue(curPresetIndex, PresetClass::bridgeNorthKey) +
    p->presetValue(curPresetIndex, PresetClass::bridgeNorthKey);
  Actuator::doMsg(ArduConf00::bridgeI, bridgeConfVal);
  lm->set(LEDManager::Leds[ArduConf00::bridgeI], bridgeConfVal);
}
  

void ArduStomp::checkAuto(){
  byte nextPreset =  a->check();
  if (curPresetIndex != nextPreset){
    curPresetIndex = nextPreset;
    doPreset();
  }
}

void ArduStomp::stepAlarm() {
  lm->set(LEDManager::powerID,1);
  delay(ALARM_PAUSE);
  lm->set(LEDManager::powerID,0);
  delay(ALARM_PAUSE);
}

void ArduStomp::autoOff(){
  if (a && a->running){
    Actuator::actuators[AUTO_ACT]->update();
  }
}
