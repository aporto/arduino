#include "mariamole_auto_generated.h"
/*
 MIDI note player
 
 This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
 If this circuit is connected to a MIDI synth, it will play 
 the notes F#-0 (0x1E) to F#-5 (0x5A) in sequence.

 
 The circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

 created 13 Jun 2006
 modified 13 Aug 2012
 by Tom Igoe 

 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Midi
 
 */

void setup() {
	//  Set MIDI baud rate:
	//Serial.begin(31250);
	Serial.begin(115200);
	
	pinMode(0, INPUT_PULLUP);
}

#define NOTE_OFF       0x80
#define NOTE_ON        0x90
#define CC             0xB0

#define CHANNEL  10

int controllers[6] = { 
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15
};

typedef struct {
  uint8_t status;   // first  byte   : status message (NOTE_ON, NOTE_OFF or CC (controlchange) and midi channel (0-15)
  uint8_t data1;    // second byte   : first value (0-127), controller number or note number
  uint8_t data2;    // third  byte   : second value (0-127), controller value or velocity
} 

t_midiMsg; 

int oldValue = -1;

void loop() {
	t_midiMsg msg;  
	int sensorValue = analogRead(A0);
	if (sensorValue != oldValue) {
		msg.status = CC;		
		msg.status = msg.status | (CHANNEL-1);  
		msg.data1  = controllers[0];                  // Get the controller number from the array above.
		msg.data2  = sensorValue / 8;                    // Get the value of the analog input from the analogVal array.
		//Serial.write((uint8_t *)&msg, sizeof(msg));    // Send the MIDI message.
		midi_controller_change(CHANNEL-1, controllers[0], sensorValue/8);
	}
	oldValue = sensorValue;
  // play notes from F#-0 (0x1E) to F#-5 (0x5A):
  /*for (int note = 0x1E; note < 0x5A; note ++) {
    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(0x90, note, 0x45);
    delay(100);
    //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
    noteOn(0x90, note, 0x00);   
    delay(100);
  }*/
	delay(20);
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

