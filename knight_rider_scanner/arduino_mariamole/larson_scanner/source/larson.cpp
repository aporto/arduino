//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
// Change these values according to your assembly

#define WAVE_LENGHT 4	// Number of leds lit at the same time
#define LED_COUNT	12	// Total number of leds 
#define SWIPE_DELAY	30	// delay between changing the current led lit
#define DECAY_DELAY	20	// delay to fade out each led

// Order of the Arduino pins, as attached to the leds
unsigned char PIN_ORDER[12] = {12, 13, 11, 10, 3, 2, 4, 5, 6, 7, 8, 9};

//---------------------------------------------------------------------------

// No need to change anything from here on
int ledState[54] = {0};
int currentLed = 0;
int direction = 1;
int swipeSpeedCounter = SWIPE_DELAY;
int decaySpeedCounter = DECAY_DELAY;

void setup() {    
	for (int i=0; i < LED_COUNT; i++) {
		pinMode(PIN_ORDER[i], OUTPUT);     	
	}
	Serial.begin(9600);
  	Serial.println("---------");
  	Serial.println("");
}

//---------------------------------------------------------------------------

void loop() {	
  	swipeSpeedCounter += 1;
	if (swipeSpeedCounter >= SWIPE_DELAY) {
		swipeSpeedCounter = 0;
	}
	
	decaySpeedCounter += 1;
	if (decaySpeedCounter >= DECAY_DELAY) {
		decaySpeedCounter = 0;
	}
	
	if (swipeSpeedCounter == 0) {
		currentLed += direction;	
		if (currentLed >= LED_COUNT) {
			currentLed = LED_COUNT - 2;
			direction = -direction;
		}
		if (currentLed <= 0) {
			currentLed = 0;
			direction = -direction;
		}
	}	
	
	for (int i=0; i < LED_COUNT; i++) {
		if (decaySpeedCounter == 0) {
			if (ledState[i] > 0) {
				--ledState[i];
			}
		}
	}	
	
	for (int wavePosition=0; wavePosition < WAVE_LENGHT; wavePosition++) {		
		ledState[currentLed] = WAVE_LENGHT;			
		for (int i=0; i < LED_COUNT; i++) {
			digitalWrite(PIN_ORDER[i], ledState[i] > wavePosition);		
		}		
		
		delayMicroseconds(1000);         
	}	
}

//---------------------------------------------------------------------------