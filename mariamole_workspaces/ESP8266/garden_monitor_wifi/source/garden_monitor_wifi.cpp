//---------------------------------------------------------------------------
#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
// ESP8266 library example
//
// Put your router password and SSID on an external file, 
// for security reasons. They shall be defined as:
// #define SSID        "ssss"
// #define PASSWORD    "ssss"
#include "../../../../../../../router_password.h"
//---------------------------------------------------------------------------

#define CH_PD 44 //sinal de controle de CH_PD
#define RST 46 //sinal de controle de RST
#define GPIO0 48 //sinal de controle de GPIO0

#define LED_STATUS_RED		2
#define LED_STATUS_GREEN	3
#define LED_STATUS_BLUE		4

#define STATUS_INITIALIZING		0
#define STATUS_CONNECTING_AP	1
#define STATUS_SENDING_DATA		2
#define STATUS_ERROR			255

#include <ESP8266.h>

unsigned char status = STATUS_INITIALIZING;

//Cria objeto de conex?o wifi com o m?dulo, usando a Serial1 do Mega.
ESP8266 wifi(Serial1);

void setStatusLed(unsigned char stat = 254)
{
	if (stat != 254) {
		status = stat;
	}	
	
	digitalWrite(LED_STATUS_RED, LOW); 
	digitalWrite(LED_STATUS_GREEN, LOW); 
	digitalWrite(LED_STATUS_BLUE, LOW); 
	
	delay(200);
	
	if (status == STATUS_CONNECTING_AP) {
		digitalWrite(LED_STATUS_GREEN, HIGH); 
	} else if (status == STATUS_SENDING_DATA) {
		digitalWrite(LED_STATUS_BLUE, HIGH); 
	} else if (status == STATUS_ERROR) {
		digitalWrite(LED_STATUS_RED, HIGH); // error
	}
	
	delay(200);
}
 
void setup()
{
	pinMode(CH_PD,OUTPUT);
	pinMode(RST,OUTPUT);		
	digitalWrite(CH_PD,HIGH); 
	digitalWrite(RST,HIGH); 
	
	pinMode(LED_STATUS_RED,OUTPUT);
	pinMode(LED_STATUS_GREEN,OUTPUT);
	pinMode(LED_STATUS_BLUE,OUTPUT);
	
	setStatusLed(STATUS_INITIALIZING);		
	
    Serial.begin(9600);
    Serial.print("Iniciando Setup...\r\n");    
    Serial.print("Versao de Firmware ESP8266:");
    Serial.println(wifi.getVersion().c_str());
	
	//Serial.println("Lista de APs");
	//Serial.println(wifi.getAPList());
	  
    //Vamos setar o modulo para operar em modo Station (conecta em WiFi) e modo AP (? um ponto de WiFi tambem)
    if (wifi.setOprToStation()) {
        Serial.println("Operacao em modo station ok");
    } else {
        Serial.println("Erro ao colocar o modulo ESP8266 em modo station");
		setStatusLed(STATUS_ERROR);
		return;
    }	
	
	 if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
		setStatusLed(STATUS_ERROR);
    }
}

void sendData(void) 
{
	setStatusLed(STATUS_CONNECTING_AP);
	
	//Agora vamos conectar no ponto de WiFi informado no inicio do codigo, e ver se corre tudo certo
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Conectado com Sucesso!\r\n");
        Serial.print("IP: ");
        //rotina wifi.getLocalIP() retorna o IP usado na conexao com AP conectada.
        Serial.println(wifi.getLocalIP().c_str());    
    } else {
        Serial.print("Falha na conexao AP!\r\n");
		setStatusLed(STATUS_ERROR);
		return;
    }    
	
	setStatusLed(STATUS_SENDING_DATA);
	
	uint8_t buffer[1024] = {0};

    if (wifi.createTCP("www.google.com", 80)) {
        Serial.print("create tcp ok\r\n");
    } else {
        Serial.print("create tcp err\r\n");
		setStatusLed(STATUS_ERROR);
		return;
    }

    char *hello = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    wifi.send((const uint8_t*)hello, strlen(hello));

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }

    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } else {
        Serial.print("release tcp err\r\n");		
    }	
	
	if (wifi.leaveAP()) {
        Serial.println("Desconectado com sucesso do roteador!\r\n");		
    } else {
        Serial.println("Erro ao desconectar do roteador");		
    }  
	
	setStatusLed(STATUS_INITIALIZING);
}

void loop(void)
{
	if (status == STATUS_ERROR) {
		delay(5000);
		setStatusLed(STATUS_INITIALIZING);		
	} else {	
		sendData();
		delay(5000);
	}
}