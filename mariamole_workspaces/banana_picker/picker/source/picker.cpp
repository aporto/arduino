#include "mariamole_auto_generated.h"

#include <Servo.h>

// this shall be identified after modifying the servo for continuos rotation. 
// It depends on the difference between the two selected resistors 

#define SERVO_CENTER 	112 

Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created


int pos = 0;    // variable to store the servo position

void setup()
{
	myservo.attach(5);  // attaches the servo on pin 9 to the servo object
	Serial.begin(9600);
	
	pinMode(2, INPUT_PULLUP);     
	pinMode(3, INPUT_PULLUP);  

	myservo.write(SERVO_CENTER);
	delay(15);        			   	
}

bool wasPressed = false;

void loop()
{
	bool b1 = !digitalRead(2);
	bool b2 = !digitalRead(3);

	if (b1 != b2) {
		if (b1) {
			myservo.write(SERVO_CENTER + 30);
			Serial.println("sobe");
			wasPressed = true;						
			delay(15);        			
		} else {
			myservo.write(SERVO_CENTER - 30);
			Serial.println("desce");
			wasPressed = true;						
			delay(15);        			
		}		
	} else {
		if (wasPressed) {
			myservo.write(SERVO_CENTER);
			Serial.println("off");
			wasPressed = false;						
			delay(15);        			
		}
	}
	
	//delay(500);
	/*for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
	{                                  // in steps of 1 degree
		
	}
	for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees
	{
		myservo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(15);                       // waits 15ms for the servo to reach the position
	}
	*/
}
