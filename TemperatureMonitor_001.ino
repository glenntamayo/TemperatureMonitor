//Created by Glenn Gil David Tamayo

/******************** CODE CONFIGURATION *******************/

//#define RTCADJUST

//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
//#include <WiFiUdp.h>`
#include <ArduinoOTA.h>
#include <TiltSensor.h>
#include <Timer.h>
#include <BlynkButton_ggt.h>
#include <BlynkSimpleEsp8266.h>
#include <Tools_ggt.h>
#include <LCD5110_ggt.h>
#include <DHTesp.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif
/******************** CODE CONFIGURATION *******************/


/***************** ARDUINO PIN CONFIGURATION ****************/

const byte scePin = D3;   // SCE - Chip select, pin 3 on LCD.
const byte rstPin = D4;   // RST - Reset, pin 4 on LCD.
const byte dcPin = D0;    // DC - Data/Command, pin 5 on LCD.
const byte sdinPin = D7;  // DN(MOSI) - Serial data, pin 6 on LCD.
const byte sclkPin = D5;  // SCLK - Serial clock, pin 7 on LCD.
const byte blPin = D2;    // LED - Backlight LED, pin 8 on LCD.
const byte dhtPin = D1;
/***************** ARDUINO PIN CONFIGURATION ****************/

/******************** CLASS CONSTRUCTORS ********************/

IPAddress gateway(192,168,1,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network
ESP8266WiFiMulti wifiMulti;
Tools Tools;
LCD5110_ggt Lcd(dcPin, scePin, rstPin, blPin, sdinPin, sclkPin);
DHTesp dht;
Timer t;

BlynkButton sliderBrightness;
BlynkButton sliderContrast;
/******************** CLASS CONSTRUCTORS ********************/

/********************* GLOBAL VARIABLES *********************/
int brightness = 1023;
byte contrast = 40;


/********************* GLOBAL VARIABLES *********************/

void setup(){
  
  WiFi.mode(WIFI_AP_STA);
  wifiMulti.addAP("TAMAYO2.4", "lekleklek");
  Blynk.config("505d5e624e004d76801c0f381206de3f");
  ArduinoOTA.begin();
  
  Serial.begin(115200);
  SerialDhtSetup();

  Lcd.begin(brightness); // This will setup our pins, and initialize the LCD
  
  Lcd.setContrast(177); // Good values range from 40-60
  Lcd.clearDisplay(WHITE);
  Lcd.updateDisplay();

  
  dht.setup(dhtPin, DHTesp::DHT11);

  //t.every(dht.getMinimumSamplingPeriod(), SerialDhtLoop,  (void*)0);
  t.every(2000, LcdDhtLoop,  (void*)0);

  sliderBrightness.setup(cbBrightness, 10);
  sliderContrast.setup(cbContrast, 10);
  
}
  
void loop(){
  static int cursorX = 0;
  static int cursorY = 0;

  t.update();
  
  if (wifiMulti.run() == WL_CONNECTED) { 
    ArduinoOTA.handle();
    if (!Blynk.connected()) {
      Blynk.connect();
    }
    Blynk.run();  
    
  }
  
}  
void LcdDhtLoop(void* context) {
  String lcdStr = "Temp: ";
  
  char outStr[4];
  dtostrf(dht.getTemperature(), 3 , 1, outStr);
  lcdStr.concat(outStr);
  lcdStr.concat(" C");

  lcdStr.concat('\n');
  lcdStr.concat("Humi: ");
  
  char outStr1[4];
  dtostrf(dht.getHumidity(), 3 , 1, outStr1);
  lcdStr.concat(outStr1);
  lcdStr.concat(" %");
  
  char lcdChar[30];
  lcdStr.toCharArray(lcdChar, sizeof(lcdChar));
  Lcd.setStr(lcdChar, 0, 0, BLACK);
  Lcd.updateDisplay();
}

void SerialDhtSetup(){
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);  
}

void SerialDhtLoop(void *context) {
  //DHTesp dht = *(DHTesp*)_dht;
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.println();
  //Serial.print(dht.toCelsius(temperature), 1);
  //Serial.println();
  //Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  //Serial.print("\t\t");
  //Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);  
}
