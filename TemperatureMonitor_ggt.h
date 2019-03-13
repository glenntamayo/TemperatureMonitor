#ifndef TemperatureMonitor_ggt_h
#define TemperatureMonitor_ggt_h

#include <Arduino.h>
#include <DHTesp.h>
#include <LCD5110_ggt.h>
	
class TemperatureMonitor_ggt {
	private:
		DHTesp dht;
		LCD5110_ggt Lcd;
		//byte dhtPin;
		//String dhtModel;
		//float temperature;
		//float humidity;
		void dhtSetup(byte, String);
	public:
		TemperatureMonitor_ggt();
		void begin();
		void begin(byte, String);
		void begin(byte, String, byte, byte, byte, byte, byte, byte);
		float getTemperature();
		float getHumidity();
		
		void setContrast(byte);
		void setBrightness(int);
		void displayOut(char[30]);
	

};

#endif