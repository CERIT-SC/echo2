//
//  AllocComputation.hpp
//  Echo
//
//  Created by Miloš Šimek on 01/11/2017.
//  Copyright © 2017 Miloš Šimek. All rights reserved.
//

#ifndef AllocComputation_hpp
#define AllocComputation_hpp

// AllocComputation computes allocation size needed for hash table.
// It was created because realocations during computation become bottleneck
// when computing large data on parallel systems.

#include <vector>
#include <atomic>
#include <cmath>
#include <thread>

#include "RandomisedAccess.hpp"
#include "Tools.hpp"
#include "debug.hpp"
using namespace std;

typedef unsigned long long ULL;

class AllocComputation {
    vector<atomic<unsigned>> counterArray;
    RandomisedAccess& seqRandAcc;
    unsigned kmerLen, maxSeqLen;
    unsigned hashIndBits;
    
public:
    AllocComputation(RandomisedAccess& seqRandAcc,
                     unsigned maxSeqLen,
                     unsigned kmerLen) :
                        seqRandAcc(seqRandAcc),
                        maxSeqLen(maxSeqLen),
                        kmerLen(kmerLen) {}
    
    vector<atomic<unsigned>> operator()(ULL hashSize, unsigned numberOfThreads);
    
private:
    void computeHashesForSection(ULL startIndex, ULL endIndex);
    void computeHashesForSeq(ULL seqIndex,
                             char* seqDataArray,
                             vector<unsigned>& added);
};

#endif /* AllocComputation_hpp */
