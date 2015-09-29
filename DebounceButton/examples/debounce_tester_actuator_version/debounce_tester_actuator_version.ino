// -*-c++-*-
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
#define A_PIN   (11)    

DebounceButton *buttons[NB_ACTUATORS];

byte Pins[] = {VUP_PIN,
               VDW_PIN,
               TUP_PIN,
               TDW_PIN,
               N_PIN,
               M_PIN,
               B_PIN,
               P_PIN,
               A_PIN};

void setup(){
  Serial.begin(115200);
  while(!Serial);
  for (byte b=0;b<NB_ACTUATORS;b++){
    buttons[b] =  new DebounceButton(Pins[b]);
  }
}

void loop(){
  for (byte b=0;b<NB_ACTUATORS;b++){
    if (buttons[b]->pressed()){
       Serial.print("Pressed:\t");
       Serial.println(b);
    }
  }
}
