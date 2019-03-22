#ifndef TemperatureMonitor_ggt_h
#define TemperatureMonitor_ggt_h

#include <Arduino.h>
#include <DHTesp.h>
#include <LCD5110_ggt.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include "FileExplorer_ggt.h"

class TemperatureMonitor {
	private:
		DHTesp dht;
    
		LCD5110_ggt Lcd;
    byte dcPin, scePin, rstPin, blPin, sdinPin, sclkPin;
		
		int brightness;
		byte contrast;
		void dhtSetup(byte, String);

    String contentsLcd5110Settings = "LCD 5110 Settings \nbrightness:1023 \ncontrast:180 \nEnd LCD 5110 Settings";
    
	public:
		TemperatureMonitor();
		void begin();
		void begin(byte, String);
		void begin(byte, String, byte, byte, byte, byte, byte, byte);
		float getTemperature();
		float getHumidity();
		
		
		int setBrightness(int);
		int setBrightness(String);
    byte setContrast(byte);
    byte setContrast(String);
    void displayOut(char[30]);
    
    void getLcd5110Settings();
    void setSettings(String, String);
    
    FileExplorer fileExplorer;
    
};

#endif
