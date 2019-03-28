/*
  Created by Glenn Gil Tamayo, 
*/

#include <Arduino.h>
#include "CommandLine_ggt.h"

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

String* CommandLine::combineArgs(String _incoming, Delimiter _delimiter) {
  /*
  for (int i = 0; i < _delimiter.getCount(); i++) {
    Serial.println(_delimiter.index[i]);  
  }
  */
  Argument combineArg;
  combineArg.init(_incoming, _delimiter);

  static String out[2];

  out[0] = combineArg.index[0];
  out[1] = combineArg.index[1];
  //Serial.print("combineArgs: <from>"); Serial.println(combineArg.index[0]);
  //Serial.print("combineArgs: <to>"); Serial.println(combineArg.index[1]);

  return out;
}
//**********************COMMANDLINE**********************//

//**********************DELIMITER**********************//
CommandLine::Delimiter::Delimiter() {
}

CommandLine::Delimiter::Delimiter(String _incoming, char _type) {
  const char _commandDelimiterType = ' '; //default delimiter after command
  
  for (int i = 0; i < 10; i++) {
    index[i] = 255;  
  }

  //count the number of delimiter types in the command string
  char charCommand[_incoming.length()];
  count = 0;
  //Serial.print("command.length(): "); Serial.println(command.length());
  for (int i = 0; i < _incoming.length(); i++) {
      charCommand[i] = _incoming.charAt(i);

      if (count == 0) {
        if(charCommand[i] == _commandDelimiterType) {
          index[0] = i;  
          count++;
        }
      } else {
        if(charCommand[i] == _type) {
          index[count] = i;  
          count++;
        }
      }
      
      if (count == 10) {
        break; 
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
  raw = _incoming.substring(delimiter.index[0] + 1);;
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

  for (int i = 0; i < count; i++){
    index[i].trim();  
  }

  /*
  Serial.print("Argument::count: "); ;Serial.println(count);
  for (int i = 0; i < count; i++) {
    Serial.print("Argument::index["); Serial.print(i); Serial.print("]: ");Serial.print(index[i]); Serial.println("|");
  } 
  */
}

byte CommandLine::Argument::getCount() {
  //Serial.print("Argument::getCount: "); ;Serial.println(count);
  return count;  
}

String CommandLine::Argument::getRaw() {
  return raw;  
}

//**********************ARGUMENTS**********************//
