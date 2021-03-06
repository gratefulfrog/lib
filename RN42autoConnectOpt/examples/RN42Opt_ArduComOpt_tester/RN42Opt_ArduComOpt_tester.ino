/*
 * This example sketch allows RN-42 bluetooth module to
 * communicate via Serial1 on an Arduino Micro or Leonardo 
 * using the ArduCom library for error checking and correction
 
 * RN42 behavoir:
 * - power on: slow red blink
 * - command mode: quick red blink, searching for a connection... needs up to 10 seconds!
 * - connected: solid green
 * - loss of connection: slow red blink
 * - connection re-established (only with last connected RN42!): solid green 
 */

#include <outQueue.h>
#include <ArduComOpt.h>
#include <RN42autoConnectOpt.h>

//////////////////////////////////////////////////////////////////////////
////////////////////   Serial Terminal Globals  //////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Global Variables
int incomingMsgSizeSerial = ARDUCOMOPT_MSGSIZE, 
    currentCharCountSerial = 0;   // how many chars we've read on Serial Terminal

char outgoingBuffer[ARDUCOMOPT_MSGSIZE];

ArduComOptMaster *c;

///////////////////////////////////////////////////////////////////////////
/////////////  Terminal Input and equeueing       /////////////////////////
///////////////////////////////////////////////////////////////////////////
void processSerialMonitorIncoming() {
  // this is a part of the test suite, not of the ArduCom class 
  // If we have already read a whole atom, then enqueue it for sending, and reset char count
  // or, if we have something to read, add it to the current underwork atom
  if (currentCharCountSerial == incomingMsgSizeSerial){
    c->enqueueMsg(outgoingBuffer);
    ArduComOpt::msg("Enqueued:" ,outgoingBuffer,c->msgSize);
    currentCharCountSerial = 0;
  }
  else if (Serial.available()>0){
    outgoingBuffer[currentCharCountSerial++] = Serial.read();
  }
}

///////////////////////////////////////////////////////////////////////////
///////////////////   Serial Monitor Functions     ////////////////////////
///////////////////////////////////////////////////////////////////////////

void setupSerial(){
  Serial.begin(115200);  // Begin the serial monitor at 115200bps
  while(!Serial);
  delay(5000);           // to give the human time to start the Serial Monitor  
}
///////////////////////////////////////////////////////////////////////////
///////////////////       Std Functions     ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

void setup(){
  RN42autoConnectOpt(&Serial1).setupRN42AndConnect();
  setupSerial();
  c = new ArduComOptMaster(&Serial1,ARDUCOMOPT_MSGSIZE);
  c->doInit();
  Serial.println("Ready!");
}
    
void loop(){
  // read and process anything from the Serial Monitor; 
  processSerialMonitorIncoming();
  // step the comms loop
  c->stepLoop();
}

