/*
	Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "TemperatureMonitor_ggt.h"
#include <DHTesp.h>
#include <LCD5110_ggt.h>

TemperatureMonitor_ggt::TemperatureMonitor_ggt(){

}

void TemperatureMonitor_ggt::begin() {
		
}

void TemperatureMonitor_ggt::begin(byte _dhtPin, String _dhtModel){
	dhtSetup(_dhtPin, _dhtModel);
}

void TemperatureMonitor_ggt::begin(byte _dhtPin, String _dhtModel, byte _dcPin, byte _scePin, byte _rstPin, byte _blPin, byte _sdinPin, byte _sclkPin) {
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

void TemperatureMonitor_ggt::setContrast(byte _contrast) {
	Lcd.setContrast(_contrast);
}

void TemperatureMonitor_ggt::setBrightness(int _brightness) {
	Lcd.backlight(_brightness);
}

void TemperatureMonitor_ggt::displayOut(char _out[30]) {
	Lcd.setStr(_out, 0, 0, BLACK);
	Lcd.updateDisplay();
}