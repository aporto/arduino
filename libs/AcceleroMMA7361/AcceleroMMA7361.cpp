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
//#include <WProgram.h>
#include <AcceleroMMA7361.h>

AcceleroMMA7361::AcceleroMMA7361()
{
}

void AcceleroMMA7361::begin(int sleepPin, int selfTestPin, int zeroGPin, int xPin, int yPin, int zPin, int xOffset, int yOffset, int zOffset)
{
  pinMode(sleepPin, OUTPUT);
  pinMode(selfTestPin, OUTPUT);
  pinMode(zeroGPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(zPin, INPUT);
  digitalWrite(sleepPin,HIGH);
  digitalWrite(selfTestPin,LOW);
  _sleepPin = sleepPin;
  _selfTestPin = selfTestPin;
  _zeroGPin = zeroGPin;
  _xPin = xPin;
  _yPin = yPin;
  _zPin = zPin;
  _xOffset = xOffset;
  _yOffset = yOffset;
  _zOffset = zOffset;
}

double AcceleroMMA7361::getXRaw()
{
  return analogRead(_xPin);
}

double AcceleroMMA7361::getYRaw()
{
  return analogRead(_yPin);
}

double AcceleroMMA7361::getZRaw()
{
  return analogRead(_zPin);
}

double AcceleroMMA7361::getXVolt()
{
  return _mapMMA7361V(analogRead(_xPin));
}

double AcceleroMMA7361::getYVolt()
{
  return _mapMMA7361V(analogRead(_yPin));
}

double AcceleroMMA7361::getZVolt()
{
  return _mapMMA7361V(analogRead(_zPin));
}

double AcceleroMMA7361::getXAccel()
{
  return _mapMMA7361G(analogRead(_xPin))+ _xOffset;
}

double AcceleroMMA7361::getYAccel()
{
  return _mapMMA7361G(analogRead(_yPin)) + _yOffset;
}

double AcceleroMMA7361::getZAccel()
{
  return _mapMMA7361G(analogRead(_zPin)) + _zOffset;
}

long AcceleroMMA7361::_mapMMA7361V(int value)
{
  return map(value,0,1024,0,3300);
}

long AcceleroMMA7361::_mapMMA7361G(int value)
{
  return map(value,0,1024,-800,1600);
}
