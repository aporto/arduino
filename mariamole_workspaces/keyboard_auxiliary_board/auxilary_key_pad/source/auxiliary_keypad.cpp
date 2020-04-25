//---------------------------------------------------------------------------

/*

This code uses an external keypad to control Synthesia

Example: Press the main (ESC) key on the external keypad to
send the sequence "," + "." to synthesia via keyboard (keygen) to
command synthesia to retun to the begining of the song

*/

#include "mariamole_auto_generated.h"

MiniKeyboard key (8,9,10,11,12);

//---------------------------------------------------------------------------

void setup() {                
	Mouse.begin();
	Keyboard.begin();
	Serial.begin(9600);
}

//---------------------------------------------------------------------------

void loop() {
	key.ReadPins();
	
	if (key.Esc()) {
		Serial.println("Esc");
		//Mouse.click(MOUSE_LEFT);
		//delay(40);
		//Mouse.click(MOUSE_LEFT);
		Keyboard.write(','); 
		Keyboard.write('.'); 
		while (key.Esc()) {
			key.ReadPins();
			delay(10);
		}		
	}	
	delay(100);	
}

//---------------------------------------------------------------------------