//
//  MessageLogger.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//

#include "MessageLogger.hpp"

#define SWF setw(2) << setfill('0')

MessageLogger::MessageLogger(string fileName) {
    
    if (fileName.size()) {
        file.open(fileName, ios::out);
        
        if (!file.good()) {
            log("Cannot create log file.", true, false);
            throw LoggerException("cannot_create_file");
        }
        
        fileIsSet = true;
    } else {
        fileIsSet = false;
    }
}


void MessageLogger::log(string message, bool stdOutput, bool intoFile) {
    stringstream msg;
    
    //get time
    time_t timeInS = time(NULL);
    tm * timeinfo = localtime(&timeInS);
    
    //create time stamp
    msg << timeinfo->tm_year+1900 << '_' << SWF << timeinfo->tm_mon << '_' << SWF << timeinfo->tm_mday;
    msg << " - ";
    msg << SWF << timeinfo->tm_hour << ':' << SWF << timeinfo->tm_min << ':' << SWF << timeinfo->tm_sec;
    msg << ": ";
    
    //then message
    msg << message;
    
    //output
    if (stdOutput)             cout << msg.str() << endl;
    if (intoFile && fileIsSet) file << msg.str() << endl;
}

void MessageLogger::addNewLine(bool stdOutput, bool intoFile) {
    if (stdOutput) cout << endl;
    if (intoFile)  file << endl;
}
