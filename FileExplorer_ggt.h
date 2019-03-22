#ifndef FileExplorer_ggt_h
#define FileExplorer_ggt_h

#include <Arduino.h>
#include <FS.h>

class FileExplorer {
	private:
		void commandDebug(String, byte*, String*);
	public:
		FileExplorer();
		void begin();
    //Directory directory;
		String getFileContents(String);
		String getParameter(String, String);
    String getParameter(String, String, int);
    void format();
    
    String listDirectory(String);

    String command(String);
    
};

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
      public:
        Argument();
        void init(String, Delimiter);
        byte getCount();
        String index[10];
    };

    Argument arg;
    String combineArgs(Argument);  //all arguments into a single argument
    String combineArgs(Arguments, char); // combine arguments separated by a specified char
    
    
};

#endif
