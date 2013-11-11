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
  //Serial.print("Sizeof buff\t");
  //Serial.println(sizeof(buf));
  if (!ArduConf00::getMsg(confID,val,buf)){
    return;
  }
  /*Serial.print("outgoing buff from Actuator::doMsg\t");
  for (byte b=0;b<sizeof(buf);b++){
    Serial.print(buf[b]);
  }
  Serial.println();
  */
  //byte nbSettings = ArduConf00::bufLenNbSettings(confID, true);
  //byte nbMessages = sizeof(buf)/ArduConf00::wordLen;
  //Serial.print("\tnbMessages\t");
  //Serial.println(nbMessages);
  for (byte b = 0 ; 
       //allOK && (b < nbSettings);
       allOK && (b < sizeof(buf)/ArduConf00::wordLen);
       b++){
    allOK = as->com->enqueueMsg(&buf[b*ArduConf00::wordLen]);
    /*Serial.print("\nEnqueing:\t");
    for (byte c=0;c<5;c++){
      Serial.print((buf+b*ArduConf00::wordLen)[c]);
    }
    */
    //Serial.print("\tallOK:\t");
    //Serial.println(allOK);
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

boolean Actuator::bridgeAction(){
  //Serial.print("on call to bridgeAction, state:\t");
  //byte curS =s->val;
  //Serial.println(curS);
  byte nextState =  (s->*sf)();
  doMsg(confID, nextState);
  if (allOK){
    LEDManager::set(confID,
		    ArduConf00::bridgeState2LedVal(nextState));
  }
  return allOK;
}

boolean Actuator::presetsAction(){
  //Serial.print("Actuator::presetsAction: current state:\t");
  //byte curState = s->val;
  //Serial.print(curState);
  as->curPresetIndex = (s->*sf)();
  //Serial.print("\tnext state:\t");
  //Serial.println(as->curPresetIndex);
  //LEDManager::set(confID,as->curPresetIndex);
  as->doPreset();
  freeRam();
  return true;
}
 
boolean Actuator::saveAction(){
  if (! as->p){
    return false;
  }
  // vol, tone, neck, middle are easy
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::volKey,
		     &State::volState->val,
		     true);
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::toneKey,
		     &State::toneState->val,
		     true);
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::neckKey,
		     &State::neckState->val,
		     true);
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::middleKey,
		     &State::middleState->val,
		     true);
  // bridge is a little harder 
  // bridgeState 0 ->
  // bridgeState 1 -> north(0),both(1)
  // bridgeState 2 -> north(1),both(0)
  byte bNorthVal = State::bridgeState->val == 2 ? 1 : 0,
    bBothVal = State::bridgeState->val == 1 ? 1 : 0;
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::bridgeNorthKey,
		     &bNorthVal,
		     true);
  as->p->presetValue(as->curPresetIndex, 
		     PresetClass::bridgeBothKey,
		     &bBothVal,
		     true);
  // then flash the leds!!
  LEDManager::flashLeds();
  return true;
  
}

boolean Actuator::autoAction(){
  byte nextState =  (s->*sf)();
  as->a->start(boolean(nextState));
  LEDManager::set(confID,nextState);
  //freeRam();
  return true;
}

void Actuator::autoOff(){
  actuators[AUTO_ACT]->s->val = 0;
  as->a->start(false);
  LEDManager::set(ArduConf00::autoID,0);
}
    
boolean Actuator::update(){
  if (allOK && 
      (millis() - lastActionTime > MIN_TIME_BETWEEN_BUTTON_PRESSES) &&
      af !=NULL &&
      db->pressed()){
    if (confID != ArduConf00::autoID){
      as->autoOff();
    }
    lastActionTime = millis();
    //Serial.print("inside if of Actuator::update, confID:\t");
    //Serial.print(confID);
    //Serial.println("\tdb->pressed()!");
    return (this->*af)();
  }
  return false;
}

Actuator *Actuator::actuators[NB_ACTUATORS];

void Actuator::init(ArduStomp *ass){
  //Serial.println("Actuator::init");
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
			      //&Actuator::stdAction);
                              &Actuator::bridgeAction);
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
  // save preset actuator
  actuators[9] = as->a ? new Actuator(S_PIN,
				      ArduConf00::saveID,
				      NULL,
				      NULL,
				      &Actuator::saveAction) : NULL;  

  lastActionTime = millis();
  allOK = true;
}
