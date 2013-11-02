/* RN42autoConnectOpt.h
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

#ifndef RN42AUTOCONNECTOPT_H
#define RN42AUTOCONNECTOPT_H

#include <Arduino.h>

// time to wait after setting command mode,
#define RN42AUTOCONNECTOPT_CMD_DELAY (100)
// time to wait for a scan of bt devices.
#define RN42AUTOCONNECTOPT_SCAN_DELAY (10000)

class RN42autoConnectOpt {
 private:
  const static char c = 'C',
                    m = 'M',
                    d = 'D';
  boolean setupCommandMode();
  void doConnect();
  void clearRN42Output();
  
  HardwareSerial *port;

public:
  RN42autoConnectOpt(HardwareSerial *p);
  void setupRN42AndConnect(); 
};

#endif
