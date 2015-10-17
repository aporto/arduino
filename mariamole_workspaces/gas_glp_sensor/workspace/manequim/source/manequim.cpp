//---------------------------------------------------------------------------

#include "mariamole_auto_generated.h"


#define LED_RED_R		2
#define LED_GREEN_R		3
#define LED_BLUE_R		4
#define LED_RED_L		5
#define LED_GREEN_L		6
#define LED_BLUE_L		7

#define RED				0
#define GREEN			1
#define BLUE			2
#define CYAN			3
#define PURPLE			4
#define YELLOW			5
#define WHITE			6


#define LEFT			0
#define RIGHT			1
#define BOTH			2
#define NONE			3

//---------------------------------------------------------------------------

void setup() {           	
	pinMode(LED_RED_R, OUTPUT);     
	pinMode(LED_GREEN_R, OUTPUT);     
	pinMode(LED_BLUE_R, OUTPUT);     
	pinMode(LED_RED_L, OUTPUT);     
	pinMode(LED_GREEN_L, OUTPUT);     
	pinMode(LED_BLUE_L, OUTPUT);    

	digitalWrite(LED_RED_R, LOW);   
	digitalWrite(LED_GREEN_R, LOW);   
	digitalWrite(LED_BLUE_R, LOW);	
	
	digitalWrite(LED_RED_L, LOW);   
	digitalWrite(LED_GREEN_L, LOW);   
	digitalWrite(LED_BLUE_L, LOW);	
	
	Serial.begin(9600);
	
}

//---------------------------------------------------------------------------

void setColor(int color, int side)
{
	bool left = (side == LEFT) || (side == BOTH);
	bool right = (side == RIGHT) || (side == BOTH);
	
	digitalWrite(LED_RED_L, LOW);   
	digitalWrite(LED_GREEN_L, LOW);   
	digitalWrite(LED_BLUE_L, LOW);   

	digitalWrite(LED_RED_R, LOW);   
	digitalWrite(LED_GREEN_R, LOW);   
	digitalWrite(LED_BLUE_R, LOW);   
	
	Serial.println(0);
	
	Serial.print(color);
	Serial.print(":");
	Serial.println(side);
	

	if (side == NONE) {
		return;
	}
	
	
	switch (color) {
		case RED: 
			if (left) {
				Serial.println(1);
				digitalWrite(LED_RED_L, HIGH); 
			} 
			if (right) {
				Serial.println(2);
				digitalWrite(LED_RED_R, HIGH); 
			}
			break;
			
		case GREEN:
			if (left) {
				Serial.println(3);
				digitalWrite(LED_GREEN_L, HIGH); 
			} 
			if (right) {
				Serial.println(4);
				digitalWrite(LED_GREEN_R, HIGH); 
			}
			break;
			
		case BLUE: 	
			if (left) {
				Serial.println(5);
				digitalWrite(LED_BLUE_L, HIGH); 
			} 
			if (right) {
				Serial.println(6);
				digitalWrite(LED_BLUE_R, HIGH); 
			}
			break;
			
		case PURPLE: 	
			if (left) {
				Serial.println(5);
				digitalWrite(LED_RED_L, HIGH); 
				digitalWrite(LED_BLUE_L, HIGH); 
			} 
			if (right) {
				Serial.println(6);
				digitalWrite(LED_RED_R, HIGH); 
				digitalWrite(LED_BLUE_R, HIGH); 
			}
			break;
		
		case CYAN: 	
			if (left) {
				Serial.println(5);
				digitalWrite(LED_GREEN_L, HIGH); 
				digitalWrite(LED_BLUE_L, HIGH); 
			} 
			if (right) {
				Serial.println(6);
				digitalWrite(LED_GREEN_R, HIGH); 
				digitalWrite(LED_BLUE_R, HIGH); 
			}
			break;
			
		case YELLOW: 	
			if (left) {
				Serial.println(5);
				digitalWrite(LED_GREEN_L, HIGH); 
				digitalWrite(LED_RED_L, HIGH); 
			} 
			if (right) {
				Serial.println(6);
				digitalWrite(LED_GREEN_R, HIGH); 
				digitalWrite(LED_RED_R, HIGH); 
			}
			break;
			
		case WHITE: 	
			if (left) {
				Serial.println(5);
				digitalWrite(LED_RED_L, HIGH); 
				digitalWrite(LED_GREEN_L, HIGH); 
				digitalWrite(LED_BLUE_L, HIGH); 
			} 
			if (right) {
				Serial.println(6);
				digitalWrite(LED_RED_R, HIGH); 
				digitalWrite(LED_GREEN_R, HIGH); 
				digitalWrite(LED_BLUE_R, HIGH); 
			}
			break;
	}
}

void loop() {
	/*digitalWrite(LED_RED_R, LOW);   
	digitalWrite(LED_GREEN_R, LOW);   
	digitalWrite(LED_BLUE_R, HIGH);	
	
	digitalWrite(LED_RED_L, LOW);   
	digitalWrite(LED_GREEN_L, LOW);   
	digitalWrite(LED_BLUE_L, HIGH);	
	*/
	
	int color = random(RED, CYAN + 1);
	int time = random(0, 50);
	int side = random(NONE + 1);
	
	setColor(color, side);
	delay(time);            
	
}

//---------------------------------------------------------------------------