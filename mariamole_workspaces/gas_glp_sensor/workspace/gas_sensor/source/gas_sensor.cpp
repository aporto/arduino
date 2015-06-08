//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

#define ALARM_PERIOD			100
#define SOUND_ALARM_PIN			10
#define LED_ALARM_PIN			8

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
		analogWrite(SOUND_ALARM_PIN, 0);
		analogWrite(LED_ALARM_PIN, 0);
	}
	
	alarm = start;
}


void setup() {
  Serial.begin(9600);
	pinMode(SOUND_ALARM_PIN, OUTPUT);    
	pinMode(LED_ALARM_PIN, OUTPUT);    
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
				analogWrite(SOUND_ALARM_PIN, 127);
				digitalWrite(LED_ALARM_PIN, HIGH);
			} else {
				analogWrite(SOUND_ALARM_PIN, 200);
				digitalWrite(LED_ALARM_PIN, LOW);
			}
			lastAlarmPhaseCounter = millis();
		}		
	} else {
		analogWrite(SOUND_ALARM_PIN, 0);
		digitalWrite(LED_ALARM_PIN, LOW);
	}

	delay(200);	
}

//---------------------------------------------------------------------------