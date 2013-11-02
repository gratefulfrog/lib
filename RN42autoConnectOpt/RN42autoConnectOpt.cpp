/* RN42autoConnect.cpp
 */

#include <RN42autoConnectOpt.h>

/*
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
*/

boolean RN42autoConnectOpt::setupCommandMode(){
  // set command mode to the RN42, when it is not connected automatically
  // wait for confirmation and reply TRUE if confirmed, false otherwise
  port->print("$");  // Print three times individually
  port->print("$");
  port->print("$");  // Enter command mode
  // Short delay, wait for the Mate to send back CMD
  delay(RN42AUTOCONNECTOPT_CMD_DELAY);  
  
  byte boolByte = 0;
  String rn42Output = "";
  while(port->available()){
    switch (boolByte){
      case 0:
	// we have not read any of the CMD mode ACK, we are looking for 'C'
	boolByte = ((c == (char)port->read()) ? 4 : 0);
	break;
      case 4:
	// we have just read the 'C', now looking for 'M'
	boolByte = ((m == (char)port->read()) ? 6 : 0);
	break;
      case 6:
	// we have just read the 'CM', now looking for 'D'
	boolByte = ((d == (char)port->read()) ? 7 : 0);
	break;
      default:
	port->read();
    }
  }
  return boolByte == 7;
}

void RN42autoConnectOpt::doConnect(){
  port->println("SR,Z");
  port->println("R,1");
}
void RN42autoConnectOpt::clearRN42Output(){
  while (port->available()){
    port->read();  
  }
}
 RN42autoConnectOpt::RN42autoConnectOpt(HardwareSerial *p):port(p){
}
void RN42autoConnectOpt::setupRN42AndConnect(){
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
  // this is needed in the case of a long inquiry!
  delay(RN42AUTOCONNECTOPT_SCAN_DELAY);  
  clearRN42Output();
}

