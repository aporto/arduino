#include <WiiMotion.h>

void WiiMotion::wmpOn()
{
	Wire.beginTransmission(0x53); //WM+ starts out deactivated at address 0x53
	Wire.write(0xfe); //send 0x04 to address 0xFE to activate WM+
	Wire.write(0x04);
	Wire.endTransmission(); //WM+ jumps to address 0x52 and is now active
}

void WiiMotion::wmpSendZero()
{
	Wire.beginTransmission(0x52); //now at address 0x52
	Wire.write((byte)0x00); //send zero to signal we want info
	Wire.endTransmission();
}

void WiiMotion::calibrateZeroes()
{
	yaw0 = 0;
	pitch0 = 0;
	roll0 = 0;
	for (int i=0;i<10;i++) {
		wmpSendZero();
		Wire.requestFrom(0x52,6);
		for (int i=0;i<6;i++) {
			data[i]=Wire.read();
		}
		yaw0 += (((data[3]>>2)<<8)+data[0])/10; //average 10 readings
		pitch0 += (((data[4]>>2)<<8)+data[1])/10;
		roll0 += (((data[5]>>2)<<8)+data[2])/10;
	}
	/*Serial.print("Yaw0:");
	Serial.print(yaw0);
	Serial.print(" Pitch0:");
	Serial.print(pitch0);
	Serial.print(" Roll0:");
	Serial.println(roll0);
	*/
}

void WiiMotion::read()
{
	wmpSendZero(); //send zero before each request (same as nunchuck)
	Wire.requestFrom(0x52,6); //request the six bytes from the WM+
	for (int i=0;i<6;i++){
		data[i]=Wire.read();
	}
	yaw=((data[3]>>2)<<8)+data[0]-yaw0;
	pitch=((data[4]>>2)<<8)+data[1]-pitch0;
	roll=((data[5]>>2)<<8)+data[2]-roll0;
}

//see http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus
//for info on what each byte represents

void WiiMotion::begin()
{
	Wire.begin();
	wmpOn(); //turn WM+ on
	calibrateZeroes(); //calibrate zeroes
}