#include <outQueueStatic.h>

void outQueueStatic::resetElt(byte index){
  for(byte j = 0;j<ELEN;j++){
    queue[index][j] = '\0';
  }
}

void outQueueStatic::setElt(byte index, char *val){
  for (byte b=0;b<ELEN;b++){
    queue[index][b] = val[b];
  }
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
  if (eltCount == QLEN){  // Queue is full!
    return false; // and do nothing
  }
  eltCount++;
  //msg("outQueueStatic::enQ, enqued\t",next,5);  
  Serial.print("\noutQueueStatic::enQ\tQ elt count:\t");
  Serial.println(eltCount);
  setElt(tail->val,next);
  tail->inc();
  return true;
}

boolean outQueueStatic::deQ(char *buf){
  // true if something to deQ,
  // copy from head to buf, then zero buf, and inc head
  if (eltCount == 0){
    return false;
  }
  eltCount--;
  //Serial.print("outQueueStatic::deQ\tQ elt count:\t");
  //Serial.println(eltCount);
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->val][i];
  }
  resetElt(head->val);
  head->circularInc();
  //msg("outQueueStatic::deQ, dequed\t",buf,5);
  Serial.print("\noutQueueStatic::deQ\tQ elt count:\t");
  Serial.println(eltCount);
  return true;
}

boolean  outQueueStatic::pQ(char* buf) const{
  if (eltCount == 0){
    return false;
  }
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->val][i];
  }
  Serial.print("outQueueStatic::pQ\teltCount:\t");
  Serial.print(eltCount);
  //msg("outQueueStatic::pQ, peeked\t",buf,5);

  return true;
}
  
  

