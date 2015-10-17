/* File automatically created by MariaMole */ 
#ifndef __MINI_KEYBOARD_H__
#define __MINI_KEYBOARD_H__

#include <Arduino.h>

class MiniKeyboard {
public:
	MiniKeyboard (byte pin1, byte pin2, byte pin3, byte pin4, byte pin6);
	void ReadPins(void);
	bool Up(void);
	bool Down(void);
	bool Left(void);
	bool Right(void);
	bool Enter(void);
	bool Esc(void);
private:
	byte pin_1;
	byte pin_2;
	byte pin_3;
	byte pin_4;
	byte pin_6;	

	bool up, down, left, right, enter, esc;

	bool ReadPin(byte pin, bool usePin6);
};


#endif