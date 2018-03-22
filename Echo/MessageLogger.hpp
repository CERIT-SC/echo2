//
//  MessageLogger.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//

#ifndef __EchoErrorCorrection__MessageLogger__
#define __EchoErrorCorrection__MessageLogger__

#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

//EXCEPTIONS - LoggerException what() can return:
//cannot_create_file - constructor can throw this

class MessageLogger {
    fstream file;
    bool fileIsSet;
    
    MessageLogger(const MessageLogger&);
    void operator=(const MessageLogger&);
public:
    MessageLogger(string fileName);
    
    void log(string message, bool stdOutput = true, bool intoFile = true);
    void addNewLine(bool stdOutput = true, bool intoFile = true);
};

class LoggerException : public std::runtime_error {
public:
    LoggerException(string msg) : runtime_error(msg) {}
};

#endif /* defined(__EchoErrorCorrection__MessageLogger__) */
