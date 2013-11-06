#include <Actuator.h>

ArduStomp *Actuator::as;
long Actuator::lastActionTime;
boolean Actuator::allOK;

Actuator::Actuator(byte buttonPin, 
		   byte l0,
		   byte cI,
		   State *sPtr,
		   stateFunc sfPtr, 
		   actionFunc afPtr): led0(l0),
				      confI(cI),
				      s(sPtr),
				      sf(sfPtr),
				      af(afPtr) {
  db = new DebounceButton(buttonPin);
}

void Actuator::doMsg(byte confID, byte val){
  char buf[ArduConf00::msgLenNbSettings[confID][0]*ArduConf00::wordLen];
  ArduConf00::getMsg(confID,val,buf);
  for (byte b = 0 ;
       allOK && (b < ArduConf00::msgLenNbSettings[confID][0]);
       b++){
    allOK = as->com->enqueueMsg(buf+b);
  }
}
  

// FIX IDs !!
boolean Actuator::stdAction(){
  if (db->pressed()){
    byte curState = s->val,
      nextState =  (s->*sf)();
    if (curState != nextState){
      doMsg(confI, nextState);
      LEDManager::set()
      return true;
    }
  }
  return false;
}

boolean Actuator::presetsAction(){
  return true;
}
boolean Actuator::autoAction(){
  return true;
}

boolean Actuator::update(){
  if (allOK && (millis() - lastActionTime > MIN_TIME_BETWEEN_BUTTON_PRESSES)){
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
			      6, // led index
			      ArduConf00::neckI,
			      State::neckState,
			      &State::toggle,
			      &Actuator::stdAction);
  // middle actuator
  actuators[1] = new Actuator(M_PIN, // button pin
			      7, // led index
			      1, //  1 led
			      ArduConf00::middleI,
			      State::middleState,
			      &State::toggle,
			      &Actuator::stdAction);
  // bridge actuator
  actuators[2] = new Actuator(B_PIN, // button pin
			      14, // led index
			      2, //  2 leds
			      ArduConf00::bridgeI,
			      State::bridgeState,
			      &State::circularInc,
			      &Actuator::stdAction);
  // vol UP actuator
  actuators[3] = new Actuator(VUP_PIN, // button pin
			      1, // led index
			      3, //  led
			      ArduConf00::volI,
			      State::volState,
			      &State::inc,
			      &Actuator::stdAction);
  // vol DOWN actuator
  actuators[4] = new Actuator(VDW_PIN,
			      1, // led index
			      5, // 5 led
			      ArduConf00::volI,
			      State::volState,
			      &State::dec,
			      &Actuator::stdAction);
  // tone UP actuator
  actuators[5] = new Actuator(TUP_PIN, // button pin
			      9, // led index
			      5, // 5 led
			      ArduConf00::toneI,
			      State::toneState,
			      &State::inc,
			      &Actuator::stdAction);
  // tone DOWN actuator
  actuators[6] = new Actuator(TDW_PIN,
			      9, // led index
			      5, // 5 led
			      ArduConf00::toneI,
			      State::toneState,
			      &State::dec,
			      &Actuator::stdAction);
  // preset actuator
  actuators[7] = new Actuator(P_PIN,
			      17, // led index
			      4, // only 1 led
			      5, // not an AutoConf00 elt
			      State::presetsState,
			      &State::circularInc,
			      &Actuator::presetsAction);
  // auto actuator
  actuators[AUTO_ACT] = new Actuator(A_PIN,
				     21, // led index
				     1, // only 1 led
				     5, // not an AutoConf00 elt
				     State::autoState,
				     &State::toggle,
				     &Actuator::autoAction);  
  lastActionTime = millis();
  allOK = true;
}
