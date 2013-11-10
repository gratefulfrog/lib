// -*- c++ -*-
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
#define INC inc(1)
#define DEC inc(-1)
#define TOGGLE inc(1)

class State {
protected:  
  static const byte minVal = 0,
    incVal = 1;
  const byte maxVal;
  State(byte mxV=1); // min is zero and start is zero  

public:
  static State  *neckState,
    *middleState,
    *bridgeState,
    *volState,
    *toneState,
    *presetsState,
    *autoState,
    *states[];
  
  static void init();
  byte val;
  virtual byte inc(char) =0;
};

class LinearState: public State {
public:
  LinearState(byte mV=1);
  virtual byte inc(char direction);
};

class CircularState: public State {
public:
  CircularState(byte mV=1);
  virtual byte inc(char);
};

class ToggleState: public State {
public:
  ToggleState(byte mV=1);
  virtual byte inc(char);
};

#endif


