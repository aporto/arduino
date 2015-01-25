/**************************************************************************************************
 * acceleroMMA7361.h - Library for retrieving data from the MMA7361 accelerometer. Only tested 	  *
 * with Duemilanove                                                                               *
 * Following functions need improving. I asumed the output to be linear (it is nearly linear but  *
 * not really). I also asumed the Arduino input to be scaled 0-5V                                 *
 * begin variables                                                                                *
 *  -int sleepPin: number indicating to which pin the sleep port is attached. DIGITAL OUT         *
 *  -int selfTestPin: number indicating to which pin the selftest port is attached.DIGITAL OUT    *
 *  -int zeroGPin: number indicating to which pin the ZeroGpin is connected to. DIGITAL IN        *
 *  -int xPin: number indicating to which pin the x-axis pin is connected to. ANALOG IN           *
 *  -int yPin: number indicating to which pin the y-axis  pin is connected to. ANALOG IN          *
 *  -int zPin: number indicating to which pin the z-axis  pin is connected to. ANALOG IN          *
 *  -int offset: array indicating the G offset on the x,y and z-axis                              *
 * Functions currently present:                                                                   *
 *  -getXRaw(): Instantly returns the raw data from the X-axis analog I/O port of the Arduino as  *
 *    a double. WARNING! OFFSET HAS NO INFLUENCE                                                  *
 *  -getYRaw(): Instantly returns the raw data from the Y-axis analog I/O port of the Arduino as  *
 *    a double. WARNING! OFFSET HAS NO INFLUENCE                                                  *
 *  -getZRaw(): Instantly returns the raw data from the Z-axis analog I/O port of the Arduino as  *
 *    a double. WARNING! OFFSET HAS NO INFLUENCE                                                  *
 *  -getXVolt(): Instantly returns the voltage in miliVolts from the X-axis analog I/O port of    *
 *    the Arduino as a double. WARNING! OFFSET HAS NO INFLUENCE                                   *
 *  -getYVolt(): Instantly returns the voltage in miliVolts from the Y-axis analog I/O port of    *
 *    the Arduino as a double. WARNING! OFFSET HAS NO INFLUENCE                                   *
 *  -getZVolt(): Instantly returns the voltage in miliVolts from the Z-axis analog I/O port of    *
 *    the Arduino as a double. WARNING! OFFSET HAS NO INFLUENCE                                   *
 *  -getXAccel(): Instantly returns the acceleration of the X-axis as a double (1G = 100.00)      *
 *  -getYAccel(): Instantly returns the acceleration of the Y-axis as a double (1G = 100.00)      *
 *  -getXAccel(): Instantly returns the acceleration of the Z-axis as a double (1G = 100.00)      *
 **************************************************************************************************
 * Revision History:                                                                              *
 *  Version 0.1: get raw values                                                                   *
 *  Version 0.2: get voltages and G forces                                                        *
 **************************************************************************************************
 * Created by Jef Neefs: Suggestions, questions or comments please contact me                     *
 *  -mail: neefsj at gmail dot com                                                                *
 *  -skype: studioj                                                                               *
 **************************************************************************************************/
#ifndef AcceleroMMA7361_h
#define AcceleroMMA7361_h
#include <Arduino.h>//<WProgram.h>

class AcceleroMMA7361
{
  public:
    AcceleroMMA7361();
    void begin(int sleepPin, int selfTestPin, int zeroGPin, int xPin, int yPin, int zPin, int xOffset, int yOffset, int zOffset);
    double getXRaw();
    double getYRaw();
    double getZRaw();
    double getXVolt();
    double getYVolt();
    double getZVolt();
    double getXAccel();
    double getYAccel();
    double getZAccel();
    double getTotalVector();
	
  private:
	long _mapMMA7361V(int value);
	long _mapMMA7361G(int value);
    int _sleepPin;
	int _selfTestPin;
	int _zeroGPin;
	int _xPin;
	int _yPin;
	int _zPin;
	int _xOffset;
	int _yOffset;
	int _zOffset;
};
#endif

