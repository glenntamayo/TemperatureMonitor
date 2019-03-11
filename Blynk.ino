BLYNK_CONNECTED() {
 
}

void cbBrightness(void* context) {
  brightness = sliderBrightness.state;
  Blynk.virtualWrite(V0, sliderBrightness.state); //return to released state no slide
  Lcd.backlight(brightness);
  
  //Serial.println(brightness);
}

void cbContrast(void* context) {
  contrast = sliderContrast.state;
  Blynk.virtualWrite(V1, sliderContrast.state); //return to released state no slide
  Lcd.setContrast(contrast);
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



/*
BLYNK_CONNECTED() {
  updateSettingsTab();
  
}

byte getMenuIndex(char* item, const char menuArray[][20]) {
  byte i = 0;
  for (i; i < 10; i++) {
    if (strcmp(menuArray[i], item) == 0){
      break;
    }
  }
  return i;
}

void updateSettingsTab() {
  Blynk.setProperty(V21, "labels", range.menuItems[0], range.menuItems[1], range.menuItems[2]);
  Blynk.setProperty(V22, "labels", outputDataRate.menuItems[0], outputDataRate.menuItems[1], outputDataRate.menuItems[2], outputDataRate.menuItems[3], outputDataRate.menuItems[4], outputDataRate.menuItems[5], outputDataRate.menuItems[6], outputDataRate.menuItems[7]);  
  Blynk.setProperty(V23, "labels", writeInterval.menuItems[0], writeInterval.menuItems[1], writeInterval.menuItems[2], writeInterval.menuItems[3], writeInterval.menuItems[4]);
  Blynk.setProperty(V21, "color", BLYNK_GREEN);
  Blynk.setProperty(V22, "color", BLYNK_GREEN);
  Blynk.setProperty(V23, "color", BLYNK_GREEN);
  
  Blynk.setProperty(V24, "labels", menuReadInterval[0], menuReadInterval[1], menuReadInterval[2], menuReadInterval[3], menuReadInterval[4]);
  
  //Blynk.virtualWrite(V21, 1 + getMenuIndex(tiltSensor.getRange(), menuMmaRange));
  //Blynk.virtualWrite(V22, 1 + getMenuIndex(tiltSensor.getDataRate(), menuMmaOdr));
  Blynk.virtualWrite(V21, 1 + range.savedChoice);
  Blynk.virtualWrite(V22, 1 + outputDataRate.savedChoice);
  Blynk.virtualWrite(V23, 1 + writeInterval.savedChoice);


  
}

BLYNK_WRITE(V1) {
  String incoming = param.asStr();
  if (String("resetPulse") == incoming) {
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
  } else if  (String("eepromRead") == incoming.substring(0,10)) {
    int eepromAddress = incoming.substring(11).toInt();
    terminal.print("EEPROM data at address ");
    terminal.print(eepromAddress);
    terminal.println(" is:");
    if ((eepromAddress > -1) && (eepromAddress < 512)) {
      byte eepromValue = EEPROM.read(eepromAddress);
      terminal.println(eepromValue);
    }
  } else if (String("eepromWrite") == incoming.substring(0,11)) {
    int eepromAddress = incoming.substring(12, incoming.indexOf(',')).toInt();
    int eepromValue = incoming.substring(incoming.indexOf(',') + 1).toInt();
    //terminal.println(eepromAddress);
    //terminal.println(eepromValue);
    EEPROM.write(eepromAddress, eepromValue);
    EEPROM.commit();
    terminal.print("Successfully written ");
    terminal.print(eepromValue);
    terminal.print(" at address ");
    terminal.println(eepromAddress);
  } else if  (String("eepromClear") == incoming) {
    for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.end();
    terminal.println("EEPROM cleared");
  }
  #ifdef SDFILE
  else if (String("ls") == incoming) {
    String strDirectory = printDirectory();
    splitSendToBlynkTerminal(strDirectory, 1024);
    returnOK(strDirectory);
  } else if (String("readRTC") == incoming) {
    terminal.println("RTCTimeCheck: ");
    terminal.println(tiltSensor.rtcTimeCheck());
  } else if (String("readFile") == incoming.substring(0,8)) {
    terminal.println("Read File: "); //e.g. readFile 20180511.txt
    String strFilename = incoming.substring(9);
    terminal.println(readFile(strFilename));
    terminal.println("File Read done");
  }
  #endif
  
  terminal.flush();
}

BLYNK_WRITE(V10) {
  buttonTakeAngle.collect(param.asInt());
}

BLYNK_WRITE(V12) {
  buttonRefreshSettingsTab.collect(param.asInt());
}

BLYNK_WRITE(V14) {
  buttonChartOn.collect(param.asInt());
}

BLYNK_WRITE(V15) {
  sliderAlpha.collect(param.asInt());
}

BLYNK_WRITE(V17) {
  buttonRefreshAboutTable.collect(param.asInt());
}

BLYNK_WRITE(V18) {
  buttonSaveSettings.collect(param.asInt());
}

BLYNK_WRITE(V21) {
  if (range.unsavedChoice != param.asInt()) {
    range.unsavedChoice = param.asInt();
    range.changed = 1;
    Blynk.setProperty(V21, "color", BLYNK_YELLOW);
  }
}

BLYNK_WRITE(V22) {
  if (outputDataRate.unsavedChoice != param.asInt()) {
    outputDataRate.unsavedChoice = param.asInt();
    outputDataRate.changed = 1;
    Blynk.setProperty(V22, "color", BLYNK_YELLOW);
  }
}

BLYNK_WRITE(V23) {
  if (writeInterval.unsavedChoice != param.asInt()) {
    writeInterval.unsavedChoice = param.asInt();
    writeInterval.changed = 1;
    Blynk.setProperty(V23, "color", BLYNK_YELLOW);
  }
}

void chartTimerEvent() {
  Blynk.virtualWrite(V2, tiltSensor.xAngleFil);  
  Blynk.virtualWrite(V5, tiltSensor.xAngleFil);
  Blynk.virtualWrite(V3, tiltSensor.yAngleFil);  
  Blynk.virtualWrite(V6, tiltSensor.yAngleFil);  
  Blynk.virtualWrite(V4, tiltSensor.zAngleFil);  
  Blynk.virtualWrite(V7, tiltSensor.zAngleFil);  
}

#ifdef SDFILE
void RTCTimeCheckToBlynkTerminal() {
    terminal.println(tiltSensor.rtcTimeCheck());
    terminal.flush();
}
#endif

void cbTakeAngleToBlynkTerminal(void* context) {
    terminal.println(readingEntry);
    terminal.flush();
}

void cbRefreshSettingsTable(void* context) { //Used this order because column widths cannot be adjusted, or at least I can't
  updateSettingsTab();
}

void cbRefreshAboutTable(void* context) {
  aboutTable.clear();
  aboutTable.addRow(0, tiltSensor.getDeviceName(), "Device Name");
  aboutTable.addRow(1, WiFi.macAddress(), "Mac Address");
  aboutTable.addRow(2, tiltSensor.getAccelModel(), "Model");
  char charIP[16];
  sprintf(charIP, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
  aboutTable.addRow(3, charIP, "IP Address");
  aboutTable.addRow(4, charPulseVersion, "Pulse Version");
  aboutTable.addRow(5, tiltSensor.getRange(), "Acc. Range");
  aboutTable.addRow(6, tiltSensor.getDataRate(), "Acc. ODR");
}

void cbChartOn(void* context) {
  chartTimer.run();
  Blynk.virtualWrite(V15, alpha * 100.00);
}

void cbApplyAlpha(void* context) {
  alpha = sliderAlpha.state / 100.00;
  Blynk.virtualWrite(V15, sliderAlpha.state); //return to released state no slide
}

void splitSendToBlynkTerminal(String stringToSend, int chunkSize) {
  int numChunks = stringToSend.length() / chunkSize;
  for (int i = 0; i < numChunks; i++){
    String strSubstring = stringToSend.substring(i * chunkSize, i * chunkSize + (chunkSize));
    terminal.print(strSubstring);  
  }
  
  String strSubstring = stringToSend.substring(numChunks * chunkSize);
  terminal.print(strSubstring);  
}
*/
