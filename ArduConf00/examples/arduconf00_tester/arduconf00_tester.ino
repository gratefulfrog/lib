#include <ArduConf00.h>

void setup(){
 
 Serial.begin(115200);
  while (!Serial);
  delay(5000);
  for (byte i = 0;i<ArduConf00::nbControls; i++){
    for (byte ii = 0; ii<ArduConf00::msgLenNbSettings[i][1]; ii++){
      Serial.print("Control ID: ");
      Serial.print(i);
      Serial.print(" Value: ");
      Serial.print(ii);
      byte chars = ArduConf00::msgLenNbSettings[i][0]*ArduConf00::wordLen +1;
      char buf[chars];
      //Serial.print(" Chars: ");
      //Serial.println(chars);
      ArduConf00::getMsg(i,ii,buf);
      buf[chars-1] = '\0';
      Serial.print(" msg: ");
      Serial.println(buf);
    }
  }
}

void loop(){
}
