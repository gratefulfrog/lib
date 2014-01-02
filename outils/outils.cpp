#include <outils.h>
/*
void freeRam (boolean force){
  extern int __heap_start, *__brkval;
  int v;
  static int result = 3000;
  int temp = min(result,((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval)));
  if (force || temp != result){
    result = temp;
    Serial.print("Free Ram: ");
    Serial.println(result);
  }
  //  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
*/
/*
extern void msg(char* str, char *buf, byte len){
  bool something =false;
  if (sizeof(str)){
    Serial.print(str);
    something = true;
  }
  for (byte b=0;b<len;b++){
    Serial.print(buf[b]);
    something = true;
  }
  if(something){
    Serial.println();
  }
}
*/
/*
void checkRam(){
  static long lastRamCheckTime = millis();
  const int freeRameCheckDelay = 10000;

  if (millis() - lastRamCheckTime > freeRameCheckDelay){
    lastRamCheckTime = millis();
    freeRam(true);
  }
}  
*/
