//
//  AllocComputation.cpp
//  Echo
//
//  Created by Miloš Šimek on 01/11/2017.
//

#include "AllocComputation.hpp"


vector<atomic<unsigned>>
AllocComputation::operator()(ULL hashSize, unsigned numberOfThreads) {
    assert(numberOfThreads > 0);
    
    //create atomic array for counting
    vector<atomic<unsigned>> atomicArray(hashSize);
    for (auto &i: atomicArray) i = 0;
    this->counterArray = move(atomicArray);
    
    //compute allowed bits for hash indexes
    short exp = log(hashSize)/log(2);
    hashIndBits = numeric_limits<unsigned>::max();
    hashIndBits >>= 32 - exp;
    
    //compute in threads
    vector<thread> threads(numberOfThreads);
    ULL sectStart = 0;
    ULL sectSize = seqRandAcc.size()/numberOfThreads;
    
    for (auto it = threads.begin(); it != threads.end(); it++) {
        *it = thread(&AllocComputation::computeHashesForSection, this,
                     sectStart, sectStart+sectSize);
        sectStart += sectSize;
    }
    
    //wait for it to finish
    for (auto it = threads.begin(); it != threads.end(); it++) {
        it->join();
    }
    
    //compute remainder
    computeHashesForSection(sectStart, seqRandAcc.size());
    
    return move(this->counterArray);
}

void AllocComputation::computeHashesForSection(ULL startIndex, ULL endIndex) {
    //initialize arrays in thread only once (for speed)
    shared_ptr<char> seqData = shared_ptr<char>(new char[maxSeqLen],
                                                [](char* p){ delete [] p; });
    vector<unsigned> added;
    added.reserve(maxSeqLen-kmerLen);
    
    for (ULL i = startIndex; i < endIndex; i++) {
        computeHashesForSeq(i, seqData.get(), added);
    }
}

void AllocComputation::computeHashesForSeq(ULL seqIndex,
                                          char* seqDataArray,
                                          vector<unsigned>& added) {
    Interpreter* seq = seqRandAcc[seqIndex];
    
    //get data for sequence
    unsigned length = seq->getLength();
    for (unsigned i=0; i < length; i++) {
        seqDataArray[i] = seq->at(i);
    }
    
    unsigned hashVal;
    added.clear();
    assert(added.capacity() > 0); //the array must stay allocated for speed
                                  //purposes
    bool isComputed = false;
    
    for (unsigned i=0; i <= length-kmerLen; i++) {
        hashVal = getHash(seqDataArray+i, seqDataArray+i+kmerLen, isComputed);
        if (!isComputed) continue;
        hashVal &= hashIndBits;
        
        //check if this sequence allready have this hash
        if (find(added.begin(), added.end(), hashVal) == added.end()) {
            added.push_back(hashVal);
        } else continue;
        
        //count occurrence
        counterArray[hashVal]++;   //atomic operation
    }
    
    delete seq;
}
