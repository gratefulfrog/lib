/* RN42autoConnect.cpp
 */

#include <RN42autoConnect.h>
/*
class RN42autoConnect {
 private:
  boolean setupCommandMode();
  void doConnect();
  void clearRN42Output();
  
  HardwareSerial *port;

public:
  RN42autoConnect(HardwareSerial*);
  void setupRN42AndConnect();
};
*/

boolean RN42autoConnect::setupCommandMode(){
  // set command mode to the RN42, when it is not connected automatically
  // wait for confirmation and reply TRUE if confirmed, false otherwise
  port->print("$");  // Print three times individually
  port->print("$");
  port->print("$");  // Enter command mode
  delay(cmdDelay);  // Short delay, wait for the Mate to send back CMD
  
  String rn42Output = "";
  while(port->available()){
    rn42Output += (char)port->read();  
  }
  boolean ret = rn42Output.indexOf(commandModeConfirmationString)>-1;
  return ret;
}
void RN42autoConnect::doConnect(){
  port->println("SR,Z");
  port->println("R,1");
}
void RN42autoConnect::clearRN42Output(){
  while (port->available()){
    port->read();  
  }
}
RN42autoConnect::RN42autoConnect(HardwareSerial *p):port(p),
						    commandModeConfirmationString("CMD"){
}
void RN42autoConnect::setupRN42AndConnect(){
  // this version depends ont eh RN42 being in automatic reconnect mode (SM,3), 
  // which means that it will automatically reconnect to the last BT device that it
  // knew, if in range!
  // so, to get it to connect to a new device, you have to rease the previous
  // device from memory, and reboot, which will force a scan for RN devices
  // and connection, and storage of that new device in RN42 memory
  // the 10 second delay is needed for the BT device scan.
  // at the end, we clear any relics that the RN42 may have sent during the scan
  // and connect process.
  port->begin(115200);
  while(!*port);
  while (!setupCommandMode());
  doConnect();
  delay(scanDelay);  // this is needed in the case of a long inquiry!
  clearRN42Output();
}

