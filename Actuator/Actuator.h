/* Actuator.h
 * called at each loop iteration to check button and act on it
 * there is one actuator instance per button.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduiono.h>
#include <DebounceButton.h>
#include <State.h>
#include <ArduConf00.h>
#include <ArduStomp.h>
#include <outils.h>

#define NB_ACTUATORS (9)
#define AUTO_ACT (8)

#define MIN_TIME_BETWEEN_BUTTON_PRESSES (100)

#define VUP_PIN (2)
#define VDW_PIN (3)
#define TUP_PIN (4)
#define TDW_PIN (5)
#define N_PIN   (6)
#define M_PIN   (7)
#define B_PIN   (8)
#define P_PIN   (9)
#define A_PIN   (11)    // CHANGED 2013 11 06 to fit new 16bit led array

class Actuator;
class ArduStomp;

typedef boolean (Actuator::*actionFunc)();
typedef   byte (State::*stateFunc)();

class Actuator {
private:
  static long lastActionTime;
  const actionFunc af;
  DebounceButton   *db;
  State *s;
  const stateFunc sf; // inc, dec, or toggle
  const byte confID;  // the id in the conf class
  Actuator(byte buttonPin,
	   byte cID,
	   State *sPtr,
	   stateFunc sfPtr, 
	   actionFunc afPtr);
  boolean stdAction();
  boolean bridgeAction();
  boolean presetsAction();
  boolean autoAction();

 public:
  static boolean allOK;
  static const byte nbActuators =  NB_ACTUATORS;
  static Actuator *actuators[NB_ACTUATORS];
  static ArduStomp *as;
  static void init(ArduStomp *ass);
  static void doMsg(byte confID, byte val);
  boolean update();  // returns true if something has been done
};
#endif
