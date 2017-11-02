//
//  Hashing.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef EchoErrorCorrection_Hashing_h
#define EchoErrorCorrection_Hashing_h

#include <cmath>
#include <limits>
#include <algorithm>

#include "MessageLogger.hpp"
#include "HashKmerOccTable.hpp"
#include "RandomisedAccess.hpp"
#include "KmerOccurrence.hpp"
#include "HashComputation.hpp"
#include "Params.hpp"
#include "GlobalSettings.hpp"
#include "debug.hpp"
using namespace std;

typedef unsigned long long ULL;


class Hashing {
    MessageLogger& logger;
    RandomisedAccess& seqRandAcc;
    const Params& params;
    
public:
    Hashing(MessageLogger & logger, RandomisedAccess& seqRandAcc, const Params& params)
        : logger(logger), seqRandAcc(seqRandAcc), params(params) {}
    
    
    HashPtr operator()(unsigned numberOfThreads) {
        logger.log("Hashing phase (2/4)");
        
        //1) run the computation to find out how much space needs to be allocated
        logger.log("Computing memory needs");
        
        //2) allocate table exactly
        logger.log("Allocating memory");
        
        //3) run computation again and save results to table
        logger.log("Computing hashes");
        
        
        
        
        
        //create hash table
        HashPtr hash = chooseTableVersion(seqRandAcc.size());
        ULL hashTableSize = getHashSize();
        
        //log table size
        string message("Size of hash table: 2^");
        short tableSizeIndex = static_cast<short>(log2(static_cast<double>(hashTableSize)));
        message += to_string(tableSizeIndex);
        if (tableSizeIndex == HASH_TABLE_SIZE_EXP_MAX) message += " (maximum)";
        if (tableSizeIndex == HASH_TABLE_SIZE_EXP_MIN) message += " (minimum)";
        logger.log(message);
    
        hash->setSize(hashTableSize);
        
        //compute hashes-occurrences
        HashComputation hashComputation(seqRandAcc, params.maxSeqLength, params.kmerLength);
        hashComputation(hash, numberOfThreads);
        
        hash->shrink_to_fit(min(numberOfThreads, 4u));
        
        logger.log("Hashing complete.");
        return hash;
    }
    
private:
    HashPtr chooseTableVersion(ULL seqCount) {
        if (seqCount <= numeric_limits<unsigned>::max()) { //unsigned id versions
            
            if (params.maxSeqLength < numeric_limits<unsigned char>::max()) { //char
                return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>());
            }
            
            if (params.maxSeqLength < numeric_limits<unsigned short>::max()) { //short
                return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsShort>());
            }
            
            return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsUns>()); //unsigned
            
        } else { //ull id versions
            
            if (params.maxSeqLength < numeric_limits<unsigned char>::max()) { //char
                return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UllChar>());
            }
            
            if (params.maxSeqLength < numeric_limits<unsigned short>::max()) { //short
                return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UllShort>());
            }
            
            return HashPtr(new HashKmerOccTable_Impl<KmerOcc_UllUns>()); //unsigned
        }
    }

    ULL getHashSize() {
        double exp;
        
        //table size is not set in parameters (compute it)
        if (params.hashTableSize == 0) {
            exp = selectExp(seqRandAcc.size()/2);
        } else {
            exp = params.hashTableSize;
        }
        
        if (exp > HASH_TABLE_SIZE_EXP_MAX) exp = HASH_TABLE_SIZE_EXP_MAX;
        if (exp < HASH_TABLE_SIZE_EXP_MIN) exp = HASH_TABLE_SIZE_EXP_MIN;
        
        return pow(2, static_cast<int>(floor(exp)));
    }
    
    unsigned selectExp(ULL numOfSeq) {
        //small data
        if (numOfSeq < 5500)  return 20;
        if (numOfSeq < 10000) return 21;
        if (numOfSeq < 14500) return 22;
        if (numOfSeq < 19000) return 23;
        if (numOfSeq < 23500) return 24;
        if (numOfSeq < 28000) return 25;
        
        //large data
        if (numOfSeq < 1000000)   return 26;
        if (numOfSeq < 40000000)  return 27;
        if (numOfSeq < 80000000)  return 28;
        if (numOfSeq < 120000000) return 29;
        if (numOfSeq < 160000000) return 30;
        if (numOfSeq < 200000000) return 31;
        
        return 32;
    }
};

#endif
