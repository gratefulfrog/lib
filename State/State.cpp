// State.h  State Class used to control states of all ArduGuitar
// Controllable elts:
// neckState
// middleState
// BridgeState
// VolState
// ToneState
// presetsState
// autoState

#include <State.h>

State::State(byte maxV): maxVal(maxV), val(0){}

State *State::neckState,
  *State::middleState,
  *State::bridgeState,
  *State::volState,
  *State::toneState,
  *State::presetsState,
  *State::autoState,
  *State::states[7];

void State::init(){
  states[0] = neckState    =  new State(); 
  states[1] = middleState  =  new State(); 
  states[2] = bridgeState  =  new State(2);
  states[3] = autoState    =  new State();
  states[4] = presetsState =  new State(3);
  states[5] = toneState    =  new State(5);
  states[6] = volState     =  new State(5);
  //states[5] = volState     =  new State(5);
  //states[6] = toneState    =  new State(5);
  //states[3] = presetsState =  new State(3);
  //states[4] = autoState    =  new State();
  /*
    neckState;
    middleState,
    bridgeState,
    presetsState,
    autoState,
    volState,
    toneState
  */
}
byte State::inc(){
  return val = (val == maxVal ? maxVal : val+incVal);
}
byte State::dec(){
  return val = (val == minVal ? minVal : val-incVal);
}
byte State::circularInc(){
  return val = (val == maxVal ? minVal : val+incVal);
}
byte State::toggle(){
  return val = (val == minVal ? maxVal : minVal);
}





