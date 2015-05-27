//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
#define CH_PD 44 //sinal de controle de CH_PD
#define RST 46 //sinal de controle de RST
#define GPIO0 48 //sinal de controle de GPIO0


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