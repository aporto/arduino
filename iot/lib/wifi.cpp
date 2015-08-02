

Wifi::Wifi (unsigned char SoftSerialRxPin, 
		unsigned char SoftSerialTxPin, 
		unsigned char SoftSerialBaudRate, 
		unsigned char EspChPdPin, 
		unsigned char EspRstPin)
	: SoftSerial(SOFT_RX, SOFT_TX),
	  wifi(SoftSerial, SoftSerialBaudRate);

{
	m_EspChPdPin = EspChPdPin;
	m_EspRstPin = EspRstPin;	
	
	dbg = false;
}

void Wifi::setDebug(bool debug)
{
	dbg = debug;
}

bool Wifi::setup (void)
{
	pinMode(m_EspChPdPin, OUTPUT);
	pinMode(m_EspChPdPin, OUTPUT);		
	
	digitalWrite(m_EspChPdPin, LOW); 
	digitalWrite(m_EspChPdPin, LOW); 
	
	delay(100);
	
	digitalWrite(m_EspChPdPin, HIGH); 
	digitalWrite(m_EspChPdPin, HIGH); 
	
	delay(100);
	
	if (wifi.setOprToStation()) {
		if (dbg) Serial.println("ESP set to station mode OK");
    } else {
		if (dbg) Serial.println("Failed to set ESP to station mode. It may already by in this mode. Please check you ESP docs");
    }	
	
	 if (wifi.disableMUX()) {
        if (dbg) Serial.println("set single connection mode: Ok");
    } else {
        if (dbg) Serial.println("Failed to set single connection mode");
		return false;
    }
	
	return true;
}

bool Wifi::sendData(unsigned short const * const analogData, int analogCount, 
						unsigned char const * const digitalData, int digitalCount) 
{
    if (wifi.joinAP(SSID, PASSWORD)) {
        if (dbg) Serial.print("Sucessfully connected! IP:");
        if (dbg) Serial.println(wifi.getLocalIP().c_str());    

    } else {
        if (dbg) Serial.println("Connection to AP failed!");
		return false;
    }    
	
	uint8_t buffer[10] = {0};

    if (wifi.createTCP(HOST_ADDRESS, HOST_PORT)) {
        if (dbg) Serial.("create tcp ok\r\n");
    } else {
        if (dbg) Serial.("create tcp err\r\n");
		setStatusLed(STATUS_ERROR);
		return false;
    }

	String req = "GET " //+ //String(HOST_ADDRESS) 
				 + String(HOST_PAGE) + String(data)
				 + " HTTP/1.1\r\nHost: "
				 + String(HOST_ADDRESS) 
				 + "\r\nConnection: close\r\n\r\n";
	wifi.send((const uint8_t *)req.c_str(), req.length());
	debug(req.c_str());
    //char *hello = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    //wifi.send((const uint8_t*)hello, strlen(hello));

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        debug("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        debug("]\r\n");
    }

    if (wifi.releaseTCP()) {
        debug("release tcp ok\r\n");
    } else {
        debug("release tcp err\r\n");		
    }	
	
	if (wifi.leaveAP()) {
        debug("Desconectado com sucesso do roteador!\r\n");		
    } else {
        debug("Erro ao desconectar do roteador");		
    }  
	
	data = "";
	setStatusLed(STATUS_NONE);
	
	return true;
}
