/*                  Proiect IncalzireVCV_Boiler
  DE FACUT:
    * senzor tempeatura DS sau DHT?
    * tranmisie nRF24L01 cu watchdog de fiecare parte
    * powersaving - idle doar ascultam
    * meniu pt setare temperatura si ceas....FUN!!!!
    * ce setam din remote????
    * encoder sau butoane???? - intai incercam cu butoane...cred
*/

#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> /*Functionare display*/

#define builtinLed PB5           //builtinLed Builtin pentru eroare RTC

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 ecran(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int DS1307 = 0x68;

void setup() {
    while(!ecran.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    PORTB ^= _BV(builtinLed);
    delay(1000);
    }

}

void loop() {

}