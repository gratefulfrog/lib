// -*- c++ -*-
#include <State.h>


void printState(int s){
  Serial.print("State:\t");
  Serial.print(s);
  Serial.print("\tVal:\t");
  Serial.println(State::states[s]->val);
}

void setup(){
  Serial.begin(115200);
  while(!Serial);
  State::init();
  delay(5000);
}

State *ss;
LinearState *ls = new LinearState(5);
CircularState *cs = new CircularState(3);
ToggleState *ts = new ToggleState(1);

void loop(){
  ss = ls;
  Serial.println("\nLinear State: INC");
  for (int i=0;i<15;i++){
    Serial.print(ss->INC);
    Serial.print(" ");
  }
  Serial.println("\nLinear State: DEC");
  for (int i=0;i<15;i++){
    Serial.print(ss->DEC);
    Serial.print(" ");
  }
  ss = cs;
  Serial.println("\nCircular State: INC");
  for (int i=0;i<15;i++){
    Serial.print(ss->INC);
    Serial.print(" ");
  }
  Serial.println("\nCircular State: DEC");
  for (int i=0;i<15;i++){
    Serial.print(ss->DEC);
    Serial.print(" ");
  }
  ss = ts;
  Serial.println("\nToggle State: INC");
  for (int i=0;i<15;i++){
    Serial.print(ss->INC);
    Serial.print(" ");
  }
  Serial.println("\nToggle State: DEC");
  for (int i=0;i<15;i++){
    Serial.print(ss->DEC);
    Serial.print(" ");
  }
  delay(10000);
}

