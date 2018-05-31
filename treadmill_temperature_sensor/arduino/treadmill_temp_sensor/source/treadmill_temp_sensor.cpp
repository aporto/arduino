//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

byte heart0[8] = {
  B00000,
  B00000,
  B01010,
  B01110,
  B00100,
  B00000,
  B00000,
};

byte heart1[8] = {
  B00000,
  B11011,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
};

char txt[20] = {0};
int heartAnim = 0;
int tempDispCounter = 10;

const int rs = 9, en = 6, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {                
	//pinMode(13, OUTPUT);     
	lcd.createChar(0, heart0);
	lcd.createChar(1, heart1);
	lcd.begin(16, 2);
	lcd.print("Temperat. motor:");
	Serial.begin(9600);
}

//---------------------------------------------------------------------------

void loop() {
	int a = analogRead(A0);
	float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
	float temperature=1/(log(resistance/10000)/ 3975 + 1 / 298.15) -273.15;//convert to temperature via datasheet ;
	memset(txt, 0, sizeof(txt)-1);
	dtostrf(temperature, 3, 1, txt);
	Serial.print(a);
	Serial.print(": ");
	Serial.print(txt);
	Serial.print(": ");
	Serial.println(temperature);

	lcd.setCursor(0,1);
	
	lcd.write(byte(heartAnim));
	++heartAnim;
	if (heartAnim == 2) {
		heartAnim = 0;
	}
	
	if (tempDispCounter > 4) {
		tempDispCounter = 0;
		lcd.print(" ");
		lcd.print(txt);
		lcd.print(" \337C");
	}
	++tempDispCounter;	
	
	delay(500);	
}

//---------------------------------------------------------------------------