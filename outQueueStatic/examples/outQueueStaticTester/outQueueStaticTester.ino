/* outQueueStaticTester.ino
 * a simple test of the outQueue class:
 * on setup():
 * - create a q,
 * - print "test" to Serial
 * - enqueue NBTESTS times "12345" and "abcde" while printing msgs to Serial
 * - print all the q while dqing.
 * in the loop():
 * - read words of 5 chars from Serial
 * - at each read of a 5 char word, enq it, and print a peek at 
 *   the head and the tail
 * - after MOTLIMIT number of words, print all dqing each,
 * This should demonstrate the various possible things the q can do.
 */

#include <cycler.h>
#include <outQueueStatic.h>

#define NBTESTS  45
#define MOTLIMIT 10

outQueueStatic q;

char mot[ELEN];  // a place to store words read on Serial
int incomingCount = 0,          // how many chars we read on Serial
    motCount=0;                 // how many words we read on Serial

char cc[ELEN+1];
    
void  printTop(){
   if(q.pQ(cc)){
     Serial.print("Head of the Q: ");
     Serial.println(cc);
   }
   else{
     Serial.println("printtop failed");
   }
}
void printAll(){
  Serial.println("Here's the whole Q!");
  while(q.deQ(cc)){
    Serial.println(cc);
  }
}
int freeRam (){
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//////////////////////
//// std functions ///
//////////////////////

void setup(){
  cc[ELEN] = '\0';
  Serial.begin(115200);
  while(!Serial);
  delay(5);
  Serial.println("test begins");
  Serial.print("Free Ram: ");
  Serial.println(freeRam());
  for (int i=0;i<NBTESTS;i++){
    char ingoing[ELEN] = {'0','0','0','0','0'};
    ingoing[ELEN-1] = 48+i%10;
    ingoing[ELEN-2] = 48+(i/10) %10;
    ingoing[ELEN-3] = 48+(i/100) %100;
    if(q.enQ(ingoing)){
      Serial.print("enqd: " );
    }
    else{
      Serial.print("failed to enq: ");
    }
    for (int j=0;j<ELEN;j++){
      Serial.write(ingoing[j]);
    }
    Serial.println();
  }
  Serial.print("Free Ram: ");
  Serial.println(freeRam());
  printAll();
  Serial.print("Free Ram: ");
  Serial.println(freeRam());
}

void loop() {
  if(Serial.available()>0){
    mot[incomingCount++] = Serial.read();
  }
  if (incomingCount == ELEN) {
    q.enQ(mot);
    incomingCount=0;
    motCount++;
    printTop();
    Serial.print("Free Ram: ");
    Serial.println(freeRam());
  }
  if (motCount == MOTLIMIT){
    printAll();
    Serial.print("Free Ram: ");
    Serial.println(freeRam());
    motCount=0;
  }
}
