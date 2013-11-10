#include <ArduComOptStatic.h>

boolean procReply(char *reply, char *sent, byte replySize){
  // This is part of the ArduCom protocol suite.
  // it is used by the Master, only!
  // if it starts with e, then not ok!
  // otherwise, if any of the chars do not match the last sent, i.e. the top of the queue, 
  // then not ok.
  //ArduComOptStatic::msg("Received reply: ",reply,replySize);
  msg("Received reply: ",reply,replySize);
  if(reply[0] == ARDUCOMOPTSTATIC_ERRORCHAR){
    return false;
  }
  boolean ret = true;
  for (byte i=1; i< replySize;i++){
    if (sent[i-1] != reply[i]){
      ret = false;
      break;
    }
  }
  return ret;
}

/* DEBUG: all this section
///////////////////////////////////////////////////////////////////////////
///////////////////  Human Monitoring Stuff ///////////////////////////////
///////////////////////////////////////////////////////////////////////////
 void ArduComOptStatic::msg(String s) {
  // send stuff to the serial terminal for human observation
  Serial.println(s);
}

void  ArduComOptStatic::msg(String s, char *c, int len)  {
  // send stuff to the serial terminal for human observation
  // allows a string plus a un-terminated char array to be sent
  char buff[len+1];
  for (int i=0;i< len;i++){
    buff[i]=  c[i];
  }
  buff[len] = '\0';
  msg (s + buff);
}
*/

//////////////////////////////////////////////////////////
/////////////////  ArduComOptStatic        /////////////////////
//////////////////////////////////////////////////////////


void ArduComOptStatic::respond(){
  port->write((byte*)msgBuffer,responseSize);
}
 
ArduComOptStatic::ArduComOptStatic(HardwareSerial *p,
				   ArduComOptStaticResponseFunc f, 
				   byte mSz): port(p), 
					      msgSize(mSz), 
					      responseSize(mSz+1),
					      initialized(false),
					      rFunc(f){
  msgBuffer = new char[responseSize];
}

void ArduComOptStatic::clearIncoming(){
  while (port->read() != -1);
}

//////////////////////////////////////////////////////////
/////////////////  ArduComOptStaticSlave   /////////////////////
//////////////////////////////////////////////////////////


void ArduComOptStaticSlave::executeMsg(){
  // after each atomic incoming msg, reply
  (*rFunc)(msgBuffer,NULL,msgSize);
  respond();
}

ArduComOptStaticSlave::ArduComOptStaticSlave(HardwareSerial *p,
					     ArduComOptStaticResponseFunc f, 
					     byte mSz): ArduComOptStatic::ArduComOptStatic(p,f,mSz){
  currentCharCount = 1;
}

void ArduComOptStaticSlave::doInit() {
  // setup the port
  port->begin(ARDUCOMOPTSTATIC_BAUDRATE);
  while(!*port);
  // send initChar and wait for response
  while(!initialized){
    port->write(ARDUCOMOPTSTATIC_INITCHAR);
    delay(ARDUCOMOPTSTATIC_INITDELAY);
    if(port->available()>0){
      char c = port->read();
      if (c !=ARDUCOMOPTSTATIC_INITCHAR){
        msgBuffer[currentCharCount++] = c;
      }      
      initialized = true;
    }
  }
}

void ArduComOptStaticSlave::stepLoop(){
  //processIncomingAtom();
  if (currentCharCount == responseSize) {
    executeMsg();
    currentCharCount = 1;
  }

  if (port->available() > 0) {
    // read one char and add it to the buffer if ok:
    char c = port->read();
    if (c !=ARDUCOMOPTSTATIC_INITCHAR){
      msgBuffer[currentCharCount++] = c;
    }
  }
}
//////////////////////////////////////////////////////////
/////////////////  ArduComOptStaticMaster  /////////////////////
//////////////////////////////////////////////////////////


void ArduComOptStaticMaster::executeMsg(){
  // We have received an incoming atom from Serial1,
  // if it is ok, then we confirm ack and can pop the queue 
  // if it is not ok, then we do not pop the queue and do not set the ACK
  // in any event, we try to send the atom currently at the top of the queue, 
  Serial.println("Entering: ArduComOptStaticMaster::executeMsg()");
  char sBuf[msgSize];
  if(q->pQ(sBuf) && 
     (*rFunc)(msgBuffer,sBuf,msgSize+1)){
     q->deQ(sBuf);
     // DEBUG: 1 line
     //Serial.println("in 'if' of ArduComOptStaticMaster::executeMsg()");
     msg("dq'd: ",sBuf,msgSize);
     atomAckedOnPort = true;
  }
  // so it's ok and we moved on, or it's not ok and we are still at the same on
  Serial.println("In ArduComOptStaticMaster::executeMsg, about to sendAtom.");
  sendAtom();  // try to send the next one, or the last one again 
}

void ArduComOptStaticMaster::sendAtom(){
  // send the raw char array, and set atomAckedOnPort to false, 
  // so that the Queue won't be popped until we get the ACK
  char sBuf[msgSize];
  if (q->pQ(sBuf)){
    port->write((byte*)sBuf,msgSize);
    // DEBUG: 7 lines
    if(sBuf[0] !='0' && sBuf[0] !='1'){
      Serial.println("Sent BAD atom!");
      freeRam();
      while(Serial.read()<0);
    }
    else{
      msg("Sent atom: ",sBuf,msgSize);
    }  
    atomAckedOnPort =false;
  }
}

ArduComOptStaticMaster::ArduComOptStaticMaster(HardwareSerial *p, 
					       byte mSz): ArduComOptStatic::ArduComOptStatic(p,&procReply,mSz),
							  atomAckedOnPort(false){
  currentCharCount = 0;
  q = new outQueueStatic();
}

boolean ArduComOptStaticMaster::enqueueMsg(char *msg){
  // DEBUG: 7 lines
  if(msg[0] !='0' && msg[0] !='1'){
    Serial.println("Enqd BAD msg!");
    freeRam();
    while(Serial.read()<0);
  }
  return q->enQ(msg);
}

void ArduComOptStaticMaster::doInit(){
  // wait for the other Arduino to send a char, if it is the initChar, then we can start
  // so we are "initialized" and the last sent atom has been acked, since there are none
  // if not just ignore the char as garbage... and pause for a short while.
  port->begin(ARDUCOMOPTSTATIC_BAUDRATE);
  while(!*port);
  while(!initialized){
    if (port->available()>0){
      char c = port->read();
      if (c==ARDUCOMOPTSTATIC_INITCHAR){
	atomAckedOnPort = initialized= true;
	port->write(ARDUCOMOPTSTATIC_INITCHAR);
	delay(ARDUCOMOPTSTATIC_INITDELAY);
      }
    }
  }
}

void ArduComOptStaticMaster::stepLoop(){
  // do one of the following:
  // if the current incoming atom is complete, then process it,
 // or, if there is something to read on Serial1, add it to the current incoming atom,
  // or, if it is ok to send something, then send the atom at the head of the queue
  //processIncomingAtom();
  if (currentCharCount == responseSize) {
    // DEBUG: 1 line
    msg("Rec'd reply: ",msgBuffer,responseSize);
    //Serial.println("hello?");
    executeMsg();
    currentCharCount = 0;
  }
  else if (port->available()>0){
    char c = port->read();
    //Serial.print("reading on port:\t");
    //Serial.println(c);
    if (c != ARDUCOMOPTSTATIC_INITCHAR){
      msgBuffer[currentCharCount++] = c;
    }
    else{
      port->write(ARDUCOMOPTSTATIC_INITCHAR);  // to clear an init call!
    }
    //Serial.print("\ncurrentCharCount:\t");
    //Serial.println(currentCharCount);
    // DEBUG: 1 line
    //msg("Rec'd a char : ",&c,1);
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
    //Serial.println("In ArduComOptStaticMaster::stepLoop, about to sendAtom.");
    sendAtom();  
  }
}
