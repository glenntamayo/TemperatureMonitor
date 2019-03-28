/*
	Created by Glenn Gil Tamayo, 
*/
#define DEBUG 1

#include <Arduino.h>
#include "FileOperations_ggt.h"
#include "CommandLine_ggt.h"
#include <ArduinoOTA.h>
#include <FS.h>

FileOperations::FileOperations(){
  
}

void FileOperations::begin() {
  Serial.println("");
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");  
  } else {
    Serial.println("SPIFFS Initialization....failed");  
  }
}

void FileOperations::format() {
  //Format File System
  if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}

String FileOperations::createFile(String _filename) {
  String out;
  File f = SPIFFS.open(_filename, "w");  

  if (!f) {
    //Serial.println("File creation failed"); 
    out.concat("File creation failed \n"); 
    return out; 
  } else {
    f.close();
    //Serial.println("File creation success");  
    out.concat("File creation success \n");  
    
  }
  delay(500);

  return out;
}

String FileOperations::createFile(String _filename, String _contents) {
  String out;

  out.concat(createFile(_filename));
  
  File f = SPIFFS.open(_filename, "w");  

  if (!f) {
    out.concat("File opening failed"); 
    return out; 
  } else {
    f.print(_contents);
    f.close();
    out.concat("File writing success");  
    
  }
  delay(500);

  return out;
}

String FileOperations::getFileContents(String _filename) {
  File f = SPIFFS.open(_filename, "r");  
  String contents = "";

  if(!f) {
    Serial.println("File opening failed");
    return ("File not found");
  } else{
    for(int i = 0; i < f.size(); i++) {
        char mychar = f.read();
        contents.concat(mychar);
      }
      f.close();
      return contents;
  }
  delay(500);
}

String FileOperations::getParameter(String _file, String _parameter, int _index) {
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

String FileOperations::listDirectory(String _directory) {
  String out = "";
  Dir dir = SPIFFS.openDir(_directory);

  int countFiles = 0;
  out.concat("Files in current directory: \n");
  while (dir.next()) {
    countFiles++;
    out.concat(dir.fileName());
    out.concat('\t');
    File f = dir.openFile("r");
    out.concat(f.size());
    out.concat("b\n");
    f.close();  
  }
  if (countFiles == 0) {
    out.concat("Directory doesn't exists or is empty");  
  }  
  return out;
}

String FileOperations::command(String _incoming) {
  String out = "";
  
  CommandLine cl(_incoming, ' ');
  String strCommand = cl.getCommand();

  byte argCount = cl.arg.getCount();


  if(strCommand.equals("dir")) {
    String incomingDir = "/";
    if (argCount == 0) { 
      out.concat(listDirectory(incomingDir));
    } else {
      incomingDir.concat(cl.combineArgs(cl.arg));//pass the Argument rather _incoming
      Serial.print("incomingDir: "); Serial.print(incomingDir); Serial.println("|");
      out.concat(getFileContents(incomingDir));
    }
  } else if(strCommand.equals("rename")) {
    String fromFilename = "/";
    String toFilename = "/";

    CommandLine::Delimiter kama(_incoming, ',');
    if (kama.getCount() == 0) {
      out.concat("Syntax error: rename <from filename>, <to filename>");
      return out;  
    }

    String* renameFilenames = cl.combineArgs(_incoming, kama);
    fromFilename.concat(renameFilenames[0]);
    toFilename.concat(renameFilenames[1]);

    Serial.print("fromFilename: "); Serial.println(fromFilename);
    Serial.print("toFilename: "); Serial.println(toFilename);

    if(SPIFFS.rename(fromFilename, toFilename)) {
      out.concat("File renamed succussfully");  
    } else {
      out.concat("File renamed failed");  
    }
  } else if(strCommand.equals("write")) {
    String filename = "/";
    String contents = "";

    if (argCount == 0) { 
      out.concat("Syntax error: Specify path and filename");
      return out;
    } else {
      CommandLine::Delimiter kama(_incoming, ',');
      Serial.println(kama.getCount());
      if (kama.getCount() == 1) {
        filename.concat(cl.combineArgs(cl.arg));
        out.concat(createFile(filename));
      } else {
        String* tempArg = cl.combineArgs(_incoming, kama);
        filename.concat(tempArg[0]);
        contents.concat(tempArg[1]);
        out.concat(createFile(filename, contents));
      }
    }
  }
  //--------------------------//
  else{
    out.concat(_incoming);
  }

  #ifdef DEBUG
    //commandDebug(_command, delimiter, argument);
  #endif
  
  return out;
}
/*
void FileOperations::commandDebug(String _command, byte* delimiter, String* argument) {
  Serial.println(_command);
  
  Serial.print("Command::delimiter[0]: "); Serial.println(delimiter[0]);
  
  for (int i = 0; i < delimiter[0]; i++) {
    Serial.print("command::Delimiter position ["); Serial.print(i + 1); Serial.print("]: "); Serial.println(delimiter[1 + i]);
  }

  for (int i = 0; i < delimiter[0] + 1; i++) {
    Serial.print("command::argument["); Serial.print(i); Serial.print("]: ");Serial.println(argument[i]);
  }
  
}
*/
