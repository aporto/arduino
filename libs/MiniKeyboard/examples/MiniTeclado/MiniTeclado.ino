//---------------------------------------------------------------------------

#include "MiniKeyboard.h"

MiniKeyboard key (8,9,10,11,12);

//---------------------------------------------------------------------------

void setup() {                
	Serial.begin(9600);
}

//---------------------------------------------------------------------------

void output (String text, bool value)
{
	Serial.print (text);
	Serial.print (": ");
	if (value) {
		Serial.print ("true");
	} else {
		Serial.print ("false");
	}
	Serial.println("");
}

void loop() {
	key.ReadPins();
	
	output("Left ", key.Left());
	output("Up   ", key.Up());
	output("Down ", key.Down());
	Serial.println ("");
	output("Right", key.Right());
	output("Enter", key.Enter());
	output("Esc  ", key.Esc());
	
	Serial.println ("-------------------------------------");
	
	delay(1000);            
}

//---------------------------------------------------------------------------