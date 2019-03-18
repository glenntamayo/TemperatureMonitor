/*
	Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "TemperatureMonitor_ggt.h"
#include <DHTesp.h>
#include <LCD5110_ggt.h>
#include <ArduinoOTA.h>
#include <FS.h>

TemperatureMonitor_ggt::TemperatureMonitor_ggt(){
  spiffSetup();
  
  
  
}

void TemperatureMonitor_ggt::begin() {
  
  getLcd5110Settings();
  
  
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


/******************** WIFI TOOLS ********************/

/******************** WIFI TOOLS ********************/

/******************** LCD5110 TOOLS ********************/
void TemperatureMonitor_ggt::setSettings(String _filename, String _contents) {
  File f = SPIFFS.open(_filename, "w");  

  if (!f) {
    Serial.println("Settings file opening failed");  
  } else {
    f.print(_contents);
    f.close();  //Close file
  }
  delay(500);
}


void TemperatureMonitor_ggt::getLcd5110Settings() {
  // Uncomment only on new MCU
  //setSettings("/LCD5110 Settings", contentsLcd5110Settings);
  
  String displaySettings = getFileContents("/LCD5110 Settings");
  
  brightness = getParameter(displaySettings, "brightness", 0).toInt();
  contrast = getParameter(displaySettings, "contrast", 0).toInt();
  Serial.print("brightness: ");
  Serial.println(brightness);
  Serial.print("contrast: ");
  Serial.println(contrast);
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
/******************** LCD5110 TOOLS ********************/

/******************** SPIFF TOOLS ********************/
void TemperatureMonitor_ggt::spiffSetup() {
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");  
  } else {
    Serial.println("SPIFFS Initialization....failed");  
  }
}

void TemperatureMonitor_ggt::spiffFormat() {
  //Format File System
  if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}

String TemperatureMonitor_ggt::getFileContents(String _filename) {
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

String TemperatureMonitor_ggt::getParameter(String _file, String _parameter, int _index) {
  int startIndex = _file.indexOf(_parameter);
  
  int lastStartIndex = 0;
  for (int i = 0; i < _index + 1; i++) {
    startIndex = _file.indexOf(_parameter, lastStartIndex);
    lastStartIndex = startIndex + 1;
  }

  startIndex +=  _parameter.length();
  int endIndex = _file.indexOf('\n', startIndex);
  String value = _file.substring(startIndex + 1, endIndex - 1);
  return value;
}
/******************** SPIFF TOOLS ********************/
