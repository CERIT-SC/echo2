//
//  main.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 08/09/13.
//

#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <iomanip>

#include "Sequence.hpp"
#include "RandomisedAccess.hpp"
#include "Options.hpp"
#include "MessageLogger.hpp"
#include "HashKmerOccTable.hpp"
#include "Params.hpp"
#include "GlobalSettings.hpp"
#include "Tools.hpp"
#include "FastqSave.hpp"

//phases
#include "Preprocessing.hpp"
#include "Hashing.hpp"
#include "ParameterSelection.hpp"
#include "ErrorCorrection.hpp"

using namespace std;

//function declarations
unsigned processFiles(Options& options, MessageLogger& logger);
string runTime(time_t start);
bool runtimeCompatibilityTests();

string getOutputFileName(const Options& options, ULL fileIndex);
bool openOutputFile(MessageLogger& logger, FastqSave& fastqFile, string fileName);
bool saveToFile(MessageLogger& logger, FastqSave& fastqFile, RandomisedAccess& access,
                vector<string>& sequences, vector<string>& quality);


int main(int argc, const char * argv[])
{
    if (!runtimeCompatibilityTests()) return RETURN_COMPATIBILITY_ERROR;
    
    //launch
    Options options(argc, argv);
    switch (options.getState()) {
        case ST_INP_ERROR:
            cout << options.getErrMessage() << endl;
            return RETURN_INC_OPT;
        case ST_HELP:
            options.printHelp();
            return RETURN_SUCCESS;
        case ST_VERSION:
            cout << APPLICATION_NAME_STRING << " version " << APPLICATION_VERSION_STRING << endl;
            return RETURN_SUCCESS;
        case ST_OK: break;
    }
    
    //logging
    shared_ptr<MessageLogger> initLogger;
    try {
        initLogger = shared_ptr<MessageLogger>(new MessageLogger(options.logFileName()));
    } catch (LoggerException& e) {
        return RETURN_LOG_FILE_ERROR;
    }
    MessageLogger& logger = *initLogger;
    options.logLaunchOptions(logger);
    
    
    time_t programStart = time(nullptr);
    const vector<string>& inputFiles = options.inputFileNames();
    
    //correction of all files
    unsigned succFileCount;
    try {
        succFileCount = processFiles(options, logger);
        
        if (succFileCount < inputFiles.size()) {
            if (inputFiles.size() > 1) {
                logger.log("Some files weren't processed.");
                logger.log(string("Total run time (d:h:m:s): ") + runTime(programStart));
            }
            else {
                logger.log("File weren't processed.");
            }
            
            logger.addNewLine();
            return RETURN_FILES_SKIPPED;
        }
        
        if (inputFiles.size() > 1) {
            logger.log("Finished processing all files.");
            logger.log(string("Total run time (d:h:m:s): ") + runTime(programStart));
            logger.addNewLine();
        }
        
    } catch (bad_alloc& e) {
        logger.log("Error while allocating memory. Terminating.");
        return RETURN_MEMORY_ERROR;
        
    } catch (exception& e) {    //something, that should never happen
        logger.log(string("Unexpected error: ") + e.what() + " Terminating.");
        return RETURN_UNKNOWN_ERROR;
    } catch (...) {
        logger.log(string("Unexpected error. Terminating."));
        return RETURN_UNKNOWN_ERROR;
    }
    
    return 0;
}

unsigned processFiles(Options& options, MessageLogger& logger) {
    unsigned succFileCounter = 0;
    
    //run for every input file
    const vector<string>& inputFiles = options.inputFileNames();
    for (auto inputFile = inputFiles.begin(); inputFile != inputFiles.end(); inputFile++) {
        
        //start
        if (inputFiles.size() == 1) logger.log("Starting");
        else logger.log(string("Starting file: " + *inputFile));
        time_t startFile = time(NULL);
        
        //prepare output file
        FastqSave fastqFile;
        string outputFileName = getOutputFileName(options, inputFile - inputFiles.begin());
        if(!openOutputFile(logger, fastqFile, outputFileName)) continue;
        
        //preprocessing
        Preprocessing preprocessing(logger);
        
        vector<Sequence> seqArray;
        RandAccessPtr randAccessPtr;
        
        if(!preprocessing(*inputFile, seqArray, randAccessPtr)) continue;
        
        RandomisedAccess& seqRandAccess = *randAccessPtr;
        Params params = setParams(options, seqArray);
        logger.log("K-mer length: " + to_string(params.kmerLength));
        
        //hashing
        Hashing hashing(logger, seqRandAccess, params);
        HashPtr hashTable = hashing(options.numberOfCores());
        
        //parameter selection
        ParameterSelection paramSelect(logger);
        paramSelect(params, hashTable, seqRandAccess, options.numberOfCores());
        
        //error correction
        vector<string> repairedSeq, quality;
        
        ErrorCorrection errCorrection(logger);
        errCorrection(params, hashTable, seqRandAccess, repairedSeq, quality, options.numberOfCores());
        
        //saving result into file
        if (!saveToFile(logger, fastqFile, seqRandAccess, repairedSeq, quality)) continue;
        
        //finish
        if (inputFiles.size() == 1) logger.log("Finished.");
        else logger.log(string("Finished file: " + *inputFile));
        
        logger.log(string("Run time (d:h:m:s): ") + runTime(startFile));
        logger.addNewLine();
        
        succFileCounter++;
    }
    
    return succFileCounter;
}

string runTime(time_t start) {
    stringstream result;
    time_t seconds = time(NULL) - start;
    time_t days = seconds/(3600*24);
    seconds = seconds - days*(3600*24);
    time_t hours = seconds/3600;
    seconds = seconds - hours*3600;
    time_t minutes = seconds/60;
    seconds = seconds - minutes*60;
    
    result.fill('0');
    result << days << ":" << setw(2) << hours << ":";
    result << setw(2) << minutes << ":" << setw(2) << seconds;
    
    return result.str();
}

bool runtimeCompatibilityTests() {
    if (sizeof(int) != 4) {
        cout << "Compatibility error: int variable must be of size 4 B." << endl;
        return false;
    }
    
    if (sizeof(unsigned long long) != 8) {
        cout << "Compatibility error: long variable must be of size 8 B." << endl;
        return false;
    }
    
    return true;
}

string getOutputFileName(const Options& options, ULL fileIndex) {
    const vector<string>& outputFiles = options.outputFileNames();
    const vector<string>& inputFiles = options.inputFileNames();
    
    if (fileIndex >= outputFiles.size()) {
        return inputFiles[fileIndex] + OUTPUT_FILE_NAME_EXTENSION;
    }
    
    return outputFiles[fileIndex];
}

bool openOutputFile(MessageLogger& logger, FastqSave& fastqFile, string fileName) {
    
    try {
        fastqFile.open(fileName);
        
    } catch (FastqSaveException& e) {
        if (string("cannot_open_file") == e.what()) {
            logger.log(string("Cannot open output file: ") + fileName + " Skipping.");
            logger.addNewLine();
            
            return false;
            
        } else throw;
    }
    
    logger.log("Output file: " + fileName + " opened.");
    return true;
}

bool saveToFile(MessageLogger& logger, FastqSave& fastqFile, RandomisedAccess& access,
                vector<string>& sequences, vector<string>& quality) {
    
    //collecting result
    vector<ULL> order = getOriginalOrder(access);
    
    logger.log("Saving into file.");
    
    try {
        fastqFile(sequences, quality, order);
        
    } catch (FastqSaveException& e) {
        if (string("cannot_save_to_file") == e.what()) {
            logger.log(string("Error while saving to file: ") + fastqFile.getFileName() + " Skipping.");
            logger.addNewLine();
            
            return false;
            
        } else throw;
    }
    
    return true;
}
