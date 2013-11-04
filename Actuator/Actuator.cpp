#include <Actuator.h>

Actuator::Actuator(byte buttonPin, 
		   byte l0,
		   byte nbL,
		   State *sPtr,
		   stateFunc sfPtr, 
		   actionFunc afPtr): led0(l0),
				      nbLeds(nbL),
				      s(sPtr),
				      sf(sfPtr),
				      af(afPtr) {
  db = new DebounceButton(buttonPin);
}

boolean Actuator::stdAction(){
  // temp for tests
  
  return true;
}

boolean Actuator::update(){
  if (millis() - lastActionTime > MIN_TIME_BETWEEN_BUTTON_PRESSES){
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
			      1, //  1 led
			      State::neckState,
			      &State::toggle,
			      &Actuator::stdAction);
  // middle actuator
  actuators[1] = new Actuator(M_PIN, // button pin
			      7, // led index
			      1, //  1 led
			      State::middleState,
			      &State::toggle,
			      &Actuator::stdAction);
  // bridge actuator
  actuators[2] = new Actuator(B_PIN, // button pin
			      14, // led index
			      2, //  2 leds
			      State::bridgeState,
			      &State::circularInc,
			      &Actuator::stdAction);
  // vol UP actuator
  actuators[3] = new Actuator(VUP_PIN, // button pin
			      1, // led index
			      5, // 5 led
			      State::volState,
			      &State::inc,
			      &Actuator::stdAction);
  // vol DOWN actuator
  actuators[4] = new Actuator(VDW_PIN,
			      1, // led index
			      5, // only 1 led
			      State::volState,
			      &State::dec,
			      &Actuator::stdAction);
  // tone UP actuator
  actuators[5] = new Actuator(TUP_PIN, // button pin
			      9, // led index
			      5, // 5 led
			      State::toneState,
			      &State::inc,
			      &Actuator::stdAction);
  // tone DOWN actuator
  actuators[6] = new Actuator(TDW_PIN,
			      9, // led index
			      5, // only 1 led
			      State::toneState,
			      &State::dec,
			      &Actuator::stdAction);
  // preset actuator
  actuators[7] = new Actuator(P_PIN,
			      17, // led index
			      4, // only 1 led
			      State::presetsState,
			      &State::circularInc,
			      &Actuator::stdAction);
  // auto actuator
  actuators[AUTO_ACT] = new Actuator(A_PIN,
				     21, // led index
				     1, // only 1 led
				     State::autoState,
				     &State::toggle,
				     &Actuator::stdAction);  
  lastActionTime = millis();
}
