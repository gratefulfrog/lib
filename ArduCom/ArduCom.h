/* ArduCom.h
 * ArduCom class,
 * responseLength = messageLength + 1
 * only one buffer used for both incoming and outgoing
 */
#ifndef ARDUCOM_H
#define ARDUCOM_H

#define INITCHAR 'A'
#define BAUDRATE 115200

#define ARDUCOM_MSGSIZE 5

#include <Arduino.h>
#include <outQueue.h>

typedef boolean (*ArduComResponseFunc)(char*,char*,int);

// this is part of the protocol
boolean procReply(char *reply, int replySize);


class ArduCom {
  protected:
    const static char initChar = INITCHAR;
    const static long baudRate = BAUDRATE;

    HardwareSerial *port;
    
    ArduComResponseFunc rFunc;
    
    boolean initialized;
    int currentCharCount;   // how many chars we've read
    char *msgBuffer;
         //*responseBuffer;

    void respond();    
    virtual void executeMsgString() = 0;
    void processMsgAtom();
    void readIntoMsgAtom();
  public:
    static void msg(String s) ;
    static void msg(String s, char *c, int len) ;

    const int msgSize,
              responseSize;
  
    ArduCom(HardwareSerial *p,ArduComResponseFunc f, int mSz);
    void clearIncoming();
    virtual void doInit() = 0;
    virtual void stepLoop() = 0;
};

class ArduComSlave: public ArduCom {
 protected:
  void executeMsgString();
 public:
   ArduComSlave(HardwareSerial *p,ArduComResponseFunc f, int mSz);
   void doInit();
   void stepLoop();
};

class ArduComMaster: public ArduCom {
 protected:
  outQueue *q;
  boolean atomAckedOnPort;
  const static  int initDelay = 100; // pause at initialization 
  void executeMsgString();
  void sendAtom();
 public:
   ArduComMaster(HardwareSerial *p, int mSz);
   void enqueueMsg(char *msg);
   char* peekQ();
   void doInit();
   void stepLoop();
};

#endif


