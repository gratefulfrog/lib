#include "SDReader.h"

void SDReader::skipHeaderLine(){
  // just read the chars of the first line and ignore them, 
  // set the file pointer to the 1st char of the 2nd line
  char c = f.read();     
  while (c!='\r' && c != '\n'){
      c = f.read();
  }    
}

SDReader::SDReader(char *FileName, boolean sdBegun){
  if(!sdBegun){
    pinMode(SDPIN, OUTPUT); 
    SD.begin(SDPIN);
  }
  f = SD.open(FileName);
}


////////////////////////////////////////
///  PresetClass Methods //////////////
////////////////////////////////////////

PresetClass::PresetClass(char *presetFileName): SDReader(presetFileName,false){
  for (byte b=0;b<NB_PRESETS;b++){
    presets[b] =  0;
    mapp[b] = '\0';
  }
}

byte PresetClass::preVal(unsigned int i, unsigned int mask, int shift) const{
  // internal call
  return (i >> shift) & mask;
}

//byte PresetClass::presetValue(byte presetIndex, byte key) const{
void PresetClass::presetValue(byte presetIndex, 
			      byte key, 
			      byte *val, 
			      boolean set){
  // public method to read and set the preset values
  // in case of set: values should be on internal GUITAR scale ALREADY
  // PUPs 0,1
  // vol or tone : [0,5]
  unsigned int mask =  P_PUP,
    shift = P_BB_SHIFT;
  switch (key) {
    case volKey:
      mask = P_VT;
      shift = P_VOL_SHIFT;
      break;
      //return preVal(presets[presetIndex],P_VT, P_VOL_SHIFT);
    case toneKey:
      mask = P_VT;
      shift = P_TONE_SHIFT;
      break;
      //return preVal(presets[presetIndex],P_VT, P_TONE_SHIFT);
    case neckKey:
      shift =  P_NECK_SHIFT;
      break;
      //return preVal(presets[presetIndex],P_PUP, P_NECK_SHIFT);
    case middleKey:
      shift = P_MID_SHIFT;
      break;
      //return preVal(presets[presetIndex],P_PUP, P_MID_SHIFT);
    case bridgeNorthKey:
      shift = P_BN_SHIFT;
      break;
      //return preVal(presets[presetIndex],P_PUP, P_BN_SHIFT);    
      //case bridgeBothKey:
      //return preVal(presets[presetIndex],P_PUP, P_BB_SHIFT);    
  }
  if(set){
    presets[presetIndex] =  
      (presets[presetIndex] & ~(mask<<shift)) | 
      ((*val) << shift);
  }
  *val = preVal(presets[presetIndex],mask,shift);    
}

boolean PresetClass::parse() {
  if (!f){
    //Serial.println("preset parse fail"); 
    return false;
  }
  byte line = 0,
       pos = 0;
  boolean gotFirstChar = false;
  skipHeaderLine();
  while(line < NB_PRESETS && f.available()){
    char c[] = {'\0', '\0', '\0'},
         cc[] = {'\0','\0'};
    c[0] = f.read();
    switch (c[0]){
      case '\r':
      case '\n':
        line++;
        pos = 0;
        gotFirstChar = false;
        //Serial.println("R or N");
        break;
      case '\t':
        pos++;
        //Serial.println("T");
        break;
      default:
        switch (pos){ 
          case 0: // first letter read  for name
            if (!gotFirstChar){
              mapp[line] = c[0];
              gotFirstChar = true;
              //Serial.println(c);
            }
            break;
          case 1: // volume, read another char and convert
            if (f.peek() != '\t'){
              //Serial.println("reading another in vol.");
              c[1] = f.read();
            }
            //Serial.println(c);
            presets[line] |= (constrain(round(atoi(c)/2.0),0,5) << P_VOL_SHIFT);
            break;
          case 2: // tone, read another and convert
            cc[0] = f.peek();
            //Serial.print("Got this on the tone peek: ");
            //Serial.println(cc);
            if (f.peek() != '\t'){
              //Serial.println("reading another in tone.");
              c[1] = f.read();
            }
	    /*
	      Serial.print("Tone read\t");
	      Serial.println(c);
	      Serial.println("\tTone wrote\t");
	      Serial.println(constrain(round(atoi(c)/2.0),0,5));
	    */
            presets[line] |= (constrain(round(atoi(c)/2.0),0,5) << P_TONE_SHIFT);
            break;
          default: // neck , mid, bridgeN, bridgeB
            presets[line] |= (atoi(c) << (6 - pos));
            //Serial.println(c);
        }
    }
  }
  f.close();
  //Serial.println("Presets Read:");
  //tempGet();
  return true;
}

byte PresetClass::firstLetter2Index(char c) const {
  byte ret = 0;
  while (ret<NB_PRESETS){
    if (mapp[ret] == c){
      break;
    }
    ret++;
  }
  return ret;
}

void PresetClass::save(char *presetsFileName) {
  SD.remove(presetsFileName);
  /*
    if (!(f = SD.open(presetsFileName,FILE_WRITE))){
    //Serial.println("preset.save: open fail"); 
    return false;
  }
  */  
  char names[4][8] = {"\nRock\t","\nWoman\t","\nJazz\t", "\nComp\t"};
  f = SD.open(presetsFileName,FILE_WRITE);
  f.print("name\tvol\ttone\tneck\tmiddle\tbNorth\tbBoth");
  for (byte i=0;i<NB_PRESETS;i++){
    f.print(names[i]);
    byte ind = firstLetter2Index(names[i][1]),
      val;
    for (byte k =0;k<nbKeys-1;k++){
      presetValue(ind,k,&val);
      f.print(int(k<2? 2*val :val));  // correct 2014 01 30
      f.print('\t');
    }
    presetValue(ind,nbKeys-1,&val);
    f.print(int(val));
  }
  f.print("\nAuto\t0\t0\t0\t0\t0\t0\n");
  f.close();
}
	


////////////////////////////////////////
///  AutoClass Methods ////////////////
////////////////////////////////////////

byte AutoClass::currentIndex() const{
  return (current->i >> A_SHIFT)  & A_INDEX ;
}

int AutoClass::currentPause() const{
  return (current->i & A_PAUSE);
}

AutoClass::AutoClass(char *autoFileName, const PresetClass *ps):SDReader(autoFileName,true), p(ps) {
  firstAuto.i = 0;
  firstAuto.next = NULL;
  current = &firstAuto;
  lastAutoTime = 0;
  running = false;
}

byte AutoClass::presetIndex(){
  byte ret = p->firstLetter2Index(f.read());
  while(f.read() != '\t');
  //Serial.print("Preset Index: ");
  //Serial.println((int)ret);
  return ret;
} 


int AutoClass::getNextNumber(){
  char nc[] = {'\0','\0','\0','\0','\0','\0'};
  byte i=0;
  while(f.available() && f.peek() != '\n'){
    nc[i++] = f.read();
  }
  //Serial.println(min(16384, atoi(nc)));
  return min(MAX_AUTO_PAUSE, atoi(nc));
}

boolean AutoClass::parse(){
  if (!f){
    return false;
  }
  struct autoStruct *cur = &firstAuto;
  skipHeaderLine();
  while(f.available()){
    cur->i  = presetIndex() << A_SHIFT;
    cur->i |= getNextNumber();
    //Serial.print("Made one: ");
    //Serial.println(cur->i);
    if (f.available()) { // then move forward by one
      cur->next = new struct autoStruct;
      cur = cur->next;
      f.read();
    }
  }
  cur->next = &firstAuto;
  f.close();
  return true;
}

void AutoClass::start(boolean yes = true){
  running = yes;
  current = &firstAuto;
  lastAutoTime = millis();
}

byte AutoClass::check(){  // returns the index of the preset that is current
  long now = millis();
  if (now > lastAutoTime + currentPause()) { // then we inc 
    current = current->next;
    lastAutoTime = millis();
  }
  return currentIndex();
}
  


