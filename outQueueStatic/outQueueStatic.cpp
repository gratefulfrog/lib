#include <outQueueStatic.h>

void outQueueStatic::resetElt(byte index){
  for(byte j = 0;j<ELEN;j++){
    queue[index][j] = '\0';
  }
}

void outQueueStatic::setElt(byte index, char *val){
  Serial.print("oQS::setElt:\t");
  for (byte b=0;b<ELEN;b++){
    Serial.print(val[b]);
    queue[index][b] = val[b];
  }
  Serial.println();
  //delay(5);
}

outQueueStatic::outQueueStatic(){
  head = new State(QLEN-1);
  tail = new State(QLEN-1);
  eltCount = 0;
  for (byte i = 0;i<QLEN;i++){
    resetElt(i);
  }
}
  
boolean outQueueStatic::enQ(char *next){
  // returns true if success, false if queue is full
  Serial.print("\noutQueueStatic::enQ\tQ elt count:\t");
  Serial.println(eltCount);
  //delay(5);
  if (eltCount == QLEN){  // Queue is full!
    return false; // and do nothing
  }
  eltCount++;
  setElt(tail->val,next);
  tail->circularInc();
  return true;
}

boolean outQueueStatic::deQ(char *buf){
  // true if something to deQ,
  // copy from head to buf, then zero buf, and inc head
  Serial.print("\noutQueueStatic::deQ\tQ elt count:\t");
  Serial.println(eltCount);
  //delay(5);
  if (eltCount == 0){
    return false;
  }
  eltCount--;
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->val][i];
  }
  resetElt(head->val);
  head->circularInc();
  return true;
}

boolean  outQueueStatic::pQ(char* buf) const{
  //Serial.print("outQueueStatic::pQ\teltCount:\t");
  //Serial.println(eltCount);
  //delay(5);
  if (eltCount == 0){
    return false;
  }
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->val][i];
  }
  return true;
}
