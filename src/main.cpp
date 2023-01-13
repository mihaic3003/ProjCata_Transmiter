/*                  Proiect IncalzireVCV_Boiler
  DE FACUT:
    * senzor tempeatura DS sau DHT?
    * tranmisie nRF24L01 cu watchdog de fiecare parte
    * powersaving - idle doar ascultam
    * meniu pt setare temperatura si ceas....FUN!!!!
    * ce setam din remote????
    * encoder sau butoane???? - intai incercam cu butoane...cred
*/

//#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> /*Functionare display*/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define builtinLed PB5           //builtinLed Builtin pentru eroare RTC

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 ecran(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char Dum[] PROGMEM ={"Dum"};
const char Lun[] PROGMEM ={"Lun"};
const char Mar[] PROGMEM ={"Mar"};
const char Mie[] PROGMEM ={"Mie"};
const char Joi[] PROGMEM ={"Joi"};
const char Vin[] PROGMEM ={"Vin"};
const char Sam[] PROGMEM ={"Sam"};
const char *const zile[] PROGMEM = {Dum ,Lun, Mar, Mie, Joi, Vin, Sam};

byte ora = 0;
byte minut = 0;
byte secunda = 0;
byte weekday = 0;
byte zi = 0;
byte luna = 0;
byte an = 0;

const uint8_t coordYPatruRanduri[4] = {0, 16, 32, 48};

uint16_t CentruAliniatSize1(const char* );
uint16_t CentruAliniatSize2(const char* );
byte DecToBcd(byte);
byte BcdToDec(byte);
void CitireTimp();
void ScriereTimp();
void EcranIdle();

const int ModulCeas = 0x68;

void setup() {
  //Serial.begin(9600);

  while(!ecran.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    PORTB ^= _BV(builtinLed);
    delay(1000);
  }
  ecran.setTextColor(SSD1306_WHITE);
  ecran.cp437(true);
  ecran.clearDisplay();
  ecran.display();
  
}

void loop() {
  CitireTimp();
  EcranIdle();
}

uint16_t CentruAliniatSize1(const char *text) {return (64 - (strlen(text)  * 3)); }
uint16_t CentruAliniatSize2(const char *text) {return (64 - (strlen(text)  * 6)); }
byte DecToBcd(byte val) {return ((val/10*16) + (val % 10));}
byte BcdToDec(byte val) {return ((val/16*10) + (val % 16));}

void CitireTimp() {
  Wire.beginTransmission(ModulCeas);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(ModulCeas, 7);
  secunda = BcdToDec(Wire.read());
  minut = BcdToDec(Wire.read());
  ora = BcdToDec(Wire.read());
  weekday = BcdToDec(Wire.read());
  if(weekday == 7) weekday = 0;
  zi = BcdToDec(Wire.read());
  luna = BcdToDec(Wire.read());
  an = BcdToDec(Wire.read());
}

void ScriereTimp(){
  Wire.beginTransmission(ModulCeas);
  Wire.write(byte(0));
  Wire.write(DecToBcd(secunda));
  Wire.write(DecToBcd(minut));
  Wire.write(DecToBcd(ora));
  Wire.write(DecToBcd(weekday));
  Wire.write(DecToBcd(zi));
  Wire.write(DecToBcd(luna));
  Wire.write(DecToBcd(an));
  Wire.write(byte(0));
  Wire.endTransmission();
  // Ends transmission of data */
}

void EcranIdle(){
char acumTampon[3];
ecran.clearDisplay();
ecran.setTextSize(1);
for(uint8_t x = 0; x < 6; x++){
  switch(x){
      case 0:
        ecran.setCursor(0, coordYPatruRanduri[0]);
        strcpy_P(acumTampon, (char *)pgm_read_word(&(zile[weekday])));
        ecran.print(acumTampon);
      break;
      case 1:
        ecran.setCursor(24, coordYPatruRanduri[0]);
        if(zi < 10) ecran.print("0");
        ecran.print(zi);
        ecran.print("/");
        ecran.setCursor(0, coordYPatruRanduri[1]);
        if(ora < 10) ecran.print("0");
        ecran.print(ora);
        ecran.print(":");
      break;
      case 2:
        ecran.setCursor(44, coordYPatruRanduri[0]);
        if(luna < 10) ecran.print("0");
        ecran.print(luna);
        ecran.print("/");
        ecran.setCursor(20, coordYPatruRanduri[1]);
        if(minut < 10) ecran.print("0");
        ecran.print(minut);
        ecran.print(":");
      break;
      case 3:
        ecran.setCursor(64, coordYPatruRanduri[0]);
        ecran.print("20");
        ecran.print(an);
        ecran.setCursor(40, coordYPatruRanduri[1]);
        if(secunda < 10) ecran.print("0");
        ecran.print(secunda);
      break;
   }
}
ecran.display();
delay(10);
}

