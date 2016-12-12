//
//  Options.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__Options__
#define __EchoErrorCorrection__Options__

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

#include "MessageLogger.hpp"
#include "GlobalSettings.hpp"
using namespace std;

enum State {ST_OK, ST_INP_ERROR, ST_HELP, ST_VERSION };

class Options {
    const char **argv;
    int argc;
    
    vector<string> inputFiles, outputFiles;
    string logFile;
    int numOfCPU, kmerLen;
    double mine, maxe;
    int minh, maxh;
    float h_rate;    //heterozygous rate
    int sampleSize; //for parameter selection
    int hashSize;
    
    State state;
    string errMessage;
    
public:
    Options(int argc, const char* argv[]);
    
    const vector<string>&  inputFileNames() const { return inputFiles; }
    const vector<string>&  outputFileNames() const { return outputFiles; }
    string  logFileName()    const   { return logFile; }
    int     numberOfCores()  const   { return numOfCPU; }
    int     kmerLenght()     const   { return kmerLen; }
    double  minErrToler()    const   { return mine; }
    double  maxErrToler()    const   { return maxe; }
    int     minOverlap()     const   { return minh; }
    int     maxOverlap()     const   { return maxh; }
    float   heterRate()      const   { return h_rate; }
    int     paramSampleSize()const   { return sampleSize; }
    int     hashTableSize()  const   { return hashSize; }
    
    State   getState()       const   { return state; }
    string  getErrMessage()  const   { return errMessage; }
    void    printHelp()      const;
    
    void logLaunchOptions(MessageLogger& logger);
private:
    void loadOptions();
    void noFlagLoading(int startIndex);
    bool testFor(int index, const char *opt1, const char *opt2 = NULL);
    void errInput(const char * message);
    void testFiles();
    void P(const char *options, const char *description = NULL) const;
    void P(const string options, const string description = "") const;
    int toInt(const char * str);
    double toDouble(const char * str);
    
    template<class T>
    void indent(const char *descr, T value, stringstream& msg);
    template<class T>
    void indent(const char *descr, const char *prefix, T value, stringstream& msg);
};

#endif /* defined(__EchoErrorCorrection__Options__) */
