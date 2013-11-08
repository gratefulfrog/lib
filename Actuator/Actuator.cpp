#include <Actuator.h>

ArduStomp *Actuator::as;
long Actuator::lastActionTime;
boolean Actuator::allOK = true;

Actuator::Actuator(byte buttonPin, 
		   byte cID,
		   State *sPtr,
		   stateFunc sfPtr, 
		   actionFunc afPtr): confID(cID),
				      s(sPtr),
				      sf(sfPtr),
				      af(afPtr) {
  db = new DebounceButton(buttonPin);
}

void Actuator::doMsg(byte confID, byte val){
  char buf[ArduConf00::bufLenNbSettings(confID)];
  if (!ArduConf00::getMsg(confID,val,buf)){
    return;
  }
  byte nbSettings = ArduConf00::bufLenNbSettings(confID, true);
  for (byte b = 0 ; 
       allOK && (b < nbSettings);
       b++){
    allOK = as->com->enqueueMsg(buf+b);
  }
}

boolean Actuator::stdAction(){
  byte curState = s->val,
    nextState =  (s->*sf)();
  if (curState != nextState){
    doMsg(confID, nextState);
    if (allOK){
      LEDManager::set(confID,nextState);
    }
    return allOK;
  }
  return false;
}

boolean Actuator::presetsAction(){
  as->curPresetIndex = (s->*sf)();
  as->doPreset();
  return true;
}
boolean Actuator::autoAction(){
  byte nextState =  (s->*sf)();
  as->a->start((boolean)nextState);
  LEDManager::set(confID,nextState);
  return true;
}
    
boolean Actuator::update(){
  if (allOK && 
      (millis() - lastActionTime > MIN_TIME_BETWEEN_BUTTON_PRESSES) &&
      af !=NULL &&
      db->pressed()){
    lastActionTime = millis();
    return (this->*af)();
  }
  return false;
}

Actuator *Actuator::actuators[NB_ACTUATORS];
void Actuator::init(ArduStomp *ass){
  as = ass;
  // neck actuator
  actuators[0] = new Actuator(N_PIN,  // button pin
			      ArduConf00::neckID,
			      State::neckState,
			      &State::toggle,
			      &Actuator::stdAction);
  // middle actuator
  actuators[1] = new Actuator(M_PIN, // button pin
			      ArduConf00::middleID,
			      State::middleState,
			      &State::toggle,
			      &Actuator::stdAction);
  // bridge actuator
  actuators[2] = new Actuator(B_PIN, // button pin
			      ArduConf00::bridgeID,
			      State::bridgeState,
			      &State::circularInc,
			      &Actuator::stdAction);
  // vol UP actuator
  actuators[3] = new Actuator(VUP_PIN, // button pin
			      ArduConf00::volID,
			      State::volState,
			      &State::inc,
			      &Actuator::stdAction);
  // vol DOWN actuator
  actuators[4] = new Actuator(VDW_PIN,
			      ArduConf00::volID,
			      State::volState,
			      &State::dec,
			      &Actuator::stdAction);
  // tone UP actuator
  actuators[5] = new Actuator(TUP_PIN, // button pin
			      ArduConf00::toneID,
			      State::toneState,
			      &State::inc,
			      &Actuator::stdAction);
  // tone DOWN actuator
  actuators[6] = new Actuator(TDW_PIN,
			      ArduConf00::toneID,
			      State::toneState,
			      &State::dec,
			      &Actuator::stdAction);
  // preset actuator
  actuators[7] = as->p ? new Actuator(P_PIN,
				      ArduConf00::presetID,
				      State::presetsState,
				      &State::circularInc,
				      &Actuator::presetsAction) : NULL;
  // auto actuator
  actuators[AUTO_ACT] = as->a ? new Actuator(A_PIN,
					     ArduConf00::autoID,
					     State::autoState,
					     &State::toggle,
					     &Actuator::autoAction) : NULL;  
  lastActionTime = millis();
  allOK = true;
}
