//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

const int pingPin = 9; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 10; // Echo Pin of Ultrasonic Sensor


float distanceCm(int trigPin, int echoPin)
{
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	unsigned long duration = pulseIn(echoPin, HIGH);
	float distance= float(duration)*0.034/2.0;
	
	return distance;
}

void setup() {                
	pinMode(13, OUTPUT); 
	pinMode(5, OUTPUT); 
	pinMode(3, OUTPUT); 
	pinMode(pingPin, OUTPUT);
	Serial.begin(9600); // Starting Serial Terminal    
}

//---------------------------------------------------------------------------

void loop() {
	float distance = distanceCm(pingPin, echoPin);
	Serial.println(distance);
	if ((distance < 20.0) && (distance > 0.0)) {		
		tone(3, 300);
		for (int i=0; i < 6; i++) {
			
			digitalWrite(5, HIGH);   
			digitalWrite(12, LOW);   
			//noTone(3);
			delay(100);            
			//tone(3, 500);
			digitalWrite(12, HIGH);   
			digitalWrite(5, LOW);   
			//noTone(3);
			delay(100);            
		}
		noTone(3);
		digitalWrite(12, LOW);   
		digitalWrite(5, LOW);   		
	}
	delay(100);
}

	//---------------------------------------------------------------------------