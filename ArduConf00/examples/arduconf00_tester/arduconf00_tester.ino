#include <ArduConf00.h>

void setup(){
 Serial.begin(115200);
  while (!Serial);
  delay(5000);
  for (byte i = 0;i<ArduConf00::connectID+1; i++){
    Serial.print("Ext ID: ");
    Serial.println(i);
    for (byte ii = 0; ii<ArduConf00::bufLenNbSettings(i,true); ii++){
      Serial.print("Control ID: ");
      Serial.print(i);
      Serial.print(" Value: ");
      Serial.print(ii);
      byte chars = ArduConf00::bufLenNbSettings(i) +1;
      if (chars >1){
        char buf[chars];
        //Serial.print(" Chars: ");
        //Serial.println(chars);
        ArduConf00::getMsg(i,ii,buf);
        buf[chars-1] = '\0';
        Serial.print(" msg: ");
        Serial.println(buf);
      }
    }
    Serial.println();
  }
}

void loop(){
}
