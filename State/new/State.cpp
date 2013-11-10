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
LinearState::LinearState(byte mV): State(mV){}
CircularState::CircularState(byte mV): State(mV){}
ToggleState::ToggleState(byte mV): State(mV){}

State *State::neckState,
  *State::middleState,
  *State::bridgeState,
  *State::volState,
  *State::toneState,
  *State::presetsState,
  *State::autoState,
  *State::states[7];

void State::init(){
  states[0] = neckState    =  new ToggleState();
  states[1] = middleState  =  new ToggleState();
  states[2] = bridgeState  =  new CircularState(2);
  states[5] = volState     =  new LinearState(5);
  states[6] = toneState    =  new LinearState(5);
  states[3] = presetsState =  new CircularState(3);
  states[4] = autoState    =  new ToggleState();
}

byte LinearState::inc(char dir){
  if (dir>0){
    return val = (val == maxVal ? maxVal :val+incVal);
  }
  return val = (val == minVal ? minVal :val-incVal);
}

byte CircularState::inc(char){
  return val = (val == maxVal ? State::minVal : val+State::incVal);
}

byte ToggleState::inc(char){
  return val = (val == State::minVal ? maxVal : minVal);
}





