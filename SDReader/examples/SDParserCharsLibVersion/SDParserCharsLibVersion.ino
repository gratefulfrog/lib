// -*- c++ -*-
 
#include <Arduino.h>
#include <SD.h>

#include <SDReader.h>
char pFile[] = "data.tsv",
     aFile[] = "cycle.tsv";
     
PresetClass *p;
AutoClass   *a;

boolean autoOK;
  
void printPreset(byte i){
  for (byte j=0;j<6;j++){ // iterate over the 6 values: vol, tone, neck, mid, briNor, briBoth
    Serial.print("  ");
    byte val;
    p->presetValue(i,j,&val); 
    Serial.print(val);
  }
  Serial.println();
  Serial.flush();
}
void  adjustPreset(byte i){
  byte mapper[][2] = {{1,0},
		      {0,1},
		      {11,0}};
  Serial.print("adjusted values, preset:\t");
  Serial.println(i);
  for (byte j=0;j<6;j++){ // iterate over the 6 values: vol, tone, neck, mid, briNor, briBoth
    byte curVal;
    p->presetValue(i,j,&curVal); 
    Serial.print("Current Val:\t");
    //Serial.print((int)p->presetValue(i,j)); 
    Serial.print(curVal);
    switch(curVal){
    case 0:
      curVal=1;
      break;
    case 1:
    case 5:
      curVal=0;
      break;
    default:
      curVal++;
      break;
    }
    p->presetValue(i,j,&curVal,true); 
    Serial.print("\tAdjusted Val:\t");
    byte newVal;
    p->presetValue(i,j,&newVal); 
    Serial.println(newVal);
  }
  Serial.println();
  Serial.flush();
}
   
void setup(){
  Serial.begin(115200);
  while(!Serial);
  delay(5000);
  p = new PresetClass(pFile);
  Serial.print("preset file id: ");
  Serial.println((int)p);
  // first read!
  if(p->parse()){
    for (byte i=0;i<4;i++){ // iterate over the 4 presets
      printPreset(i);
    }
    char cc[] = {'R', 'W', 'J', 'C'};
    for (int i=0;i<4;i++){
      Serial.println(p->firstLetter2Index(cc[i]));
    }
  }
  // then set
  for (byte i=0;i<4;i++){ // iterate over the 4 presets
    adjustPreset(i);
  }
  for (byte i=0;i<4;i++){ // iterate over the 4 presets
    printPreset(i);
  }

  a = new AutoClass(aFile,p);
  Serial.print("auto file id: ");
  Serial.println((int)a);

  autoOK = a->parse();
  if (autoOK){
    a->start(true);
  }
  Serial.println("Init Done.");
}

byte curPs = 100;
void loop(){
  if (autoOK){
    byte newPs = a->check();
    if (newPs != curPs){
      curPs = newPs;
      printPreset(curPs);
    }
  }
}
