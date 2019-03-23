//---------------------------------------------------------------------------
#include "mariamole_auto_generated.h"

//---------------------------------------------------------------------------
#define PRO_MINI

#define lcd_rs	32
#define lcd_en	34
#define lcd_d4	30
#define lcd_d5	31
#define lcd_d6	33
#define lcd_d7	35

#define key_pin_1		43	//27
#define key_pin_2		41 	//25
#define key_pin_3		42	//26
#define key_pin_4		40	//24
#define key_pin_6		38	//22

//#define STRIP_CKI		47 //41
//#define STRIP_SDI		46 //40

#define CAMERA_IR_REMOTE_CONTROL_PIN	23

#define EXT_BUTTON_GND	24 // 26
#define EXT_BUTTON_INP	25 // 27

#define LCD_LED_PIN		7

#define SD_CHIP_SELECT	53

#define SPEAKER_PIN	8

//#define mode_menu
	
#define menu_image				0
#define menu_speed				1
#define menu_lcd_brightness		2
#define menu_led_brightness		3
#define menu_extrapolation		4
#define menu_mirror				5
#define menu_start_delay		6
#define menu_use_sound			7
#define menu_keep_lcd_led		8
#define menu_send_ir_command	9
#define menu_save_config		10
#define menu_end				11

#define STRIP_LENGTH 			144
#define LED_DATA				46

#define  TONE_1     			3830    // 261 Hz
#define  TONE_2   				1912        // 294 Hz 

//--------------------------------------------------------------------------

LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7); //configura os pinos do arduino para se comunicar com o lcd
MiniKeyboard key (key_pin_1, key_pin_2, key_pin_3, key_pin_4, key_pin_6);

//byte mode = mode_menu;

WS2812 LED(STRIP_LENGTH); 

const char menu_titles[menu_end][16] = {"Image", "Duration", "LCD brightness", "LED brightness",	
										"Extrapolation", "Mirror", "Start delay", 
										"Sound volume", "Keep LCD Light", "Send shot IR", "Save config."};
const char YES_NO[2][4] = {"No", "Yes"};			  

uint8_t imageName[13] = "image.bmp";
int speed = 100;
char lcdBrightness = 80;
char ledBrightness = 100;
char extrapolation = 0;
char mirror = 0;
char startDelay = 0;
char soundVolume = 0;
char keepLCDLight = 0;
char sendIRCommandToCamera = 0;
//unsigned long timeSyncingLed = 0;

unsigned int speedDelays[10] = {0, 100, 200, 400, 700, 1000, 2000, 4000, 7000, 10000};

bool error = false;


//File previousDir;
File root;
File imgFile;
long int sdFileIndex = 0;
long int sdFileCount = 0;

bool usingExternalButton = false;

//uint8_t leds[STRIP_LENGTH][3];

//int temp; //inicia uma variável inteira(temp), para escrever no lcd a contagem do tempo

unsigned long int headerSize;
unsigned long int imgWidth;
unsigned long int imgHeight;

void readImageHeader(void);

void canon_trigger() {
    for(int i=0; i<16; i++) {
        digitalWrite(CAMERA_IR_REMOTE_CONTROL_PIN, HIGH);
        delayMicroseconds(11);
        digitalWrite(CAMERA_IR_REMOTE_CONTROL_PIN, LOW);
        delayMicroseconds(11);
    } 
    delayMicroseconds(7330); 
    for(int i=0; i<16; i++) { 
        digitalWrite(CAMERA_IR_REMOTE_CONTROL_PIN, HIGH);
        delayMicroseconds(11);
        digitalWrite(CAMERA_IR_REMOTE_CONTROL_PIN, LOW);
        delayMicroseconds(11);
    }   
}

bool GetExternalButtonStatus(void)
{
	bool status1 = !(digitalRead(EXT_BUTTON_INP)); // result is inverted because pullup res on input	
	delay(1);
	bool status2 = !(digitalRead(EXT_BUTTON_INP)); // result is inverted because pullup res on input
	
	/*Serial.print("s:");
	Serial.print(status1);
	Serial.print(",");
	Serial.println(status2);*/
	
	if (status1 == status2) {
		return status1;
	} else {
		return false;
	}
	
}

void writeToEEprom(int eepromAddress, char * memoryAddress, unsigned char numOfBytes)
{
	char * ptr = memoryAddress;
	for (unsigned char i=0; i < numOfBytes; i++) {
		EEPROM.write(eepromAddress, *ptr);
		eepromAddress++;
		ptr++;
	}	
}

void readFromEEprom(int eepromAddress, char * memoryAddress, unsigned char numOfBytes)
{
	for (unsigned char i=0; i < numOfBytes; i++) {
		*memoryAddress = EEPROM.read(eepromAddress);
		eepromAddress++;
		memoryAddress++;
	}	
}

void CountDirectoryFiles(void)
{
	sdFileCount = 0;
	while (true) {
		File entry =  root.openNextFile();
		if (!entry) {		
			break;
		}
		sdFileCount++;
		entry.close();
	}
}

void playTone(int tone, int duration) 
{
	analogWrite(SPEAKER_PIN, 0);    
	unsigned long time1 = millis();
	unsigned long deltaTime = millis() - time1;		
	while (duration > deltaTime) {
		analogWrite(SPEAKER_PIN, 255 * soundVolume / 100);
		delayMicroseconds(tone);
		analogWrite(SPEAKER_PIN, 0);
		delayMicroseconds(tone);
		
		deltaTime = millis() - time1;		
	}
}

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
	
	readImageHeader();
}

void RunMenu(void)
{
	bool exit = false;
	char option = 0;
	bool firsttime = true;
	usingExternalButton = false;
	while (exit == false) { 
		key.ReadPins();
		
		// only changes if some key is pressed		
		
		if (GetExternalButtonStatus()) {
			Serial.print("Apertou");
			exit = true;	
			usingExternalButton	= true;
			// If external button is pressed, then keep sending IR shot 
			// command to camera until external button is released,
			// then leave this loop to start the lightpainting imediatelly
            if (sendIRCommandToCamera) {
                if (soundVolume > 0) {
                    for (int i=0; i < 4; i++) {
                        delay(75);		
                        playTone(TONE_2, 75);
                    }
                }
            
                while (GetExternalButtonStatus()) {
                    canon_trigger();
                    playTone(TONE_2, 100);
                    delay(300);				
                }
            }
			break;
		}
		if (firsttime || ( ( (key.Up() || key.Down()) || ( key.Left() || key.Right() ) ) || ( key.Esc() || key.Enter() ) ) ) {
			playTone(TONE_1, 20);
			firsttime = false;
			if (key.Up()) {
				option++;
				if (option >= menu_end) { option = 0;}
			}
			if (key.Down()) {
				option--;
				if (option < 0) { option = menu_end - 1;}
			}
			if (key.Enter()) {
				if ((option == menu_image) && (strcmp(root.name(),"/") != 0)) {
					lcd.clear();
					lcd.print("Back to root...");					
					root = SD.open("/");
					//previousDir = NULL;
					CountDirectoryFiles();
					root.rewindDirectory();
					imgFile =  root.openNextFile();	
					memset(imageName, 0, sizeof(imageName));
					memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
					delay(1000);
				}
			}
			if (key.Esc()) {
				if ((option == menu_image) && imgFile.isDirectory()) {
					if (strcmp(root.name(),"/") == 0) {
						lcd.clear();
						lcd.print("Entering dir...");					
						//previousDir = root;
						root = imgFile; //SD.open((char *)imageName);																
						CountDirectoryFiles();
						root.rewindDirectory();
						imgFile =  root.openNextFile();	
						memset(imageName, 0, sizeof(imageName));
						memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
						delay(1000);
					} else {
						lcd.clear();
						lcd.print("Only one sub-dir");
						lcd.setCursor(0,1);
						lcd.print("is allowed!");
						delay(2000);
					}
				} else if (option == menu_save_config) {
					lcd.clear();
					lcd.print("Saving...");					
					EEPROM.write(0, lcdBrightness);
					EEPROM.write(1, extrapolation);
					EEPROM.write(2, mirror);
					EEPROM.write(3, startDelay);
					EEPROM.write(4, soundVolume);
					EEPROM.write(5, keepLCDLight);
					//EEPROM.write(6, speed);		
					writeToEEprom(6, (char *) &speed, 2);
					EEPROM.write(8, ledBrightness);						
                    EEPROM.write(9, sendIRCommandToCamera);	
					
					delay(1000);
					
					lcd.clear();
				} else {
					exit = true;				
					usingExternalButton	= false;
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
							speed -= 1;
							if (speed < 0) {
								speed = 0;
							}
							if ( (speed * 1000) < (imgHeight * 33 ) ) {
								// minimum time is num_lins_image * 30 milisecs
								speed = int(float(imgHeight) * 33.0 / 1000.0);
								//speed = ceil(float(imgHeight) * 33.0 / 1000.0);
							}
						} else {
							speed += 1;
							if (speed > 90) {
								speed = 90;
							}
						}
					break;

					case menu_lcd_brightness:									
						if (key.Right()) {
							lcdBrightness -= 10;
							if (lcdBrightness < 0) {
								lcdBrightness = 0;
							}
						} else {
							lcdBrightness += 10;
							if (lcdBrightness > 100) {
								lcdBrightness = 100;
							}
						}
					break;	
						
					case menu_led_brightness:			
						if (key.Right()) {
							ledBrightness -= 1;
							if (ledBrightness < 1) {
								ledBrightness = 1;
							}
						} else {
							ledBrightness += 1;
							if (ledBrightness > 100) {
								ledBrightness = 100;
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
						mirror = !mirror;						
					break;							
                    
                    case menu_send_ir_command:
                        sendIRCommandToCamera = !sendIRCommandToCamera;
                    break;
						
					case menu_use_sound:	
						if (key.Right()) {
							soundVolume -= 10;
							if (soundVolume < 0) {
								soundVolume = 00;
							}
						} else {
							soundVolume += 10;
							if (soundVolume > 100) {
								soundVolume = 100;
							}
						}		
					break;							
					
					case menu_keep_lcd_led:			
						keepLCDLight = !keepLCDLight;						
					break;							
				}
			}
			
			//Serial.println(menu_titles[option]);
			lcd.clear();
			lcd.setCursor(0, 0); 
			lcd.print(menu_titles[option]); 			
            //lcd.print(65 + option); 			
			lcd.print(":");			
			
			lcd.setCursor(0, 1); 
			switch (option) {
				case menu_image:
					lcd.setCursor(11,0);
					if (imgFile.isDirectory()) {
						lcd.print("(DIR)");
					} else {
						lcd.print("     ");
					}
					lcd.setCursor(0, 1); 
					lcd.print((char *)imageName); 								
				break;					

				case menu_speed:
					lcd.print((int)speed); 
					lcd.print(" seg (aprox.)"); 
					//lcd.print(" %"); 
				break;

				case menu_lcd_brightness:			
					lcd.print((int)lcdBrightness); 
					lcd.print(" %"); 
					analogWrite(LCD_LED_PIN, 255 * lcdBrightness / 100);
				break;	
				
				case menu_led_brightness:			
					lcd.print((int)ledBrightness); 
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
				break;	
                
                case menu_send_ir_command:
                    lcd.print(YES_NO[sendIRCommandToCamera]);
                    //lcd.print((int) sendIRCommandToCamera);
                break;
				
				case menu_use_sound:
					lcd.print((int)soundVolume); 					
					lcd.print(" %");
				break;	
				
				case menu_keep_lcd_led:
					lcd.print(YES_NO[keepLCDLight]); 					
				break;	
				
				case menu_start_delay:			
					lcd.print((int)startDelay); 
					lcd.print(" sec."); 
				break;	
				
				case menu_save_config:
					lcd.print("Press GO to save"); 
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
	
	
	//Serial.println((char *)imageName);/////
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

void readImageHeader(void)
{
	headerSize = 0;
	imgWidth = 0;
	imgHeight = 0;
	
	imgFile.seek(0);		
	char b = imgFile.read();
	char m = imgFile.read();
	if ((b != 'B') or (m != 'M')) {
		return;
	}
	imgFile.seek(14);
	SDReadInt32(&headerSize);
	SDReadInt32(&imgWidth);
	SDReadInt32(&imgHeight);
	
	if ( (speed * 1000) < (imgHeight * 33 ) ) {
		// minimum time is num_lins_image * 30 milisecs
		speed = int(float(imgHeight) * 33.0 / 1000.0);
		//speed = ceil(float(imgHeight) * 33.0 / 1000.0);
	}
}

/*void DisplaySingleLedRow(void)
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
} */

void ClearLeds(void)
{
	cRGB value;
	value.r = 0;
	value.g = 0;
	value.b = 0;
	
	
	for (int i=0; i < STRIP_LENGTH; i++) {		
		LED.set_crgb_at(i, value);
	}
	LED.sync(); // Sends the data to the LEDs
	
}

/*void DisplaySingleLedRow(void)
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
}*/

void DisplayImage(void)
{
	if (startDelay > 0) {
		int count = startDelay;
		lcd.print("Waiting...");
		while (count > 0) {
			playTone(TONE_1, 50);
			lcd.clear();
			lcd.setCursor(0,1);
			lcd.print(count);
			lcd.print ("   ");
			delay(1000);
			count--;
		}		
	}
	
	//Serial.println("Vai imprimir");
    if (sendIRCommandToCamera == 0) {
        // if send IR command is enable, sound was played before entering this main loop,
        // while IR command was being sent to camera
        if (soundVolume > 0) {
            for (int i=0; i < 4; i++) {
                delay(100);		
                playTone(TONE_2, 100);
            }
        }
	}
    
	if (!keepLCDLight) {
		analogWrite(LCD_LED_PIN, 0);
	}
	
	bool userTerminated = false;
	
	do {			
		lcd.clear();	
		lcd.print(imgFile.name());
		
		readImageHeader();
		if (headerSize == 0) {
			lcd.setCursor(0,1);
			lcd.print("Invalid file!");
			ClearLeds();
			analogWrite(LCD_LED_PIN, 255 * lcdBrightness / 100);
			delay(2000);
			return;
		}
		/*imgFile.seek(0);		
		char b = imgFile.read();
		char m = imgFile.read();
		if ((b != 'B') or (m != 'M')) {
			lcd.setCursor(0,1);
			lcd.print("Invalid file!");
			ClearLeds();
			analogWrite(LCD_LED_PIN, 255 * lcdBrightness / 100);
			delay(2000);
			return;
		}
		imgFile.seek(14);
		unsigned long int headerSize;
		SDReadInt32(&headerSize);
		unsigned long int imgWidth;
		SDReadInt32(&imgWidth);
		unsigned long int imgHeight;
		SDReadInt32(&imgHeight);
		*/
		imgFile.seek(14 + headerSize);	
		int perc = 0;
		
		//unsigned long expectedDelay = speedDelays[9 - speed]; //250 + (speed / 10 - 1) * (750 / 9);
		unsigned long expectedDelay = double(speed * 1000.0) / double(imgHeight);// - timeSyncin= speedDelays[9 - speed]; //250 + (speed / 10 - 1) * (750 / 9);
		if (expectedDelay == 0) {
			lcd.setCursor(0, 1);
			lcd.print("Full speed!");		
		}		
		
		Serial.print("height:");
		Serial.println(imgHeight);
		Serial.print("speed:");
		Serial.println(speed);
		Serial.print("expected delay:");
		Serial.println(expectedDelay);
		
		cRGB value;
		
		unsigned long time1a = millis();
		for (unsigned long int y = 0; y < imgHeight; y++) {
			unsigned long time1 = millis();
			//String line = String(y) + ":";
			
			for (int i=0; i < imgWidth; i++) { //STRIP_LENGTH				
				
				value.g = imgFile.read();
				value.b = imgFile.read();
				value.r = imgFile.read();
				
				if (i < STRIP_LENGTH) {				
					uint8_t idx = i;
						if (mirror) {
						idx = STRIP_LENGTH - 1 - i;
					}
					//if (ledBrightness < 100) {
					//my leds are supper bright. Using only 10% of maximum brigthness (divida by 1000 instead of 100)
					value.r = (unsigned char) (float(value.r) * float(ledBrightness) / 1000.0f);
					value.g = (unsigned char) (float(value.g) * float(ledBrightness) / 1000.0f);
					value.b = (unsigned char) (float(value.b) * float(ledBrightness) / 1000.0f);					
					//}
					LED.set_crgb_at(idx, value);
				}
			}
			if (imgWidth < STRIP_LENGTH) {
				int count = STRIP_LENGTH - imgWidth;
				value.g = 0;
				value.b = 0;
				value.r = 0;
				for (int i=0; i < count; i++) {
					LED.set_crgb_at(i + imgWidth, value);					
				}
			}

			//DisplaySingleLedRow();	
			LED.sync();
			
			/*if (expectedDelay > 0) {
				perc = y * 100 / imgHeight;
				lcd.setCursor(0, 1);
				lcd.print(perc);
				lcd.print(" %   ");		
			}*/
			
			unsigned long deltaTime = millis() - time1;						
			
			//Serial.print("temp actual delta:");		
			//Serial.println(deltaTime);	
			
			key.ReadPins();					
			if (key.Enter() ) { // hack para forcar a ler o teclado quando a velocidade estiver muito alta
				expectedDelay = deltaTime + 1;
			}
			
			if (usingExternalButton) {
				if (GetExternalButtonStatus() == false) {
					expectedDelay = deltaTime + 1;
				}
			}
			
			while (deltaTime < expectedDelay) {
				key.ReadPins();					
				
				deltaTime = millis() - time1;						
				/*if (usingExternalButton) {
					if (GetExternalButtonStatus() == false) {
						userTerminated = true;
						break;
					}
				} else {
					delayMicroseconds(50);
				}*/
				delayMicroseconds(50);
				if (key.Enter() ) {
					userTerminated = true;
					while (key.Enter() ) {
						delayMicroseconds(100); //delay(1);
						key.ReadPins();					
					}				
					
					break;
				}
			}
			if (userTerminated) {
				//Serial.println("2");
				break;
			}
			
			//Serial.print("actual delta:");		
			//Serial.println(deltaTime);
			
		} // end of for loop : imgHeight
		
		unsigned long time2a = millis() - time1a;
		Serial.print("actual delay:");		
		Serial.println(time2a);
		
		
		//Serial.println("3");
		//Serial.println("acabando...");
		if ( (extrapolation == 0) || userTerminated ){
			//Serial.println("4");
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
		
		
		//Serial.println("5");
	} while (true);
	//Serial.println("6");
	
	//memset(leds, 0, sizeof(leds));
	//DisplaySingleLedRow();	
	ClearLeds();
	analogWrite(LCD_LED_PIN, 255 * lcdBrightness / 100);
	
	if (usingExternalButton) {
		playTone(TONE_1, 20);
		while (GetExternalButtonStatus()) {
			delay(50);
		}		
	}
	
	//Serial.println("imprimiu");
}

void setup()
{	
	//pinMode(STRIP_SDI, OUTPUT);
	//pinMode(STRIP_CKI, OUTPUT);
	pinMode(LCD_LED_PIN, OUTPUT);
	pinMode(SPEAKER_PIN, OUTPUT);
	
	pinMode(EXT_BUTTON_GND, OUTPUT);
	digitalWrite(EXT_BUTTON_GND, 0);
	pinMode(EXT_BUTTON_INP, INPUT_PULLUP);
	
	LED.setOutput(LED_DATA); // Digital Pin 7
	LED.setColorOrderBRG();
	
//	#define EXT_BUTTON_GND	26
//#define EXT_BUTTON_INP	27
	
	lcdBrightness = EEPROM.read(0);
	extrapolation = EEPROM.read(1);
	mirror = EEPROM.read(2);
	startDelay = EEPROM.read(3);
	soundVolume = EEPROM.read(4);	
	keepLCDLight = EEPROM.read(5);	
	//speed = EEPROM.read(6);
	readFromEEprom(6, (char *)&speed, 2);
	ledBrightness = EEPROM.read(8);
    sendIRCommandToCamera = EEPROM.read(9);	

	analogWrite(LCD_LED_PIN, 255 * lcdBrightness / 100);
	
	Serial.begin  (9600);
	
	Serial.println("inicio...");	
	
	Serial.print("Stack a:[");
	Serial.print (StackCount());
	Serial.println("] ");
	
	lcd.begin(16, 2); //inicia o lcd com dimensões 16x2(colunas x linhas)
	
	//Serial.print("Stack b:[");
	//Serial.print (StackCount());
	//Serial.println("] ");
	/*lcd.setcursor(0, 0); //posiciona o cursor na primeira coluna(0) e na primeira linha(0) do lcd
	lcd.print("ola garagista!"); //escreve no lcd "olá garagista!"
	lcd.setcursor(0, 1); //posiciona o cursor na primeira coluna(0) e na segunda linha(1) do lcd
	lcd.print("labdegaragem"); //escreve no lcd "labdegaragem"
	*/
	
	//Serial.println("sd1");	
	pinMode(SD_CHIP_SELECT, OUTPUT);
	digitalWrite(SD_CHIP_SELECT, HIGH);
	
	//Serial.println("sd2");
	
	//Serial.print("Stack c:[");
	//Serial.print (StackCount());
	//Serial.println("] ");;	
  
	// see if the card is present and can be initialized:
	if (!SD.begin()) {
		//Serial.println("sd3");	
		lcd.print("SD Card Error!");
		lcd.setCursor(0, 1); 
		lcd.print("Fix and restart");
		error = true;
		return;
	}		

/*	Serial.print("Stack d:[");
/*	Serial.print (StackCount());
	Serial.println("] ");	
	Serial.println(freeMemory());
	
	Serial.println("sd4");	
*/
	root = SD.open("/");	
//	Serial.println("card initialized.");
	
//	Serial.print("Stack e:[");
//	Serial.print (StackCount());
//	Serial.println("] ");
	//Serial.println(freeMemory());
	
	// Count the number of files
	CountDirectoryFiles();
	
	
	//Serial.print("Stack f:[");
	//Serial.print (StackCount());
	//Serial.println("] ");
	
	root.rewindDirectory();
	imgFile =  root.openNextFile();	
	memset(imageName, 0, sizeof(imageName));
	memcpy(imageName, imgFile.name(), sizeof(imageName)-1);
	
	Serial.print("Stack g:[");
	Serial.print (StackCount());
	Serial.println("] ");
	
	GetNextFile(true);
	//GetNextFile(true);
	//GetNextFile(true);	
	
	lcd.clear();
	lcd.print("Purpurina 2");
	lcd.setCursor(0, 1); 
	lcd.print("Files: ");
	lcd.print (sdFileCount);
	delay(500);
	
	//unsigned long time1 = micros();	
	ClearLeds();
	//timeSyncingLed = micros() - time1;
	
	Serial.println("fim do setup!");		
}

void loop()
{
	/*(Serial.print("Stack 0:[");
	Serial.print (StackCount());
	Serial.println("] ");
	delay(1000);
	return;*/
	if (error) {
		delay(1000);
	} else {
		//Serial.print("Stack 1:[");
		//Serial.print (StackCount());
		//Serial.println("] ");
		
		RunMenu();
		DisplayImage();
		
		/*Serial.print("Stack 2:[");
		Serial.print (StackCount());
		Serial.println("] ");
		
		
		Serial.print("Stack 3:[");
		Serial.print (StackCount());
		Serial.println("] ");
		delay(5000);*/
	}
	/*lcd.setcursor(1, 1); //posiciona o cursor na décima quarta coluna(13) e na segunda linha(1) do lcd
	lcd.print(temp); //escreve o valor atual da variável de contagem no lcd
	delay(1000); //aguarda 1 segundo
	temp++; //incrementa variável de contagem
	
	
	
	if(temp == 600) //se a variável temp chegar em 600(10 minutos),...
	{
		temp = 0; //...zera a variável de contagem
	}*/

}b