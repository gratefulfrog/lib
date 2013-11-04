// State.h  State Class used to control states of all ArduGuitar
// Controllable elts:
// neckState
// middleState
// BridgeState
// VolState
// ToneState
// presetsState
// autoState

#ifndef STATECLASS_H
#define STATECLASS_H

#include <Arduino.h>

class State {
  protected:  
    static const byte minVal = 0,
                      incVal = 1;
    const byte maxVal;
    State(byte maxV=1); // min is zero and start is zero
    
  public:
    static State  *neckState,
      *middleState,
      *bridgeState,
      *volState,
      *toneState,
      *presetsState,
      *autoState;

    static void init();
    byte val;
    byte inc();
    byte circularInc();
    byte dec();
    byte toggle();
};

#endif


