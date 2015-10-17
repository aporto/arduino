/* File automatically created by MariaMole */ 

#include "MiniKeyboard.h"

MiniKeyboard::MiniKeyboard (byte pin1, byte pin2, byte pin3, byte pin4, byte pin6)
{
	pin_1 = pin1;
	pin_2 = pin2;
	pin_3 = pin3;
	pin_4 = pin4;
	pin_6 = pin6;
	
	pinMode(pin_1, OUTPUT);     
	pinMode(pin_2, INPUT_PULLUP);     
	pinMode(pin_3, INPUT_PULLUP);     
	pinMode(pin_4, INPUT_PULLUP);     
	pinMode(pin_6, OUTPUT);     	
}

bool MiniKeyboard::Left(void)
{
	return left;
}

bool MiniKeyboard::Up(void)
{
	return up;
}

bool MiniKeyboard::Down(void)
{
	return down;
}


bool MiniKeyboard::Right(void)
{
	return right;
}

bool MiniKeyboard::Enter(void)
{
	return enter;
}

bool MiniKeyboard::Esc(void)
{
	return esc;
}


void MiniKeyboard::ReadPins(void)
{
	/*digitalWrite(pin_1, LOW);
	digitalWrite(pin_6, LOW);
	delay(10);		
	*/
	digitalWrite(pin_1, HIGH);
	digitalWrite(pin_6, LOW);		
	delay(10);		
	left = !digitalRead(pin_2);
	up = !digitalRead(pin_3);
	down = !digitalRead(pin_4);
	
	digitalWrite(pin_1, LOW);
	digitalWrite(pin_6, HIGH);		
	delay(10);		
	right = !digitalRead(pin_2);
	enter = !digitalRead(pin_3);
	esc = !digitalRead(pin_4);
	
	digitalWrite(pin_1, LOW);
	digitalWrite(pin_6, LOW);	
}

bool MiniKeyboard::ReadPin(byte pin, bool usePin6)
{
	/*digitalWrite(pin_1, !usePin6);
	digitalWrite(pin_6, usePin6);*/
	digitalWrite(pin_1, LOW);
	digitalWrite(pin_6, LOW);
	
	delayMicroseconds(3000);	
	
	return digitalRead(pin);
}