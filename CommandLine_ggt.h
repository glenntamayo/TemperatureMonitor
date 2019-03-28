#ifndef CommandLine_ggt_h
#define CommandLine_ggt_h

#include <Arduino.h>

class CommandLine {
  private:
    String command;
  public:
    CommandLine();
    CommandLine(String, char);
    String getCommand();
    //byte getCount();
    //String args[10];
    
    class Delimiter {
      private:
        byte count;  
      public:
        Delimiter();
        Delimiter(String, char);
        byte getCount();
        byte index[10];
    };
    class Argument {
      private:
        byte count;
        String raw;
      public:
        Argument();
        void init(String, Delimiter);
        byte getCount();
        String index[10];
        String getRaw();
    };

    Argument arg;
    String combineArgs(Argument);  //all arguments into a single argument
    String* combineArgs(String, Delimiter); // combine arguments separated by a specified char
    
    
};


#endif
