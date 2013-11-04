/* ArduStomp.cpp
 * this is the library class that is the top level for the 
 * ArduStompBox
 */

#include <ArduStomp.h>

ArduStomp *ArduStomp::as;

ArduStomp::ArduStomp(ArduComOptStaticMaster *cc,
		     PresetClass *pp,
		     AutoClass   *aa): 
  c(cc),
  p(pp),
  a(aa){
  curPresetIndex = 0;
}

void ArduStomp::init(){
  pinMode(LATCHPIN,OUTPUT);
  pinMode(CLOCKPIN,OUTPUT);
  pinMode(DATAPIN,OUTPUT);
  RN42autoConnectOpt(&Serial1).setupRN42AndConnect();
  //msg("rn42 auto connect ok!");
  ArduComOptStaticMaster *_c = new ArduComOptStaticMaster(&Serial1,
							  ARDUCOMOPTSTATIC_MSGSIZE);
  //msg("c instatiated");
  _c->doInit();
  // read the presets or not
  PresetClass *_p = new PresetClass(PFILE);
  //Serial.print("preset file id: ");
  //Serial.println((int)p);
  if(!_p->parse()){
    delete _p;
    _p = NULL;
  }
  // read the auto or not
  AutoClass *_a = new AutoClass(AFILE,_p);
  //Serial.print("auto file id: ");
  //Serial.println((int)a);
  if (!_a->parse()){
    delete _a;
    _a = NULL;
  }
  ArduStomp::as = new ArduStomp(_c,_p,_a);
}


void ArduStomp::loop(){
  // tbd
}

void ArduStomp::autoOff(){
  if (a && a->running()){
    Actuator::actuators[AUTO_ACT]->update();
  }
}
