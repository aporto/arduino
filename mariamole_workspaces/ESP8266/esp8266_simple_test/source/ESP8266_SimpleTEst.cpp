//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

// do not change these three defines!!!
#define BOARD_TYPE_UNO			0
#define BOARD_TYPE_MINI_NEW		1
#define BOARD_TYPE_MINI_OLD		2

// Select here the board type
#define BOARD_TYPE BOARD_TYPE_UNO

#include "c:/git_hub/router_password.h"

#if BOARD_TYPE == BOARD_TYPE_UNO
		#define CH_PD 			10 
		#define RST 			11 
		#define SOFT_RX			9 		// connected to ESP TX
		#define SOFT_TX			12 		// connected to ESP RX*/
#else
	#if BOARD_TYPE == BOARD_TYPE_MINI_NEW
		#define CH_PD 			10 
		#define RST 			11 
		#define SOFT_RX			9 		// connected to ESP TX
		#define SOFT_TX			12 		// connected to ESP RX*/
	#else 
		// BOARD_TYPE == BOARD_TYPE_MINI_OLD
		#define CH_PD 			11 
		#define RST 			10 
		#define SOFT_RX			9 		// connected to ESP TX
		#define SOFT_TX			8 		// connected to ESP RX*/
	#endif
#endif


SoftSerial Serial1;

void setup() {
	Serial.begin(9600);
	Serial1.begin(9600);
	
	pinMode(CH_PD,OUTPUT);
	pinMode(RST,OUTPUT);
	
	digitalWrite(CH_PD,HIGH); //Setado em alto - funcionamento normal
	digitalWrite(RST,HIGH); //RST em alto - funcionamento normal
}

void loop() {
   while (Serial1.available()) {
       Serial.write(Serial1.read());
   }

   while (Serial.available()) {
       Serial1.write(Serial.read());
   }

}
//---------------------------------------------------------------------------