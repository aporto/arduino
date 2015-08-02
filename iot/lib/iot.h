#ifndef __IOT_H__
#define __IOT_H__

#include <Arduino.h>
#include <ESP8266.h>
#include <NewSoftwareSerial.h>
#include <iot_definitions.h>


namespace iot {
	

class Wifi {
public:
	Wifi (unsigned char SoftSerialRxPin = IOT_ESP_SOFT_RX_PIN, 
		unsigned char SoftSerialTxPin = IOT_ESP_SOFT_TX, 
		unsigned char SoftSerialBaudRate = 9600,
		unsigned char EspChPdPin = IOT_ESP_CH_PD_PIN, 
		unsigned char EspRstPin = IOT_ESP_RST_PIN);
	void setDebug(bool debug);
	bool setup (void);
	bool sendData(unsigned short const * const analogData, int analogCount, 
				unsigned char const * const digitalData, int digitalCount);
{
	 
private:
	SoftwareSerial SoftSerial;
	ESP8266 esp;
	unsigned char m_EspChPdPin, m_EspRstPin;
	bool dbg;	 
 };

} // namespace iot
#endif
