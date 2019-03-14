/*
	Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "TemperatureMonitor_ggt.h"
#include <DHTesp.h>
#include <LCD5110_ggt.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <FS.h>

TemperatureMonitor_ggt::TemperatureMonitor_ggt(){
  IPAddress gateway(192, 168, 1, 1);        // gateway of your network
  IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network
  WiFi.mode(WIFI_AP_STA);
  wifiMulti.addAP("ggdt_android", "Winterfell1");
  wifiMulti.addAP("TAMAYO2.4", "lekleklek");

  spiffSetup();
}

void TemperatureMonitor_ggt::begin() {
  displaySettings = fileContents(filenameLcd5110Settings);
  brightness = getParameter(displaySettings, "brightness");
  contrast = getParameter(displaySettings, "contrast");
  Serial.print("begin: brightness: ");
  Serial.println(brightness);
  Serial.print("begin: contrast: ");
  Serial.println(contrast);
}

String TemperatureMonitor_ggt::fileContents(String _filename) {
  File f = SPIFFS.open(_filename, "r");  
  String contents = "";

  if(f){
    for(int i = 0; i < f.size(); i++) {
        char mychar = f.read();
        contents.concat(mychar);
      }
      f.close();
      return contents;
  }
  delay(500);
}



void TemperatureMonitor_ggt::begin(byte _dhtPin, String _dhtModel){
	begin();
	dhtSetup(_dhtPin, _dhtModel);
}

void TemperatureMonitor_ggt::begin(byte _dhtPin, String _dhtModel, byte _dcPin, byte _scePin, byte _rstPin, byte _blPin, byte _sdinPin, byte _sclkPin) {
  blPin = _blPin;
  dcPin = _dcPin;
  scePin = _scePin;
  rstPin = _rstPin;
  sdinPin = _sdinPin;
  sclkPin = _sclkPin;
	begin(_dhtPin, _dhtModel);
	Lcd.begin(_dcPin, _scePin, _rstPin, _blPin, _sdinPin, _sclkPin);
	Lcd.clearDisplay(WHITE);
	Lcd.updateDisplay();
}

float TemperatureMonitor_ggt::getTemperature() {
	return (float)dht.getTemperature();	
}

float TemperatureMonitor_ggt::getHumidity() {
	return (float)dht.getHumidity();	
}


void TemperatureMonitor_ggt::dhtSetup(byte _dhtPin, String _model) {
	if(_model.equals("DHT11")) {
		dht.setup(_dhtPin, DHTesp::DHT11);
	}
}

byte TemperatureMonitor_ggt::setContrast(byte _contrast) {
  Lcd.setContrast(_contrast);
  return contrast;
}

int TemperatureMonitor_ggt::setBrightness(int _brightness) {
	Lcd.backlight(_brightness);
  return _brightness;
}

int TemperatureMonitor_ggt::setBrightness(String _source) {
  if (_source.equals("Settings")) {
    Lcd.backlight(brightness);
  }
  return brightness;
}

byte TemperatureMonitor_ggt::setContrast(String _source) {
  if (_source.equals("Settings")) {
    Lcd.setContrast(contrast);
  }
  return contrast;
}

void TemperatureMonitor_ggt::displayOut(char _out[30]) {
	Lcd.setStr(_out, 0, 0, BLACK);
	Lcd.updateDisplay();
}

bool TemperatureMonitor_ggt::isWifiConnected() {
  bool myBool;
  if (wifiMulti.run() == WL_CONNECTED) {
    myBool = true;
  }  else {
    myBool = false;  
  }
  return myBool;
}

void TemperatureMonitor_ggt::spiffSetup() {
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");  
  } else {
    Serial.println("SPIFFS Initialization....failed");  
  }
}

void TemperatureMonitor_ggt::setLcd5110Settings() {
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




void TemperatureMonitor_ggt::getLcd5110Settings() {
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

int TemperatureMonitor_ggt::getParameter(String _file, String _parameter) {
  int startIndex = _file.indexOf(_parameter) + _parameter.length();
  int endIndex = _file.indexOf('\n', startIndex);
  String value = _file.substring(startIndex + 1, endIndex - 1);
  return value.toInt();
}


void TemperatureMonitor_ggt::spiffFormat() {
  //Format File System
  if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}
