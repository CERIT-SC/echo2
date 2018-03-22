//
//  Preprocessing.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//

#ifndef EchoErrorCorrection_Preprocessing_h
#define EchoErrorCorrection_Preprocessing_h

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "MessageLogger.hpp"
#include "Sequence.hpp"
#include "RandomisedAccess.hpp"
#include "SequenceFileLoader.hpp"
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
        
        logger.log(string("Number of sequences: ") + separateThousands(to_string(seqArray.size())));
        
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
    
    string separateThousands(string input) {
        reverse(input.begin(), input.end());
        
        istringstream i(input);
        string result;
        
        int count = 0;
        while(i.peek() != EOF) {
            count++;
            if (count % 4 == 0) {
                result.push_back(',');
                continue;
            }
            result.push_back(static_cast<char>(i.get()));
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
};

#endif
