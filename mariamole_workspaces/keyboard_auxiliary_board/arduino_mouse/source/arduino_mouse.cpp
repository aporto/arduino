//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

MiniKeyboard key (8,9,10,11,12);

//---------------------------------------------------------------------------

void setup() {                
	Mouse.begin();
	Keyboard.begin();
}

//---------------------------------------------------------------------------

void loop() {
	key.ReadPins();
	
	if (key.Left()) {
		Mouse.click(MOUSE_LEFT);
		delay(40);
		Mouse.click(MOUSE_LEFT);
		while (key.Left()) {
			key.ReadPins();
			delay(10);
		}
	}		
	
	if (key.Esc()) {
		Keyboard.press(',');
		delay(40);
		Keyboard.release(',');
		while (key.Esc()) {
			key.ReadPins();
			delay(10);
		}		
	}
	
	if (key.Enter()) {
		Keyboard.press(',');
		delay(20);
		Keyboard.release(',');
		Keyboard.press('.');
		delay(20);
		Keyboard.release('.');
		
		while (key.Enter()) {
			key.ReadPins();
			delay(10);
		}		
	}
	delay(100);	
}

//---------------------------------------------------------------------------