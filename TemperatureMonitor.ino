//Created by Glenn Gil David Tamayo

/******************** CODE CONFIGURATION *******************/


#include "TemperatureMonitor_ggt.h"

#include <ArduinoOTA.h>
#include <Timer.h>
#include <BlynkButton_ggt.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>
#include <Time.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
//#include <ESP8266WebServer.h>
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
TemperatureMonitor TempMon;

Timer t;

BlynkButton sliderBrightness;
BlynkButton sliderContrast;

WidgetTerminal terminal(V4);
WidgetRTC rtc;
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

  TempMon.displayOut("Connecting  to WiFi...");
  
  brightness = TempMon.setBrightness("Settings");
  contrast = TempMon.setContrast("Settings"); // Good values range from 40-60
  
  Blynk.config(auth);
  ArduinoOTA.begin();

  //t.every(dht.getMinimumSamplingPeriod(), SerialDhtLoop,  (void*)0);
  t.every(2000, LcdDhtLoop,  (void*)0);
  t.every(2000, displayToBlynk, (void*)0);

  sliderBrightness.setup(cbBrightness, 10);
  sliderContrast.setup(cbContrast, 10);

  WiFiManager wifiManager;
  wifiManager.setTimeout(3);
  //wifiManager.autoConnect("AutoConnectAP");
  if(!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    TempMon.displayOut("Failed to connect and timeout hit.");
    delay(1000);
    TempMon.displayOut("Restarting...");
    delay(1000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
  } 
}

void loop() {
  
  t.update();

  ArduinoOTA.handle();
  
  if (!Blynk.connected()) {
    Blynk.connect();
  }
  Blynk.run();


}

void LcdDhtLoop(void* context) {

  byte cursorY = 0;

  TempMon.displayOut(getTime(), cursorY++);
  if (timeStatus() == 0) {
    TempMon.displayOut("Time not set", cursorY++);  
  } else if(timeStatus() == 1) {
    TempMon.displayOut("Time needs to sync", cursorY++);
  }
  
  String strTemp = "";
  strTemp.concat("Temp: ");
  temperature = TempMon.getTemperature();
  strTemp.concat((int)temperature);
  strTemp.concat(" C");
  TempMon.displayOut(strTemp, cursorY++);

  String strHumi = "";
  strHumi.concat("Humi: ");
  humidity = TempMon.getHumidity();
  strHumi.concat((int)humidity);
  strHumi.concat(" %");
  TempMon.displayOut(strHumi, cursorY++);

  for (int i = cursorY; i < 6; i++) {
    TempMon.displayOut("", i);  
  }
  
}

void displayToBlynk(void* context) {
  Blynk.virtualWrite(V2, (int)temperature);
  Blynk.virtualWrite(V3, (int)humidity);
  //Serial.println(temperature);
}

String rtcDate(){
  String currentDate = String(day()) + " " + month() + " " + year(); 
  return currentDate;
}

String getTime(){
  String currentTime = String(hourFormat12()) + ":" + minute() + ":" + second();
  if (isAM()) {
    currentTime.concat(" AM   ");
  } else {
    currentTime.concat(" PM   ");  
  }
  //Serial.println(timeStatus());
  return currentTime;
}

BLYNK_CONNECTED() {
  rtc.begin();
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
  String out = TempMon.fileExplorer.command(incoming);
 
  //Custom commands
  if (out.equals("restart")) {
    terminal.println("Restarting... ");
    terminal.flush();
    ESP.restart();
  } else if(out.equals("macAddress")){
    terminal.println(WiFi.macAddress());
  } else if(out.equals("blynkAuth")){
    terminal.println("Blynk Auth Token: ");
    terminal.println(auth);
    terminal.println(strlen(auth));
  } else {
    if (out.equals(incoming)) {
      out.concat(": Command not recognized");
    }    
  }
  terminal.println(out);
  terminal.flush();
}
