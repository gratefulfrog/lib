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
  *State::autoState;

void State::init(){
  neckState    =  new State();
  middleState  =  new State();
  bridgeState  =  new State(2);
  volState     =  new State(5);
  toneState    =  new State(5);
  presetsState =  new State(3);
  autoState    =  new State();

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





