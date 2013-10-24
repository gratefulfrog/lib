#include <ArduCom.h>

boolean procReply(char *reply, char *sent, int replySize){
  // This is part of the ArduCom protocol suite.
  // if it starts with e, then not ok!
  // otherwise, if any of the chars do not match the last sent, i.e. the top of the queue, 
  // then not ok.
  if(reply[0] == 'e'){
    return false;
  }
  boolean ret = true;
  for (int i=1; i< replySize;i++){
    if (sent[i-1] != reply[i]){
      ret = false;
      break;
    }
  }
  return ret;
}

///////////////////////////////////////////////////////////////////////////
///////////////////  Human Monitoring Stuff ///////////////////////////////
///////////////////////////////////////////////////////////////////////////
 void ArduCom::msg(String s) {
  // send stuff to the serial terminal for human observation
  Serial.println(s);
}

void  ArduCom::msg(String s, char *c, int len)  {
  // send stuff to the serial terminal for human observation
  // allows a string plus a un-terminated char array to be sent
  char buff[len+1];
  for (int i=0;i< len;i++){
    buff[i]=  c[i];
  }
  buff[len] = '\0';
  msg (s + buff);
}

void ArduCom::respond(){
  //port->write((byte*)responseBuffer,responseSize);
  for (int i=0;i<responseSize;i++){
    port->write(msgBuffer[i]);
  } 
} 

void ArduComSlave::executeMsgString(){
  // after each atomic incoming msg, reply
  (*rFunc)(msgBuffer,NULL,msgSize);
  respond();
}
  

ArduCom::ArduCom(HardwareSerial *p,
		 ArduComResponseFunc f, 
		 int mSz): port(p), msgSize(mSz), responseSize(mSz+1){
  msgBuffer = new char[responseSize];
  initialized = false;
  rFunc = f;
}

void ArduCom::clearIncoming(){
  while (port->read() != -1);
}

ArduComSlave::ArduComSlave(HardwareSerial *p,
			   ArduComResponseFunc f, 
			   int mSz): ArduCom::ArduCom(p,f,mSz){
  currentCharCount = 1;
}

void ArduComSlave::doInit() {
  // setup the port
  port->begin(baudRate);
  while(!*port);
  // send initChar and wait for response
  while(!initialized){
    port->write(initChar);
    delay(100);
    if(port->available()>0){
      char c = port->read();
      if (c !=initChar){
        msgBuffer[currentCharCount++] = c;
      }      
      initialized = true;
    }
  }
}

void ArduComSlave::stepLoop(){
  //processIncomingAtom();
  if (currentCharCount == responseSize) {
    executeMsgString();
    currentCharCount = 1;
  }

  //readIntoAtom();
  if (port->available() > 0) {
    // read one char and add it to the buffer if ok:
    char c = port->read();
    if (c !=initChar){
      msgBuffer[currentCharCount++] = c;
    }
  }
}

ArduComMaster::ArduComMaster(HardwareSerial *p, 
			     int mSz): ArduCom::ArduCom(p,&procReply,mSz){
  q = new outQueue();
  atomAckedOnPort = false;
  currentCharCount = 0;
}

void ArduComMaster::enqueueMsg(char *msg){
  q->enQ(msg);
}
char* ArduComMaster::peekQ(){
  return q->pQ();
}

void ArduComMaster::doInit(){
  // wait for the other Arduino to send a char, if it is the initChar, then we can start
  // so we are "initialized" and the last sent atom has been acked, since there are none
  // if not just ignore the char as garbage... and pause for a short while.
  port->begin(baudRate);
  while(!*port);
  while(!initialized){
    if (port->available()>0){
      char c = port->read();
      if (c==initChar){
	atomAckedOnPort = initialized= true;
	port->write(initChar);
	delay(initDelay);
      }
    }
  }
}

void ArduComMaster::sendAtom(){
  // send the raw char array, and set atomAckedOnPort to false, 
  // so that the Queue won't be popped until we get the ACK
  char *m = q->pQ();
  if (m != NULL){
    port->write((byte*)m,msgSize);
    msg("Sent atom: ",m,msgSize);
    atomAckedOnPort =false;
  }
}

void ArduComMaster::executeMsgString(){
  // We have received an incoming atom from Serial1,
  // if it is ok, then we confirm ack and can pop the queue 
  // if it is not ok, then we do not pop the queue and do not set the ACK
  // in any event, we try to send the atom currently at the top of the queue, 
  if((*rFunc)(msgBuffer, q->pQ(),msgSize)){
    char *c = q->deQ();
    msg("dq'd: ",c,msgSize);
    atomAckedOnPort = true;
  }
  // so it's ok and we moved on, or it's not ok and we are still at the same on
  sendAtom();  // try to send the next one, or the last one again 
}

void ArduComMaster::stepLoop(){
  // do one of the following:
  // if the current incoming atom is complete, then process it,
  // or, if there is something to read on Serial1, add it to the current incoming atom,
  // or, if it is ok to send something, then send the atom at the head of the queue
  //processIncomingAtom();
  if (currentCharCount == responseSize) {
    msg("Rec'd reply: ",msgBuffer,responseSize);
    executeMsgString();
    currentCharCount = 0;
  }
  else if (port->available()>0){
    char c = port->read();
    if (c != initChar){
      msgBuffer[currentCharCount++] = c;
    }
    else{
      port->write(initChar);  // to clear an init call!
    }
    msg("Rec'd a char : ",&c,1);
  }
  else if (atomAckedOnPort){
    // try to send the top of the queue, NOT only to prime the pump, 
    // because:
    // at init, there is nothing on Serial1,
    // but we consider that no atoms sent have been Acked, so atomAckedOnSerial1 is true,
    // when we send an Atom on Serial1, acked goes to false,
    // then if we have read an atom or if we are reading, we don't get here
    // and if we have read an atom, then acked may go to true and the queue is popped,
    // which would imply that the top sent, if there is one! 
    // But what if there is nothing in the queue? then, we return to the initial state and
    // this branch of the if will get called if an outgoing atom is enqueued.
    sendAtom();  
  }
}
