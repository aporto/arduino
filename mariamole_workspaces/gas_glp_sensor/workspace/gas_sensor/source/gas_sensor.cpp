//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

#define ALARM_PERIOD		100
#define ALARM_PIN			10

char alarmMode = 0;
unsigned long lastAlarmPhaseCounter;
unsigned long alarmCounter = 0;
bool alarm;

void setAlarm(bool start)
{
	if (start) {
		alarmMode = 0;
		lastAlarmPhaseCounter = 0;
	} else {
		analogWrite(ALARM_PIN, 0);
	}
	
	alarm = start;
}


void setup() {
  Serial.begin(9600);
	pinMode(ALARM_PIN, OUTPUT);    
	setAlarm(true);	
}


void update(void)
{
	
}

void loop() {
	int sensorvalue = analogRead(A0);
	float vol = 100.0 * (float)sensorvalue / 1024.0;	
	float pot = 100.0 * float(analogRead(A2)) / 1024.0;
	
	Serial.print(vol);
	Serial.print(" : ");
	Serial.println(pot);
	
	alarm = (vol > pot);
	
	if (alarm) {
		unsigned long ms = millis() - lastAlarmPhaseCounter;
		if (ms > ALARM_PERIOD) {
			alarmMode++;
			if (alarmMode > 1) {
				alarmMode = 0;							
			}
			if (alarmMode == 0) {
				analogWrite(ALARM_PIN, 127);
			} else {
				analogWrite(ALARM_PIN, 200);
			}
			lastAlarmPhaseCounter = millis();
		}		
	} else {
		analogWrite(ALARM_PIN, 0);
	}

	delay(200);	
}

//---------------------------------------------------------------------------