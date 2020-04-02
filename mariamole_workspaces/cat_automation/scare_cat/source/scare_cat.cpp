//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define SERVO_PIN	5

const unsigned long TIME_BETWEEN_CHECKS =	500;	// miliseconds

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo servo;  
int maxDistance = 0;

//---------------------------------------------------------------------------

int GetDistance()
{
	unsigned int uS = sonar.ping(); 
	unsigned int cm = uS / US_ROUNDTRIP_CM;
	return cm;
}

//---------------------------------------------------------------------------

void Calibrate(void)
{
	maxDistance = 130;
	
	Serial.println("Calibrating...");
	
	for (int i=0; i < 10; i++) {
		unsigned int cm = GetDistance();
		if (cm > 0) {			
			if (cm < maxDistance) {
				maxDistance = cm;
			}
		}
		delay(100);
	}
	
	maxDistance = int(0.95f * float(maxDistance));
	
	Serial.print("Calibrated at: ");
	Serial.print(maxDistance);
	Serial.print(" cm");
	Serial.println();	
}

//---------------------------------------------------------------------------

bool CheckIfTheCatIsAround()
{
	unsigned int cm = GetDistance();
	
	Serial.print(cm);
	Serial.print("cm");
	Serial.println();
	
	return ((cm > 0) && (cm < maxDistance));	
	//return (cm < 30);	
}


//---------------------------------------------------------------------------

void squirt(void)
{
	for (int i=0; i < 5; i++) {
		servo.write(175);              // tell servo to go to position in variable 'pos' 
		delay(200);
		servo.write(120);              // tell servo to go to position in variable 'pos' 
		delay(200);
	}
}

//---------------------------------------------------------------------------

void setup() {                
	servo.attach(SERVO_PIN);  
	
	Serial.begin(9600);		
	delay(1000);
	Calibrate();
	//while (!Serial) ; // Leonardo is a jerk
}

//---------------------------------------------------------------------------

void loop() {
	bool turnOn = CheckIfTheCatIsAround();	
	
	if (turnOn) {
		Serial.println("Gato apareceu");
		squirt();		
	}
	Serial.println("Waiting...");
	delay(TIME_BETWEEN_CHECKS);            	
}

//---------------------------------------------------------------------------

