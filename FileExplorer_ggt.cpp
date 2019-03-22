/*
	Created by Glenn Gil Tamayo, 
*/
#define DEBUG 1

#include <Arduino.h>
#include "FileExplorer_ggt.h"
#include <ArduinoOTA.h>
#include <FS.h>

FileExplorer::FileExplorer(){
  
}

void FileExplorer::begin() {
  Serial.println("");
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");  
  } else {
    Serial.println("SPIFFS Initialization....failed");  
  }

  //mapDirectory();
}

void FileExplorer::format() {
  //Format File System
  if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}

String FileExplorer::getFileContents(String _filename) {
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

String FileExplorer::getParameter(String _file, String _parameter, int _index) {
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
/*
void FileExplorer_ggt::mapDirectory(){
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.println(f.size());
  }
}
*/

String FileExplorer::listDirectory(String _directory) {
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

String FileExplorer::command(String _incoming) {
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

    cl.combineArgs(cl.arg, kama);
    //fromFilename = cl.combineArgs(strCommand, kama);
    //toFilename = cl.combineArgs(str);
    /*U

    
    for (int i = lastIndexFromFilename + 1; i < countArg; i++) {
      toFilename.concat(argument[i]);
      toFilename.concat(" ");
    }
    toFilename.concat(argument[countArg]);

    //Serial.print("delimiterComma: "); Serial.println(delimiterComma);
    //Serial.print("fromFilename: "); Serial.println(fromFilename);
    //Serial.print("toFilename: "); Serial.println(toFilename);

    
    if(SPIFFS.rename(fromFilename, toFilename)) {
      out.concat("File renamed succussfully");  
    } else {
      out.concat("File renamed failed");  
    }
    
    */
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
void FileExplorer::commandDebug(String _command, byte* delimiter, String* argument) {
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

//**********************COMMANDLINE**********************//
CommandLine::CommandLine() {
  
}

CommandLine::CommandLine(String _incoming, char _delimiterType) {
  Serial.println(_incoming);
  Delimiter delimiter(_incoming, _delimiterType);
  arg.init(_incoming, delimiter);
  
  if (delimiter.getCount() > 0) {
    command = _incoming.substring(0, delimiter.index[0]);
  } else {
    command = _incoming.substring(0);
  }
  
}

String CommandLine::getCommand() {
  return command;  
}

String CommandLine::combineArgs(Argument _args) {
  String out = "";
  
  for (int i = 0; i < _args.getCount() - 1; i++) {
    out.concat(_args.index[i]);
    out.concat(" ");
  }
  out.concat(_args.index[_args.getCount() - 1]);

  return out;
}

String CommandLine::combineArgs(Argument _args, char _delimiter) {

  combineArgs()
}
//**********************COMMANDLINE**********************//

//**********************DELIMITER**********************//
CommandLine::Delimiter::Delimiter() {
}

CommandLine::Delimiter::Delimiter(String _incoming, char _type) {
  for (int i = 0; i < 10; i++) {
    index[i] = 255;  
  }

  //count the number of delimiter types in the command string
  char charCommand[_incoming.length()];
  count = 0;
  //Serial.print("command.length(): "); Serial.println(command.length());
  for (int i = 0; i < _incoming.length(); i++) {
      charCommand[i] = _incoming.charAt(i);
      if(charCommand[i] == _type) {
        index[count] = i;  
        count++;
      }
  }
  //Serial.print("Delimiter::count(): "); Serial.println(count);
}

byte CommandLine::Delimiter::getCount() {
  //Serial.print("Delimiter::getCount(): "); Serial.println(count);
  return count;  
}
//**********************DELIMITER**********************//

//**********************ARGUMENTS**********************//

CommandLine::Argument::Argument(){
}

void CommandLine:: Argument::init(String _incoming, Delimiter delimiter) {
  for(int i = 0; i < 10; i++) {
    index[i] = "";  
  }
  count = 0;
  for (int i = 0; i < delimiter.getCount(); i++) {
    byte startIndex = delimiter.index[i] + 1;
    //Serial.print("StartIndex: "); Serial.print(startIndex); Serial.print('\t');
    byte endIndex =   delimiter.index[i + 1];
    //Serial.print("EndIndex: "); Serial.println(endIndex);

    if (endIndex < startIndex) {
      index[i] = _incoming.substring(startIndex);
    } else {
      index[i] = _incoming.substring(startIndex, endIndex);
    //Serial.print("getArgument::argument["); Serial.print(i); Serial.print("]: ");Serial.println(argument[i]);
    }
    count++;
  }
  
  Serial.print("Argument::count: "); ;Serial.println(count);
  for (int i = 0; i < count; i++) {
    Serial.print("Argument::index["); Serial.print(i); Serial.print("]: ");Serial.print(index[i]); Serial.println("|");
  } 
  
}

byte CommandLine::Argument::getCount() {
  //Serial.print("Argument::getCount: "); ;Serial.println(count);
  return count;  
}

//**********************ARGUMENTS**********************//
