#include <Arduino.h>
#include <DebounceButton.h>

/* test circuit:
 * a button is connected to a pin such that when pressed 
 * the pin reads HIGH.
 * the button pressed connects 1&3, 2&4.
  1   3
  | B |
  2   4
 * 1:  not connected
 * 2:  +5V
 * 3:  pin(7)
 * 4: 10K resistor to GND
 * be sure to run the serial monitor at 115200 baud
 */

#define BUTTON_PIN (7)
#define LED_PIN (13)

DebounceButton b(BUTTON_PIN);
boolean ledState = false;

void setup(){
  Serial.begin(115200);
  while(!Serial);
  pinMode(LED_PIN,OUTPUT);
}

void loop(){
  bool tempLedState = ledState;
  ledState = b.pressed() ? !ledState : ledState;
  if (tempLedState !=ledState){
     Serial.println("Pressed!");
  }
  if (ledState){
    digitalWrite(LED_PIN,HIGH);
  }
  else{
    digitalWrite(LED_PIN,LOW);
  }
}
