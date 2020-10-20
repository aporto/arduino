//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

/* 

The circuit requires two wires:
- The first is PIN_SHUTTER, the pin connected to the remote shutter trigger (doorbell or other button)
- The other shall be connected to Arduino GND

When those two pins are shorted (PIN_SHUTTER will read 0) then the command "capture" will be sent
to the Serial port to command digiCamControl (which shall be configured to use Arduino plugin)

*/

const int PIN_SHUTTER = 12;


void setup() {                
	pinMode(13, OUTPUT);  
	
	// Put the reading pin in internal PULLUP mode, so it will read
	// 1 always, except when the two wires are shorted
	pinMode(PIN_SHUTTER, INPUT_PULLUP);
	
	Serial.begin(9600);
}

//---------------------------------------------------------------------------

void loop() {
	if (digitalRead(PIN_SHUTTER) == 0) {		
		Serial.println("capture");
		for (int i=0; i < 5; i++) {
			digitalWrite(13, HIGH);   
			delay(50);            
			digitalWrite(13, LOW);   
			delay(50);            
		}
		while (digitalRead(PIN_SHUTTER) == 0) {
			// simple debounce
			delay(10);
		}
	}	
}

//---------------------------------------------------------------------------