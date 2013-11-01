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
  head = new cycler(QLEN-1);
  tail = new cycler(QLEN-1);
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
  setElt(tail->getVal(),next);
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
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->getVal()][i];
  }
  resetElt(head->getVal());
  head->inc();
  return true;
}

boolean  outQueueStatic::pQ(char* buf) const{
  if (eltCount == 0){
    return false;
  }
  for (byte i = 0; i<ELEN;i++){
    buf[i] = queue[head->getVal()][i];
  }
  return true;
}
  
  

