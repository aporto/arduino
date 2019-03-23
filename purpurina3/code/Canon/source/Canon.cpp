//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

//Canon D5(IR_pin);

#define IR_pin	23

void setup() {          
	Serial.begin  (9600);	
	pinMode(IR_pin, OUTPUT);     
}


void Canon_trigger() {
    for(int i=0; i<16; i++) {
        digitalWrite(IR_pin, HIGH);
        delayMicroseconds(11);
        digitalWrite(IR_pin, LOW);
        delayMicroseconds(11);
    } 
    delayMicroseconds(7330); 
    for(int i=0; i<16; i++) { 
        digitalWrite(IR_pin, HIGH);
        delayMicroseconds(11);
        digitalWrite(IR_pin, LOW);
        delayMicroseconds(11);
    }   
}


//---------------------------------------------------------------------------

void loop() {
	Serial.println("Shot!");
	EOS_trigger();
	/*
	D5.shotNow();
	delay(7);
	D5.shotNow();
	delay(7);	
	*/
	delay(1000);            	
}

//---------------------------------------------------------------------------