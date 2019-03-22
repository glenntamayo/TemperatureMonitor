/*
	Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "TemperatureMonitor_ggt.h"
#include "FileExplorer_ggt.h"
#include <DHTesp.h>
#include <LCD5110_ggt.h>
#include <ArduinoOTA.h>
#include <FS.h>

TemperatureMonitor::TemperatureMonitor(){
  
}

void TemperatureMonitor::begin() {
  fileExplorer.begin();
  getLcd5110Settings();
}


void TemperatureMonitor::begin(byte _dhtPin, String _dhtModel){
	begin();
	dhtSetup(_dhtPin, _dhtModel);
}

void TemperatureMonitor::begin(byte _dhtPin, String _dhtModel, byte _dcPin, byte _scePin, byte _rstPin, byte _blPin, byte _sdinPin, byte _sclkPin) {
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

float TemperatureMonitor::getTemperature() {
	return (float)dht.getTemperature();	
}

float TemperatureMonitor::getHumidity() {
	return (float)dht.getHumidity();	
}


void TemperatureMonitor::dhtSetup(byte _dhtPin, String _model) {
	if(_model.equals("DHT11")) {
		dht.setup(_dhtPin, DHTesp::DHT11);
	}
}


/******************** WIFI TOOLS ********************/

/******************** WIFI TOOLS ********************/

/******************** LCD5110 TOOLS ********************/
void TemperatureMonitor::setSettings(String _filename, String _contents) {
  File f = SPIFFS.open(_filename, "r");  

  Serial.println("");
  Serial.print("File: ");  
  Serial.print(_filename);  
  Serial.print("opening ");
  if (!f) {
    Serial.println("failed");  
  } else {
      
    Serial.println("success");  
    f.print(_contents);
    f.close();  //Close file
  }
  delay(500);
}


void TemperatureMonitor::getLcd5110Settings() {
  // Uncomment only on new MCU
  //setSettings("/LCD5110 Settings", contentsLcd5110Settings);
  
  String displaySettings = fileExplorer.getFileContents("/LCD5110 Settings");
  
  brightness = fileExplorer.getParameter(displaySettings, "brightness", 0).toInt();
  contrast = fileExplorer.getParameter(displaySettings, "contrast", 0).toInt();
  Serial.print("brightness: ");
  Serial.println(brightness);
  Serial.print("contrast: ");
  Serial.println(contrast);
}

byte TemperatureMonitor::setContrast(byte _contrast) {
  Lcd.setContrast(_contrast);
  return contrast;
}

int TemperatureMonitor::setBrightness(int _brightness) {
	Lcd.backlight(_brightness);
  return _brightness;
}

int TemperatureMonitor::setBrightness(String _source) {
  if (_source.equals("Settings")) {
    Lcd.backlight(brightness);
  }
  return brightness;
}

byte TemperatureMonitor::setContrast(String _source) {
  if (_source.equals("Settings")) {
    Lcd.setContrast(contrast);
  }
  return contrast;
}

void TemperatureMonitor::displayOut(char _out[60]) {
	//Lcd.clearDisplay(0);
	Lcd.setStr(_out, 0, 0, BLACK);
	Lcd.updateDisplay();
}
/******************** LCD5110 TOOLS ********************/
