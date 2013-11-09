/* ArduStomp.h
 * this is the library class that is the top level for the 
 * ArduStompBox
 */

#ifndef ARDUSTOMP_H
#define ARDUSTOMP_H

#include <Arduino.h>
#include <ArduConf00.h>
#include <State.h>
#include <outQueueStatic.h>
#include <ArduComOptStatic.h>
#include <outQueue.h>
#include <ArduComOpt.h>
#include <RN42autoConnectOpt.h>
#include <SD.h>
#include <SDReader.h>
#include <Actuator.h>
#include <LEDManager.h>

// for led shift register shifting
//Pin connected to latch pin (ST_CP) of 74HC595
#define LATCHPIN (13)
//Pin connected to clock pin (SH_CP) of 74HC595
#define CLOCKPIN (12)
////Pin connected to Data in (DS) of 74HC595
#define DATAPIN (11)

#define PFILE ("data.tsv")
#define AFILE ("cycle.tsv")
#define ALARM_PAUSE (1000)

class LEDManager;

class ArduStomp {
private:
  long         lastAlarmTime;

  ArduStomp(ArduComOptStaticMaster *cc,     
	    PresetClass *pp,
	    AutoClass   *aa);

public:
  static ArduStomp *as;
  static void init();
  byte curPresetIndex;  // as per SDReader
  ArduComOptStaticMaster *com;     
  PresetClass *p;
  AutoClass   *a;
  void doPreset();
  void checkAuto();
  void autoOff();
  void stepAlarm();
};

#endif
