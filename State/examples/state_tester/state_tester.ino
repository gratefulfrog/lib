#include <State.h>


void printAll(){
  Serial.print("neck  : ");
  Serial.println(State::neckState->val);
  Serial.print("middle: ");
  Serial.println(State::middleState->val);
  Serial.print("bridge: ");
  Serial.println(State::bridgeState->val);
  Serial.print("vol   : ");
  Serial.println(State::volState->val);
  Serial.print("tone  : ");
  Serial.println(State::toneState->val);
  Serial.println();
}

void incAll(){
  State::neckState->inc();
  State::middleState->inc();
  State::bridgeState->inc();
  State::volState->inc();
  State::toneState->inc();
}
void decAll(){
  State::neckState->dec();
  State::middleState->dec();
  State::bridgeState->dec();
  State::volState->dec();
  State::toneState->dec();
}

void toggleAll(){
  State::neckState->toggle();
  State::middleState->toggle();
  State::bridgeState->toggle();
  State::volState->toggle();
  State::toneState->toggle();
}
  
void setup(){
  Serial.begin(115200);
  while(!Serial);
  State::init();
  delay(5000);
  Serial.println("Looping...");
}

void loop(){
  Serial.println("\nStarting...");
  printAll();
  delay(1500);
  Serial.println("\nINC...");
  for (int i=0 ; i < 6; i++){
    incAll();
    printAll();
    delay(1500);
  }
  Serial.println("\nDEC...");
  for (int i=0 ; i < 6; i++){
    decAll();
    printAll();
    delay(1500);
  }
  Serial.println("\nTOBGGLE...");
  for (int i=0 ; i < 6; i++){
    toggleAll();
    printAll();
    delay(1500);
  }

}
