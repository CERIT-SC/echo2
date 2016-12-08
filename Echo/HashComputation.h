//
//  HashComputation.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 15/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__HashComputation__
#define __EchoErrorCorrection__HashComputation__

#include <vector>
#include <thread>
#include <cmath>
#include <limits>
#include <algorithm>

#include "Sequence.h"
#include "RandomisedAccess.h"
#include "HashKmerOccTable.h"
#include "Mutex.h"
#include "debug.h"
using namespace std;

typedef unsigned long long ULL;

class HashComputation {
    RandomisedAccess& seqRandAcc;
    vector<Mutex> mutexes;
    HashPtr hashTable;
    
    unsigned kmerLen, maxSeqLen;
    unsigned hashIndBits;
    
public:
    HashComputation(RandomisedAccess& seqRandAcc, unsigned maxSeqLen, unsigned kmerLen)
        : seqRandAcc(seqRandAcc), kmerLen(kmerLen), maxSeqLen(maxSeqLen) {}
    
    void operator()(HashPtr hashTable, unsigned numberOfThreads);
    
private:
    void computeHashesForSection(ULL startIndex, ULL endIndex); //computes for [start, end)
    void computeHashesForSeq(ULL seqIndex, char* seqDataArray, vector<unsigned>& added);
    unsigned hash(const char * startPos, const char *endPos, bool& isComputed);
};


#endif /* defined(__EchoErrorCorrection__HashComputation__) */