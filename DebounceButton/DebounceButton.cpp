/* DebounceButton.cpp
 * maintains a single debounced button
 */

#include <DebounceButton.h>
/*
class DebounceButton {
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
*/


DebounceButton::DebounceButton(byte pin){
  buttonPin = pin;
  buttonState = lastButtonState = LOW;
  lastDebounceTime = 0;
  pinMode(buttonPin,INPUT);
}  

boolean DebounceButton::pressed(){
  /* record state temporarily as 'reading'
   * if state has changed, reset 'last time of change'
   * if enough time has passed with no change of state,
   * then the temp  state is the current state.
   * if the previous state was HIGH, and current is LOW
   * then return value is TRUE, false otherwise.
   * set the last button state to the temp reading
   */

  byte reading = digitalRead(buttonPin);
  boolean ret = false;
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      //ret = (buttonState == LOW) && (lastButtonState == HIGH);
      ret = (buttonState == HIGH);
    }
  }
  lastButtonState = reading;
  
  return  ret;
}


