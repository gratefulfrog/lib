#include <outils.h>

void freeRam (){
  extern int __heap_start, *__brkval;
  int v;
  static int result = 3000;
  int temp = min(result,((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval)));
  if (temp != result){
    result = temp;
    Serial.print("Free Ram: ");
    Serial.println(result);
  }
  //  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
