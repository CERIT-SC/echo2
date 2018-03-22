//
//  NeighbourFinder.hpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 21/09/13.
//

#ifndef __EchoErrorCorrection__NeighbourFinder__
#define __EchoErrorCorrection__NeighbourFinder__

#include <memory>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <thread>
#include <vector>
#include <atomic>

#include "NeighbourTable.hpp"
#include "HashKmerOccTable.hpp"
#include "RandomisedAccess.hpp"
#include "Mutex.hpp"
#include "Tools.hpp"
#include "debug.hpp"
using namespace std;

typedef unsigned long long ULL;
#define NEIGHB_LIMIT 1000

class ErrorComputation;

class NeighbourFinder {
    const ConstHashPtr hashTable;
    const RandomisedAccess& access;
    NeighbTablePtr neighbTable;
    
    vector<Mutex> locks;
    atomic<ULL> nextHash;
    
    unsigned minOverlap;
    float errTolerance;
    ULL startSeqId, stopSeqId;
    unsigned maxCoverage;
    
public:
    NeighbourFinder(const ConstHashPtr hash, const RandomisedAccess& access, unsigned minOverlap,
                    float errTolerance);
    
    void operator()(NeighbTablePtr neighbTable, ULL startSeqId, ULL stopSeqId, unsigned maxCoverage,
                    unsigned numOfThreads);
    //computes for [startSeqId, stopSeqId)
    
private:
    void threadedComputation();
    void computeNeighbForHash(ULL hash);
    bool inline doComputeCondition(ULL seqId);
    
    void computeNeighbours(Occur occurrence1, Occur occurrence2, ErrorComputation& errComputation);
    void insertNeighbour(Occur seqOccur, Occur neighbOccur, unsigned overlapSize, long long offset,
                         unsigned numOfErr);
};


class ErrorComputation {
    vector<short> seq1;
    Interpreter* seq2;
    const RandomisedAccess& access;
    
public:
    ErrorComputation(const RandomisedAccess& access) : access(access), seq2(nullptr) {}
    ~ErrorComputation() { if(seq2) delete seq2; }
    
    void inline setFirstSeq(ULL seqId) {
        Interpreter* sequence = access[seqId];
        unsigned length = sequence->getLength();
        seq1.resize(length);
        
        for (unsigned i = 0; i < length; i++) {
            seq1[i] = sequence->at(i);
        }
        
        delete sequence;
    }
    
    void inline setSecondSeq(ULL seqId) {
        if (seq2) delete seq2;
        seq2 = access[seqId];
    }
    
    unsigned inline operator()(unsigned startPos1, unsigned startPos2, unsigned overlapSize) {
        unsigned numOfErr = 0;
        
        for (unsigned i = 0; i < overlapSize; i++) {
            if (seq1[startPos1+i] != seq2->at(startPos2+i)) numOfErr++;
        }
        
        return numOfErr;
    }
};

#endif /* defined(__EchoErrorCorrection__NeighbourFinder__) */
