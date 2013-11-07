// -*-c++-*-
/* This is an example of how to use the ArduComOptSlave class.
 * First we have to define a function that will generate the response to incoming messages
 * 'prcoMsg' is just that: It prepends alternatively an 'e' or an 'x' to the incoming message
 * Then we declare a pointer to an ArduComOptSlave, 
 * we instatiate the ArduComOptSlave, then call its doInit method. Upon exit from that, it is ready to go.
 * Note that we have used 'Serial' as the serial object port.
 * in the loop(), we only have to call stepLoop() and the rest is magic.
 * test this by inputing at the Serial Monitor:
 * first type an 'A' to confirm init,
 * then type sequences of 5 chars and observing the responses.
 * NOTE: the librairy outQueue.h must be included for this to work, I do not know why...
 */

#include <State.h>
#include <outQueueStatic.h>
#include <ArduComOptStatic.h>

boolean procMsg(char *buf,char*, byte bufSize){
  // just for creating errors,
  // this is an 'ArduComOptResponseFunc'
  static boolean lastResponseTrue = false;
  if (lastResponseTrue){
    buf[0] =  ARDUCOMOPTSTATIC_ERRORCHAR;
  }
  else{
    buf[0] =  'x';
  }
  lastResponseTrue = !lastResponseTrue;
  return !lastResponseTrue;
}
  
ArduComOptStaticSlave *com;

//////////////  std functions ////////////////

void setup(){
  com = new ArduComOptStaticSlave(&Serial,&procMsg,ARDUCOMOPTSTATIC_MSGSIZE);
  com->doInit();
}

void loop(){
  com->stepLoop();
}
