#ifndef WIIMOTIONP__
#define WIIMOTIONP__

#include <Arduino.h>
#include <Wire.h>

class WiiMotion {
public:
	int yaw, pitch, roll; //three axes
	void begin();
	void read();
	void calibrateZeroes();
	
private:	
	byte data[6]; //six data bytes
	int yaw0, pitch0, roll0; //calibration zeroes

	void wmpOn();
	void wmpSendZero();
	
};

#endif
