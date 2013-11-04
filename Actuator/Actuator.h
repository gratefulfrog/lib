/* Actuator.h
 * called at each loop iteration to check button and act on it
 * there is one actuator instance per button.
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H
#define DEBOUNCEDELAY (30)

class deBounceButton {
private:
  byte buttonPin,
    buttonState,     // = LOW,   // the current reading from the input pin
    lastButtonState; // = LOW;   // the previous reading from the input pin
  // the following variables are long's because the time, 
  //measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  
  long lastDebounceTime;  //  = 0;  // the last time the output pin was toggled
  static const byte debounceDelay = DEBOUNCEDELAY;    
  // the debounce time; increase if the output flickers
    
public:
  deBounceButton(byte pin);
  boolean pressed();  
};

typedef void (*doerFunPtr)();
typdef   byte (*State::stateFunc)();

class actuatorClass {
private:
  const doerFunPtr df;
  deBounceButton   db;
  State s;
  stateFunc sf; // inc, dec, or toggle
  byte led0,
    nbLeds;
    
  public:
  actuator(byte pin, stateFun sfptr, doerFunPtr dfptr);
  bool update();  // returns true if something has been done
};
#endif
