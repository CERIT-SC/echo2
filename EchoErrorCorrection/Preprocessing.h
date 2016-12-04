//
//  Preprocessing.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef EchoErrorCorrection_Preprocessing_h
#define EchoErrorCorrection_Preprocessing_h

#include <string>
#include <vector>
#include <sstream>

#include "MessageLogger.h"
#include "Sequence.h"
#include "RandomisedAccess.h"
#include "SequenceFileLoader.h"
using namespace std;

typedef shared_ptr<RandomisedAccess> RandAccessPtr;


class Preprocessing {
    MessageLogger & logger;
    string inputFileName;
    
public:
    Preprocessing(MessageLogger& logger) : logger(logger) {}
    
    bool operator()(string inputFileName, vector<Sequence>& seqArray, RandAccessPtr& randAccess) {
        this->inputFileName = inputFileName;
        logger.log("Preprocessing phase (1/4)");
        
        if(!loadInputFile(seqArray)) return false;
        
        logger.log(string("Number of sequences: ") + to_string(seqArray.size()));
        
        seqArray.shrink_to_fit();
        
        //creating random access
        randAccess = RandAccessPtr(new RandomisedAccess(seqArray, true));
        
        logger.log("Preprocessing complete.");
        
        return true;
    }
    
private:
    bool loadInputFile(vector<Sequence>& seqArray) {
        SequenceFileLoader loader;
        logger.log("Loading input file.");
        
        try {
            loader.load(inputFileName, seqArray);
            
        } catch (LoaderException& e) {
            if (string("cannot_open_file") == e.what()) {
                logger.log(string("Cannot open file: ") + inputFileName + string(" Skipping."));
            } else {
                logger.log("Inconsistency while reading input file. Skipping file.");
            }
            
            logger.addNewLine();
            return false;
        }
        
        logger.log("Input file loaded.");
        return true;
    }
};

#endif
