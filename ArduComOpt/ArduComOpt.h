/* ArduComOpt.h
 * ArduComOpt class,
 * using outQueue with dynamic allocation,
 * responseLength = messageLength + 1
 * only one buffer used for both incoming and outgoing
 */
#ifndef ARDUCOMOPT_H
#define ARDUCOMOPT_H

#define ARDUCOMOPT_INITCHAR ('A')
#define ARDUCOMOPT_ERRORCHAR ('e')
#define ARDUCOMOPT_BAUDRATE (115200)
#define ARDUCOMOPT_INITDELAY  (100)   // pause at initialization 
#define ARDUCOMOPT_MSGSIZE 5

#include <Arduino.h>
#include <outQueue.h>

typedef boolean (*ArduComOptResponseFunc)(char*,char*,byte);

// this is part of the protocol
boolean procReply(char *reply, char *sent, byte replySize);

class ArduComOpt {
  protected:
    HardwareSerial *port;    
    ArduComOptResponseFunc rFunc;
    boolean initialized;
    byte currentCharCount;   // how many chars we've read
    char *msgBuffer;

    void respond();    
    virtual void executeMsg() = 0;
  public:
    // DEBUG: 2 lines for debug, not used otherwise
    //static void msg(String s) ;
    //static void msg(String s, char *c, int len) ;
    // end debug methods

    const byte msgSize,
               responseSize;
  
    ArduComOpt(HardwareSerial *p,ArduComOptResponseFunc f, byte mSz);
    void clearIncoming();
    virtual void doInit() = 0;
    virtual void stepLoop() = 0;
};

class ArduComOptSlave: public ArduComOpt {
 protected:
  void executeMsg();
 public:
   ArduComOptSlave(HardwareSerial *p,ArduComOptResponseFunc f, byte mSz);
   void doInit();
   void stepLoop();
};

class ArduComOptMaster: public ArduComOpt {
 protected:
  outQueue *q;
  boolean atomAckedOnPort;
  void executeMsg();
  void sendAtom();
 public:
   ArduComOptMaster(HardwareSerial *p, byte mSz);
   //void enqueueMsg(char *msg);
   boolean enqueueMsg(char *msg);
   //char* peekQ();
   void doInit();
   void stepLoop();
};

#endif


