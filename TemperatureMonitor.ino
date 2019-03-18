//Created by Glenn Gil David Tamayo

/******************** CODE CONFIGURATION *******************/

#include "TemperatureMonitor_ggt.h"

#include <ArduinoOTA.h>
#include <Timer.h>
#include <BlynkButton_ggt.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
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
TemperatureMonitor_ggt TempMon;

Timer t;

BlynkButton sliderBrightness;
BlynkButton sliderContrast;

WidgetTerminal terminal(V4);
/******************** CLASS CONSTRUCTORS ********************/

/********************* GLOBAL VARIABLES *********************/
char auth[33] = "505d5e624e004d76801c0f381206de3f";

float humidity;
float temperature;
int brightness;
byte contrast;


/********************* GLOBAL VARIABLES *********************/

void setup() {
  Serial.begin(115200);
  TempMon.begin(dhtPin, "DHT11", dcPin, scePin, rstPin, blPin, sdinPin, sclkPin);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  brightness = TempMon.setBrightness("Settings");
  contrast = TempMon.setContrast("Settings"); // Good values range from 40-60
  
  Blynk.config(auth);
  ArduinoOTA.begin();

  //t.every(dht.getMinimumSamplingPeriod(), SerialDhtLoop,  (void*)0);
  t.every(2000, LcdDhtLoop,  (void*)0);
  t.every(2000, displayToBlynk, (void*)0);

  sliderBrightness.setup(cbBrightness, 10);
  sliderContrast.setup(cbContrast, 10);

}

void loop() {
  
  t.update();

  
  ArduinoOTA.handle();
  if (!Blynk.connected()) {
    Blynk.connect();
  }
  Blynk.run();


}

void SerialDhtSetup() {
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);
}

void LcdDhtLoop(void* context) {
  String lcdStr = "Temp: ";
  temperature = TempMon.getTemperature();
  humidity = TempMon.getHumidity();

  char outStr[4];
  dtostrf(temperature, 3 , 1, outStr);
  lcdStr.concat(outStr);
  lcdStr.concat(" C");

  lcdStr.concat('\n');
  lcdStr.concat("Humi: ");

  char outStr1[4];
  dtostrf(humidity, 3 , 1, outStr1);
  lcdStr.concat(outStr1);
  lcdStr.concat(" %");

  char lcdChar[30];
  lcdStr.toCharArray(lcdChar, sizeof(lcdChar));
  
  TempMon.displayOut(lcdChar);
}

void displayToBlynk(void* context) {
  Blynk.virtualWrite(V2, (int)temperature);
  Blynk.virtualWrite(V3, (int)humidity);
  Serial.println(temperature);
}

BLYNK_CONNECTED() {

}

void cbBrightness(void* context) {
  brightness = sliderBrightness.state;
  Blynk.virtualWrite(V0, sliderBrightness.state); //return to released state no slide
  Serial.println(brightness);
  TempMon.setBrightness(brightness);

  //Serial.println(brightness);
}

void cbContrast(void* context) {
  contrast = sliderContrast.state;
  Blynk.virtualWrite(V1, sliderContrast.state); //return to released state no slide
  TempMon.setContrast(contrast);
  Serial.println(contrast);
}

BLYNK_WRITE(V0) {
  sliderBrightness.collect(param.asInt());
  sliderBrightness.sWitch();
}
BLYNK_WRITE(V1) {
  sliderContrast.collect(param.asInt());
  sliderContrast.sWitch();
}

BLYNK_WRITE(V2) {
  sliderContrast.collect(param.asInt());
  sliderContrast.sWitch();
}

BLYNK_WRITE(V4) {
String incoming = param.asStr();
if (String("reset") == incoming) {
  terminal.println("Restarting... ");
  terminal.flush();
  ESP.restart();
} else if (String("macAddress") == incoming) {
  terminal.println("MAC Address: ");
  terminal.println(WiFi.macAddress());
} else if (String("blynkAuth") == incoming) {
  terminal.println("Blynk Auth Token: ");
  terminal.println(auth);
  terminal.println(strlen(auth));
}

terminal.flush();
}
