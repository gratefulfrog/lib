/* ArduComOptStatic.h
 * ArduComOpt class,
 * using outQueue with dynamic allocation,
 * responseLength = messageLength + 1
 * only one buffer used for both incoming and outgoing
 */
#ifndef ARDUCOMOPTSTATIC_H
#define ARDUCOMOPTSTATIC_H

#define ARDUCOMOPTSTATIC_INITCHAR ('A')
#define ARDUCOMOPTSTATIC_ERRORCHAR ('e')
#define ARDUCOMOPTSTATIC_BAUDRATE (115200)
#define ARDUCOMOPTSTATIC_INITDELAY  (100)   // pause at initialization 
#define ARDUCOMOPTSTATIC_MSGSIZE 5

#include <Arduino.h>
#include <outQueueStatic.h>

typedef boolean (*ArduComOptStaticResponseFunc)(char*,char*,byte);

// this is part of the protocol
boolean procReply(char *reply, char *sent, byte replySize);

class ArduComOptStatic {
  protected:
    HardwareSerial *port;    
    ArduComOptStaticResponseFunc rFunc;
    boolean initialized;
    byte currentCharCount;   // how many chars we've read
    char *msgBuffer;

    void respond();    
    virtual void executeMsg() = 0;
  public:
    // DEBUG: 2 lines for debug, not used otherwise
    static void msg(String s) ;
    static void msg(String s, char *c, int len) ;
    // end debug methods

    const byte msgSize,
               responseSize;
  
    ArduComOptStatic(HardwareSerial *p,ArduComOptStaticResponseFunc f, byte mSz);
    void clearIncoming();
    virtual void doInit() = 0;
    virtual void stepLoop() = 0;
};

class ArduComOptStaticSlave: public ArduComOptStatic {
 protected:
  void executeMsg();
 public:
   ArduComOptStaticSlave(HardwareSerial *p,ArduComOptStaticResponseFunc f, byte mSz);
   void doInit();
   void stepLoop();
};

class ArduComOptStaticMaster: public ArduComOptStatic {
 protected:
  outQueueStatic *q;
  boolean atomAckedOnPort;
  void executeMsg();
  void sendAtom();
 public:
   ArduComOptStaticMaster(HardwareSerial *p, byte mSz);
   boolean enqueueMsg(char *msg);
   //char* peekQ();
   void doInit();
   void stepLoop();
};

#endif


