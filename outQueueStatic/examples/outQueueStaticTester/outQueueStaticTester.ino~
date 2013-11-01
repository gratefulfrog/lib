/* outQueueTester.ino
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

#include <outQueue.h>

#define NBTESTS  5
#define MOTLIMIT 3

outQueue *q;

char *mot = new char[q->eLen];  // a place to store words read on Serial
int incomingCount = 0,          // how many chars we read on Serial
    motCount=0;                 // how many words we read on Serial
    
void  printTop(){
   char *cc = q->pQ();
   if(cc != NULL){
     char c[q->eLen+1];
     for (int j=0;j<q->eLen;j++){
        c[j]=cc[j];
      }
      c[q->eLen] = '\0';
    Serial.println(c);
 }
}
void  printLast(){
   char *cc = q->nthQ(motCount-1);
   if(cc != NULL){
     char c[q->eLen+1];
     for (int j=0;j<q->eLen;j++){
        c[j]=cc[j];
      }
      c[q->eLen] = '\0';
    Serial.println(c);
 }
}  
void printAll(){
   char *cc = q->deQ();
 while(cc != NULL){
   char c[q->eLen+1];
   for (int j=0;j<q->eLen;j++){
      c[j]=cc[j];
    }
    c[q->eLen] = '\0';
    Serial.println(c);
    cc = q->deQ();
 }
}

//////////////////////
//// std functions ///
//////////////////////

void setup(){
  Serial.begin(115200);
  while(!Serial);
  delay(5);
  Serial.println("test");
  char s[] = "12345",
       p[] = "abcde";
 q = new outQueue();
 for (int i=0;i<NBTESTS;i++){
   q->enQ(p);
   Serial.println("enqd p");
   q->enQ(s);
   Serial.println("enqd s");
 }
 printAll();
}

void loop() {
  if(Serial.available()>0){
    mot[incomingCount++] = Serial.read();
  }
  if (incomingCount == q->eLen) {
    q->enQ(mot);
    incomingCount=0;
    motCount++;
    printTop();
    printLast();
  }
  if (motCount == MOTLIMIT){
    printAll();
    motCount=0;
  }
}
