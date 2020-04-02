//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define RELAY_PIN  11 

const unsigned long TIME_TO_DRINK =			60000;	// miliseconds
const unsigned long TIME_BETWEEN_CHECKS =	1000;	// miliseconds

bool firstTeste = true;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//---------------------------------------------------------------------------

bool CheckIfTheCatIsAround()
{
	unsigned int uS = sonar.ping(); 
	unsigned int cm = uS / US_ROUNDTRIP_CM;
	
	Serial.print(cm);
	Serial.print("cm");
	Serial.println();
	
	return ((cm > 0) && (cm < 30));	
	//return (cm < 30);	
}

//---------------------------------------------------------------------------

bool TurnPumpOn(bool on)
{
	if (on == false) { // relay uses inverted logic
		digitalWrite(RELAY_PIN, HIGH);
	} else {
		digitalWrite(RELAY_PIN, LOW);
	}
}

//---------------------------------------------------------------------------

void setup() {                
	// turn the board led off
	pinMode(13, OUTPUT);     
	digitalWrite(13, LOW);
	pinMode(RELAY_PIN, OUTPUT);     
	
	bool blink = true;
	// force the pump to close during initalizing
	for (int  i=0; i < 20; i++) {				
		if (blink) {
			digitalWrite(13, HIGH);
		} else {
			digitalWrite(13, LOW);
		}
		TurnPumpOn(false);
		delay (100);
		blink = !blink;		
	}
	digitalWrite(13, LOW);
	
	Serial.begin(9600);	
	
	//while (!Serial); // Leonardo is a jerk
}

//---------------------------------------------------------------------------

void loop() {
	
	bool turnOn = CheckIfTheCatIsAround();	
	
	if (turnOn) {
		Serial.println("Gato apareceu");
		TurnPumpOn(true);
		if (firstTeste) {
			delay(2000);
			firstTeste = false;
		} else {
			delay(TIME_TO_DRINK);		
		}
	}
	TurnPumpOn(false);
	Serial.println("Waiting...");
	delay(TIME_BETWEEN_CHECKS);            	
}

//---------------------------------------------------------------------------

