/* ArduStomp.cpp
 * this is the library class that is the top level for the 
 * ArduStompBox
 */

#include <ArduStomp.h>

ArduStomp *ArduStomp::as;

ArduStomp::ArduStomp(ArduComOptStaticMaster *cc,
		     PresetClass *pp,
		     AutoClass   *aa): 
  com(cc),
  p(pp),
  a(aa){
  curPresetIndex = 0;
}

void ArduStomp::init(){
  RN42autoConnectOpt(&Serial1).setupRN42AndConnect();
  delay(5);
  //Serial.println("rn42 auto connect ok!");
  ArduComOptStaticMaster *_c = new ArduComOptStaticMaster(&Serial1,
							  ARDUCOMOPTSTATIC_MSGSIZE);
  delay(5);
  State::init();
  //Serial.println("c instatiated");
  _c->doInit();
  // read the presets or not
  PresetClass *_p = new PresetClass(PFILE);
  //Serial.print("preset class pointer: ");
  //Serial.println((int)_p);
  if(!_p->parse()){
    delete _p;
    _p = NULL;
  }
  //Serial.print("preset class pointer after parse: ");
  //Serial.println((int)_p);  

  // read the auto or not
  AutoClass *_a = new AutoClass(AFILE,_p);
  //Serial.print("auto class pointer: ");
  //Serial.println((int)_a);
  if (!_a->parse()){
    delete _a;
    _a = NULL;
  }
  //Serial.print("auto class pointer after parse: ");
  //Serial.println((int)_a);
  as = new ArduStomp(_c,_p,_a);
}

void ArduStomp::doPreset(){
  //Serial.println("entering: ArduStomp::doPreset()");
  
  if (p == NULL) {
    return;
  }
  LEDManager::zeroAll();
  
  for (byte key=0; key<(PresetClass::nbKeys-2);key++){ // not the bridge key
    byte confVal, 
      confID;
    p->presetValue(curPresetIndex,key,&confVal);
    if(ArduConf00::mapExtID(key, &confID, true)){    
      Actuator::doMsg(confID, confVal);
      if (Actuator::allOK){
	State::states[confID]->val=confVal;
	// set state!!! here!!!!
	LEDManager::set(confID, confID<2 ? ArduConf00::pickupState2LedVal(confVal) : confVal);
      }
    }
  } 
  // now do the bridge:
  // if brdigeNorth -> val is 0B10, ie 2, stateVal=2
  // else if BridgeBoth -> ledval is 0B11 ie 3, stateVal = 1
  // else val is 0
  byte bridgeStateVal =  0,
    val;
  //if(p->presetValue(curPresetIndex, PresetClass::bridgeNorthKey)) {
  p->presetValue(curPresetIndex, PresetClass::bridgeNorthKey,&val);
  if(val) {
    bridgeStateVal =  2;
  }
//else if(p->presetValue(curPresetIndex, PresetClass::bridgeBothKey)){
  else {
    p->presetValue(curPresetIndex, PresetClass::bridgeBothKey,&val);
    if (val){
      bridgeStateVal =  1;
    }
  }
  Actuator::doMsg(ArduConf00::bridgeID, bridgeStateVal);
  if (Actuator::allOK){
    State::states[ArduConf00::bridgeID]->val=bridgeStateVal;
    LEDManager::set(ArduConf00::bridgeID, 
		    ArduConf00::pickupState2LedVal(bridgeStateVal));
    State::states[ArduConf00::presetID]->val=curPresetIndex;
    LEDManager::set(ArduConf00::presetID,curPresetIndex+1);
  }
  if (a && a->running){
    // turn on its led
    LEDManager::set(ArduConf00::autoID,1);
  }
}

void ArduStomp::checkAuto(){
  if (a && a->running){
    byte nextPreset =  a->check();
    if (curPresetIndex != nextPreset){
      curPresetIndex = nextPreset;
      doPreset();
    }
  }
}

void ArduStomp::stepAlarm() {
  LEDManager::setAll();
  delay(ALARM_PAUSE);
  LEDManager::zeroAll();
  delay(ALARM_PAUSE);
}

void ArduStomp::autoOff(){
  if (a && a->running){
    Actuator::autoOff();
  }
}
