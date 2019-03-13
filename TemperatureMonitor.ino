//Created by Glenn Gil David Tamayo

/******************** CODE CONFIGURATION *******************/

#include "TemperatureMonitor_ggt.h"

#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <Timer.h>
#include <BlynkButton_ggt.h>
#include <BlynkSimpleEsp8266.h>
#include <FS.h>

#ifdef ESP32
#pragma message(THIS CODE IS FOR ESP8266 ONLY!)
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
TemperatureMonitor_ggt TempMon;
IPAddress gateway(192, 168, 1, 1);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network
ESP8266WiFiMulti wifiMulti;

Timer t;

BlynkButton sliderBrightness;
BlynkButton sliderContrast;

WidgetTerminal terminal(V4);
/******************** CLASS CONSTRUCTORS ********************/

/********************* GLOBAL VARIABLES *********************/
char auth[33] = "505d5e624e004d76801c0f381206de3f";
int brightness = 1023;
byte contrast = 40;

float humidity;
float temperature;

const char* filenameLcd5110Settings = "/LCD5110 Settings";

/********************* GLOBAL VARIABLES *********************/

void setup() {
  Serial.begin(115200);
  TempMon.begin(dhtPin, "DHT11");
  TempMon.begin(dhtPin, "DHT11", dcPin, scePin, rstPin, blPin, sdinPin, sclkPin);
  
  spiffSetup();
  //spiffFormat();
  setLcd5110Settings();
  getLcd5110Settings();

  TempMon.setBrightness(brightness);
  TempMon.setContrast(contrast); // Good values range from 40-60
  
  //SerialDhtSetup();

  WiFi.mode(WIFI_AP_STA);
  wifiMulti.addAP("ggdt_android", "Winterfell1");
  wifiMulti.addAP("TAMAYO2.4", "lekleklek");
  Blynk.config(auth);
  ArduinoOTA.begin();

  //t.every(dht.getMinimumSamplingPeriod(), SerialDhtLoop,  (void*)0);
  t.every(2000, LcdDhtLoop,  (void*)0);
  t.every(2000, displayToBlynk, (void*)0);

  sliderBrightness.setup(cbBrightness, 10);
  sliderContrast.setup(cbContrast, 10);

}

void loop() {
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

void spiffSetup() {
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");  
  } else {
    Serial.println("SPIFFS Initialization....failed");  
  }
}

void spiffFormat() {
  //Format File System
  if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}

void setLcd5110Settings() {
  File f = SPIFFS.open(filenameLcd5110Settings, "w");  

  if (!f) {
    Serial.println("LCD5110 Settings file opening failed");  
  } else {
    Serial.println("Write Data to File");
    f.print("LCD 5110 Settings \nbrightness:1023 \ncontrast:40 \nEnd LCD 5110 Settings");
    f.close();  //Close file
    Serial.println("");
    Serial.println("File Closed");
  }
  delay(500);
}

void getLcd5110Settings() {
  File f = SPIFFS.open(filenameLcd5110Settings, "r");  
  String settings = "";

  if (!f) {
    Serial.println("LCD5110 Settings file opening failed");  
  } else {
    Serial.println("File opening succss");
    for(int i = 0; i < f.size(); i++) {
        char mychar = f.read();
        settings.concat(mychar);
      }
      f.close();  //Close file
      Serial.println("File Closed");
      Serial.println(settings);
  }
  
  brightness = getParameter(settings, "brightness");
  contrast = getParameter(settings, "contrast");
  Serial.println(brightness);
  Serial.println(contrast);
  delay(500);
}

int getParameter(String _file, String _parameter) {
  int startIndex = _file.indexOf(_parameter) + _parameter.length();
  int endIndex = _file.indexOf('\n', startIndex);
  String value = _file.substring(startIndex + 1, endIndex - 1);
  return value.toInt();
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
/*
void SerialDhtLoop(void *context) {
  //DHTesp dht = *(DHTesp*)_dht;
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();

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
*/

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
