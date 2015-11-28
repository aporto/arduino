#ifndef STACK_CHECK_H__
#define STACK_CHECK_H__
/***************************************************************

Arduino lib based on code by Michael McTernan - July/2007

Usage: Simply call StackCount when you want to check how much
	of the stack has been used UNTIL this call
	
	Note1: that the return value never gets smaller, even when
	stack is freed. StackCount() always return the maximum
	amount of bytes already used at the stack
	
	Note2: StackPaint() is called automatically when the
	microcontroller starts, to paint the whole stack with
	byte '0x5c'. You can change this value, if you need. 
	StackCount() simply counts how many bytes where changed
	from the original 0x5c value	

***************************************************************/
#include <arduino.h>

void StackPaint(void) __attribute__ ((naked)) __attribute__ ((section (".init1")));

uint16_t StackCount(void);

#endif