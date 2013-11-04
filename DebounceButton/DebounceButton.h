/* DebounceButton.h
 * maintains a single debounced button
 */

#ifndef DEBOUNCEBUTTON_H
#define DEBOUNCEBUTTON_H

#include <Arduino.h>

#define DEBOUNCEDELAY (30)

class DebounceButton {
private:
  byte buttonPin,
    buttonState,     
    lastButtonState; 

  // the following variables are long's because the time, 
  // measured in miliseconds, will quickly become a bigger 
  // number than can be stored in an int.  
  long lastDebounceTime;  

  static const byte debounceDelay = DEBOUNCEDELAY;    
  // the debounce time; increase if the output flickers
  
 public:
  DebounceButton(byte pin);
  boolean pressed();  
};

#endif

