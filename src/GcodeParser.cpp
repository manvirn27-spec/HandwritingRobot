#include "GcodeParser.h"

/*GcodeParser::Command GcodeParser::parseLine(const String &line){

    char cmdChar = 'a';
    int cmdNum = -1;
    if(line.length() == -1)
        return GcodeParser::Command::invalid;
    String cleanLine = line.substring(0, line.indexOf(";"));

    for(int i = 0; i < cleanLine.length(); i++){
        char c = toupper(cleanLine[i]);
        if(c == 'G')
            switch (cmdNum) {
                case 0:
                case 1:
                case 28:
                default: break;
            }
    }
}
    */