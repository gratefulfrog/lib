#include <SPI.h>

#define LATCHPIN (13)   // Pin connected to latch pin (ST_CP) of 74HC595

byte b1 =1,
     b2 =1;
unsigned int i = 1;
boolean useBytes = true;

void setup(){
  pinMode(LATCHPIN, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(115200);
  while(!Serial);
  delay(5000);
}

void useUnsignedInt(){
  i = (i == 0 ? 1 : (i << 1));
  Serial.print("Array:\t");
  Serial.print(i,BIN);
  Serial.println();
  SPI.transfer(i>>8);  // second byte! 128 = connect; 1 = vol[0]
  SPI.transfer(i & 0B11111111);  // first byte  128 = auto; 1 = neck
}
void use2Bytes(){
  b2 = (b2 == 128 ? 1 : (b2 << 1));
  b1 = (b1 == 128 ? 1 : (b1 << 1));
  Serial.print("B1:\t");
  Serial.print(b1,BIN);
  
  Serial.print(b1 < 128 ? "\t\tB2:\t" : "\tB2:\t");
  Serial.print(b2,BIN);
  Serial.println();
  SPI.transfer(b2);  // second byte! 128 = connect; 1 = vol[0]
  SPI.transfer(b1);  // first byte  128 = auto; 1 = neck
}  

void loop(){
  useBytes = (Serial.read() > -1 ? !useBytes : useBytes);
  digitalWrite(LATCHPIN, LOW);
  if (useBytes){
    use2Bytes();
  }
  else{
    useUnsignedInt();
  }
  digitalWrite(LATCHPIN, HIGH);
  delay(1000);
}


/* with MSBFIRST,
* using an unsigned int as the ledArray,
* take the leftmost part and transfer it first
* 128 = connect
* 64 = power
* 32 = tone[2]
* 16 = tone[1]
*  8 = tone[0]
*  4 = vol[2]
*  2 = vol[1]
*  1 = vol[0]
the rightmost byte is then transfererd
* 128 = auto
* 64 = presets[2]
* 32 = presets[1]
* 16 = presets[0]
*  8 = bridge[1]
*  4 = bridge[0]
*  2 = middile
*  1 = neck
*/
