#ifndef FileOperations_ggt_h
#define FileOperations_ggt_h

#include <Arduino.h>
#include <FS.h>

class FileOperations {
	private:
		void commandDebug(String, byte*, String*);
	public:
		FileOperations();
		void begin();
    String createFile(String);
    String createFile(String, String);
		String getFileContents(String);
		String getParameter(String, String);
    String getParameter(String, String, int);
    
    void format();
    
    String listDirectory(String);

    String command(String);
    
};



#endif
