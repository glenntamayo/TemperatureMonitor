/*
	Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "TemperatureMonitor_ggt.h"
#include "FileOperations_ggt.h"
#include <DHTesp.h>
#include "LCD5110_ggt2.h"
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
  Lcd.init(sclkPin, sdinPin, dcPin, rstPin, scePin, blPin);
	//Lcd.begin(_dcPin, _scePin, _rstPin, _blPin, _sdinPin, _sclkPin);
	//Lcd.clearDisplay(WHITE);
	//Lcd.updateDisplay();
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
  fileExplorer.createFile(_filename, _contents);
  
  
}


void TemperatureMonitor::getLcd5110Settings() {
  // Uncomment only on new MCU
  setSettings("/LCD5110 Settings.txt", contentsLcd5110Settings);
  
  String displaySettings = fileExplorer.getFileContents("/LCD5110 Settings.txt");
  
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
	Lcd.setBacklight(_brightness);
  return _brightness;
}

int TemperatureMonitor::setBrightness(String _source) {
  if (_source.equals("Settings")) {
    Lcd.setBacklight(brightness);
  }
  return brightness;
}

byte TemperatureMonitor::setContrast(String _source) {
  if (_source.equals("Settings")) {
    Lcd.setContrast(contrast);
  }
  return contrast;
}

void TemperatureMonitor::displayOut(String _out) {
	Lcd.printStr(_out);
}

void TemperatureMonitor::displayOut(String _out, byte _cursorY) {
  Lcd.clearLine(_cursorY);
  Lcd.setXYChar(0, _cursorY);
  Lcd.sendStr(_out);
  
}
/******************** LCD5110 TOOLS ********************/
