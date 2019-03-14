#ifndef TemperatureMonitor_ggt_h
#define TemperatureMonitor_ggt_h

#include <Arduino.h>
#include <DHTesp.h>
#include <LCD5110_ggt.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <FS.h>

class TemperatureMonitor_ggt {
	private:
		DHTesp dht;
		LCD5110_ggt Lcd;
    byte dcPin, scePin, rstPin, blPin, sdinPin, sclkPin;
		IPAddress gateway(byte, byte, byte, byte);        // gateway of your network
		IPAddress subnet(byte, byte, byte, byte);       // subnet mask of your network
		ESP8266WiFiMulti wifiMulti;
		
		int brightness;
		byte contrast;
		void dhtSetup(byte, String);
    const char* filenameLcd5110Settings = "/LCD5110 Settings";
    int getParameter(String, String);
    String displaySettings;
    String fileContents(String);
	public:
		TemperatureMonitor_ggt();
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
    bool isWifiConnected();
    void spiffSetup();
    void getLcd5110Settings();
    void setLcd5110Settings();
    void spiffFormat();
    
};

#endif
