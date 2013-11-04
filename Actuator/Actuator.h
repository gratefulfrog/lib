/* Actuator.h
 * called at each loop iteration to check button and act on it
 * there is one actuator instance per button.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduiono.h>
#include <DebounceButton.h>
#include <State.h>


#define NB_ACTUATORS (9)

#define MIN_TIME_BETWEEN_BUTTON_PRESSES (100)

#define VUP_PIN (2)
#define VDW_PIN (3)
#define TUP_PIN (4)
#define TDW_PIN (5)
#define N_PIN   (6)
#define M_PIN   (7)
#define B_PIN   (8)
#define P_PIN   (9)
#define A_PIN   (A0)

class Actuator;

typedef boolean (Actuator::*actionFunc)();
typedef   byte (State::*stateFunc)();

class Actuator {
private:
  static const byte nbActuators =  NB_ACTUATORS;
  static long lastActionTime;
  const actionFunc af;
  DebounceButton   *db;
  State *s;
  const stateFunc sf; // inc, dec, or toggle
  const byte led0,
    nbLeds;
  Actuator(byte buttonPin, 
	   byte l0,
	   byte nbL,
	   State *sPtr,
	   stateFunc sfPtr, 
	   actionFunc afPtr);
  boolean stdAction();

 public:
  static Actuator *actuators[NB_ACTUATORS];
  void init();
  boolean update();  // returns true if something has been done
};
#endif
