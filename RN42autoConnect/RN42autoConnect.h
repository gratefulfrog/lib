/* RN42autoConnect.h
 * This class allows RN-42 bluetooth module to
 * communicate via Serial1 on an Arduino Micro or Leonardo 
 * Usage:
 *  // in std setup():
 *  RN42autoConnect(&Serial1).setupRN42AndConnect();
 *  // rest of setup...
 ***************
 * RN42 behavoir:
 * - power on: slow red blink
 * - command mode: quick red blink, searching for a connection... needs up to 10 seconds!
 * - connected: solid green
 * - loss of connection: slow red blink
 * - connection re-established (only with last connected RN42!): solid green 
 */

#ifndef RN42AUTOCONNECT_H
#define RN42AUTOCONNECT_H

#include <Arduino.h>

class RN42autoConnect {
 private:
  const static int cmdDelay = 100, // time to wait after setting command mode,
                   scanDelay = 10000; // time to wait for a scan of bt devices.
  const String commandModeConfirmationString;
  boolean setupCommandMode();
  void doConnect();
  void clearRN42Output();
  
  HardwareSerial *port;

public:
  RN42autoConnect(HardwareSerial *p);
  void setupRN42AndConnect(); 
};

#endif
