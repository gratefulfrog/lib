// short test of cylcer and biInc classes

#include <cycler.h>

#define BBB (4)

void setup(){
  Serial.begin(115200);
  while(!Serial);
}

biInc b(BBB);
cycler c(BBB);

char d = 1;
byte last = b.getVal();

void loop(){
  Serial.print("c: ");
  Serial.print(c.inc());
  Serial.print("    b: ");
  Serial.println(b.inc(d));
  if (b.getVal() == last){
    d=-d;
  }
  last = b.getVal();
  delay(1000);
}
    
