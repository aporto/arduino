//---------------------------------------------------------------------------
#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------

#define lcd_rs	32
#define lcd_en	34
#define lcd_d4	30
#define lcd_d5	31
#define lcd_d6	33
#define lcd_d7	35

#define key_pin_1	27
#define key_pin_2	25
#define key_pin_3	26
#define key_pin_4	24
#define key_pin_6	22

#define mode_menu

#define menu_image			0
#define menu_speed			1
#define menu_brightness		2
#define menu_extrapolation	3
#define menu_mirror			4
#define menu_start_delay	5
#define menu_end			6

#define STRIP_LENGTH 	32
#define STRIP_CKI		41
#define STRIP_SDI		40


//---------------------------------------------------------------------------

LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7); //configura os pinos do arduino para se comunicar com o lcd
MiniKeyboard key (key_pin_1, key_pin_2, key_pin_3, key_pin_4, key_pin_6);

//byte mode = mode_menu;

const char menu_titles[menu_end][15] = {"Image", "Speed", "Led brightness", "Extrapolation", "Mirror", "Start delay"};
const char YES_NO[2][4] = {"No", "Yes"};

uint8_t imageName[13] = "image.bmp";
unsigned char speed = 100;
unsigned char ledBrightness = 80;
char extrapolation = 0;
char mirror = 0;
char startDelay = 0;

bool error = false;

File root;
File imgFile;
long int sdFileIndex = 0;
long int sdFileCount = 0;

uint8_t leds[STRIP_LENGTH][3];

//int temp; //inicia uma variável inteira(temp), para escrever no lcd a contagem do tempo

void GetNextFile(bool forward)
{	
	if (sdFileCount < 2) {
		return;
	}
	
	
	char step = -1;
	if (forward) {
		step = 1;
	}
	
	if (forward) {
		++sdFileIndex;
		if (sdFileIndex >= sdFileCount) {
			sdFileIndex = 0;
			root.rewindDirectory();			
		}
		imgFile =  root.openNextFile();	
	} else {
		--sdFileIndex;
		if (sdFileIndex < 0) {
			sdFileIndex = sdFileCount - 1;
		}
		root.rewindDirectory();			
		for (int i=0; i <= sdFileIndex; i++) {
			imgFile =  root.openNextFile();	
		}
	}
	
	memset(imageName, 0, sizeof(imageName));
	memcpy(imageName, imgFile.name(), sizeof(imageName)-1);					
}

void RunMenu(void)
{
	bool exit = false;
	char option = 0;
	//bool la//stkey[6];
	//memset(lastkey, 0, sizeof(lastkey));
	bool firsttime = true;
	while (exit == false) { 
		key.ReadPins();
		
		// only changes if some key is pressed		
		if (firsttime || ( ( (key.Up() || key.Down()) || ( key.Left() || key.Right() ) ) || ( key.Esc() || key.Enter() ) ) ) {
			firsttime = false;
			if (key.Up()) {
				option++;
				if (option >= menu_end) { option = 0;}
			}
			if (key.Down()) {
				option--;
				if (option < 0) { option = menu_end - 1;}
			}
			if (key.Esc()) {
				if (imgFile.isDirectory()) {
					//lcd.print("*");
				} else {
					exit = true;				
					break;
				}
			}
			
			if (key.Right() || key.Left()) {
				switch (option) {
					case menu_image:
						if (key.Right()) {
							GetNextFile(false);							
						} else {
							GetNextFile(true);
						}							
						
					break;					

					case menu_speed:
						if (key.Right()) {
							if (speed > 10) {
								speed -= 10;
							}
						} else {
							if (speed < 500) {
								speed += 10;
							}
						}
					break;

					case menu_brightness:			
						if (key.Right()) {
							if (ledBrightness > 5) {
								ledBrightness -= 5;
							}
						} else {
							if (ledBrightness < 100) {
								ledBrightness += 5;
							}
						}
					break;	
						
					case menu_start_delay:			
						if (key.Right()) {
							if (startDelay > 0) {
								startDelay -= 1;
							}
						} else {
							if (startDelay < 20) {
								startDelay += 1;
							}
						}
					break;	
						
					case menu_extrapolation:			
						if (key.Right()) {
							if (extrapolation > 0) {
								extrapolation -= 1;
							}
						} else {
							if (extrapolation < 2) {
								extrapolation += 1;
							}
						}
					break;	
						
					case menu_mirror:			
						if (key.Right()) {
							if (mirror > 0) {
								mirror -= 1;
							}
						} else {
							if (mirror < 1) {
								mirror += 1;
							}
						}
					break;							
				}
			}
			
			lcd.clear();
			lcd.setCursor(0, 0); 
			lcd.print(menu_titles[option]); //escreve no lcd "olá garagista!"
			lcd.setCursor(0, 1); 
			switch (option) {
				case menu_image:
					if (imgFile.isDirectory()) {
						lcd.print("*");
					}
					lcd.print((char *)imageName); 
				break;					

				case menu_speed:
					lcd.print(speed); 
					lcd.print(" %"); 
				break;

				case menu_brightness:			
					lcd.print(ledBrightness); 
					lcd.print(" %"); 
				break;	
				
				case menu_extrapolation:
					switch (extrapolation) {
						case 0: lcd.print("Turn leds off"); break;
						case 1: lcd.print("Repeat"); break;
						case 2: lcd.print("Next image"); break;								
					}

				break;	
				
				case menu_mirror:			
					lcd.print(YES_NO[mirror]); 
					lcd.print(" %"); 
				break;	
				
				case menu_start_delay:			
					lcd.print(startDelay); 
					lcd.print(" sec."); 
				break;	

			}
			
			// wait until key is released
			while ( ( (key.Up() || key.Down()) || ( key.Left() || key.Right() ) ) || ( key.Esc() || key.Enter() ) ) {
				delay(50);
				key.ReadPins();			
			}		
		}
		
		
		delay(50);
	}
	
	lcd.clear();
}

bool SDReadInt16(unsigned int * ptr)
{
	uint8_t * ptrArray = (uint8_t *)ptr;
	ptrArray[0] = imgFile.read();
	ptrArray[1] = imgFile.read();	
	
	return true;
}

bool SDReadInt8(unsigned char * ptr)
{
	uint8_t * ptrArray = (uint8_t *)ptr;
	ptrArray[0] = imgFile.read();	
	
	return true;	
}

bool SDReadInt32(unsigned long int * ptr)
{
	uint8_t * ptrArray = (uint8_t *)ptr;
	ptrArray[0] = imgFile.read();
	ptrArray[1] = imgFile.read();	
	ptrArray[2] = imgFile.read();
	ptrArray[3] = imgFile.read();	
	
	return true;
}

void DisplaySingleLedRow(void)
{	
	//Each LED requires 24 bits of data
  //MSB: R7, R6, R5..., G7, G6..., B7, B6... B0 
  //Once the 24 bits have been delivered, the IC immediately relays these bits to its neighbor
  //Pulling the clock low for 500us or more causes the IC to post the data.

  for(int LED_number = 0 ; LED_number < STRIP_LENGTH ; LED_number++) {
    long this_led_color;// = strip_colors[LED_number]; //24 bits of color data
	memcpy(&this_led_color, &(leds[LED_number]), 4);	  

    for(byte color_bit = 23 ; color_bit != 255 ; color_bit--) {
      //Feed color bit 23 first (red data MSB)
      
      digitalWrite(STRIP_CKI, LOW); //Only change data when clock is low
      
      long mask = 1L << color_bit;
      //The 1'L' forces the 1 to start as a 32 bit number, otherwise it defaults to 16-bit.
      
      if(this_led_color & mask) 
        digitalWrite(STRIP_SDI, HIGH);
      else
        digitalWrite(STRIP_SDI, LOW);
  
      digitalWrite(STRIP_CKI, HIGH); //Data is latched when clock goes high
    }
  }

  //Pull clock low to put strip into reset/post mode
  digitalWrite(STRIP_CKI, LOW);
  delayMicroseconds(500); //Wait for 500us to go into reset
}

void DisplayImage(void)
{
	lcd.clear();
	if (startDelay > 0) {
		int count = startDelay;
		lcd.print("Waiting...");
		while (count > 0) {
			lcd.setCursor(0,1);
			lcd.print(count);
			lcd.print ("   ");
			delay(1000);
			count--;
		}
		lcd.clear();
	}
	
	do {
		lcd.print(imgFile.name());
		//Serial.println (imgFile.name());	
		imgFile.seek(0);		
		char b = imgFile.read();
		char m = imgFile.read();
		if ((b != 'B') or (m != 'M')) {
			lcd.print("Invalid file!");
			delay(2000);
			Serial.println(b);
			Serial.println(m);
			return;
		}
		
		imgFile.seek(14);
		unsigned long int headerSize;
		SDReadInt32(&headerSize);
		unsigned long int imgWidth;
		SDReadInt32(&imgWidth);
		unsigned long int imgHeight;
		SDReadInt32(&imgHeight);
		
		Serial.print("header size "); Serial.println(headerSize);
		Serial.print("imgWidth "); Serial.println(imgWidth);
		Serial.print("imgHeight "); Serial.println(imgHeight);
		
		if (imgWidth != STRIP_LENGTH) {
			lcd.print("Invalid width");
			delay(2000);
			return;		
		}
		
		imgFile.seek(14 + headerSize);	
		int perc = 0;
		
		lcd.print("Printing...");	
		unsigned long expectedDelay = 250 + (speed / 10 - 1) * (750 / 9);
		
		for (unsigned long int y = 0; y < imgHeight; y++) {
			unsigned long time1 = millis();
			//String line = String(y) + ":";
			for (int i=0; i < STRIP_LENGTH; i++) {
				char idx = i;
				if (mirror) {
					idx = STRIP_LENGTH - 1 - i;
				}
				leds[idx][2] = imgFile.read();
				leds[idx][1]= imgFile.read();
				leds[idx][0]= imgFile.read();
			}

			DisplaySingleLedRow();	
			
			perc = y * 100 / imgHeight;
			lcd.setCursor(0, 1);
			lcd.print(perc);
			lcd.print(" %   ");		
			
			unsigned long deltaTime = millis() - time1;		
			if (expectedDelay > deltaTime) {
				delay (expectedDelay - deltaTime);
			}
			key.ReadPins();					
			if (key.Enter() ) {
				break;
			}
		}
		
		if (extrapolation == 0) {
			break;
		} else if (extrapolation == 2) {
			if (sdFileIndex < sdFileCount - 1) {
				imgFile = root.openNextFile();
				memset(imageName, 0, sizeof(imageName));
				memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
				sdFileIndex++;
			}
			if (!imgFile) {
				root.rewindDirectory();
				imgFile =  root.openNextFile();	
				memset(imageName, 0, sizeof(imageName));
				memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
				break;
			}
		}
		
		
	} while (true);
	
	memset(leds, 0, sizeof(leds));
	DisplaySingleLedRow();	
}

void setup()
{
	
	pinMode(STRIP_SDI, OUTPUT);
	pinMode(STRIP_CKI, OUTPUT);
	
	Serial.begin  (9600);
	Serial.println("inicio...");	
	
	lcd.begin(16, 2); //inicia o lcd com dimensões 16x2(colunas x linhas)
	/*lcd.setcursor(0, 0); //posiciona o cursor na primeira coluna(0) e na primeira linha(0) do lcd
	lcd.print("ola garagista!"); //escreve no lcd "olá garagista!"
	lcd.setcursor(0, 1); //posiciona o cursor na primeira coluna(0) e na segunda linha(1) do lcd
	lcd.print("labdegaragem"); //escreve no lcd "labdegaragem"
	*/
	
	pinMode(53, OUTPUT);
	digitalWrite(53, HIGH);
  
	// see if the card is present and can be initialized:
	if (!SD.begin()) {
		lcd.print("SD Card Error!");
		lcd.setCursor(0, 1); 
		lcd.print("Fix and restart");
		error = true;
		return;
	}		   

	root = SD.open("/");	
	Serial.println("card initialized.");
	
	// Count the number of files
	sdFileCount = 0;
	while (true) {
		File entry =  root.openNextFile();
		if (!entry) {		
			break;
		}
		sdFileCount++;
		entry.close();
	}
	
	root.rewindDirectory();
	imgFile =  root.openNextFile();	
	memset(imageName, 0, sizeof(imageName));
	memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
	
	lcd.clear();
	lcd.print("Purpurina 2");
	lcd.setCursor(0, 1); 
	lcd.print("Files: ");
	lcd.print (sdFileCount);
	delay(500);
	
	Serial.println("fim...");		
}

void loop()
{
	if (error) {
		delay(1000);
	} else {
		RunMenu();
		DisplayImage();
	}
	/*lcd.setcursor(1, 1); //posiciona o cursor na décima quarta coluna(13) e na segunda linha(1) do lcd
	lcd.print(temp); //escreve o valor atual da variável de contagem no lcd
	delay(1000); //aguarda 1 segundo
	temp++; //incrementa variável de contagem
	
	
	
	if(temp == 600) //se a variável temp chegar em 600(10 minutos),...
	{
		temp = 0; //...zera a variável de contagem
	}*/

}