 //
//  HashComputation.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 15/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "HashComputation.hpp"

void HashComputation::operator()(HashPtr hashTable, unsigned numberOfThreads) {
    assert(numberOfThreads > 0);
    
    mutexes.resize(hashTable->getSize());
    this->hashTable = hashTable;
    
    //compute allowed bits for hash indexes
    short exp = log(hashTable->getSize())/log(2);
    hashIndBits = numeric_limits<unsigned>::max();
    hashIndBits >>= 32 - exp;
    
    //compute in threads
    vector<thread> threads(numberOfThreads);
    ULL sectStart = 0;
    ULL sectSize = seqRandAcc.size()/numberOfThreads;
    
    for (auto it = threads.begin(); it != threads.end(); it++) {
        *it = thread(&HashComputation::computeHashesForSection, this,
                     sectStart, sectStart+sectSize);
        sectStart += sectSize;
    }
    
    //wait for it to finish
    for (auto it = threads.begin(); it != threads.end(); it++) {
        it->join();
    }
    
    //compute remainder
    computeHashesForSection(sectStart, seqRandAcc.size());
}

void HashComputation::computeHashesForSection(ULL startIndex, ULL endIndex) {
    //initialize arrays in thread only once (for speed)
    shared_ptr<char> seqData = shared_ptr<char>(new char[maxSeqLen],
                                                [](char* p){ delete [] p; });
    vector<unsigned> added;
    added.reserve(maxSeqLen-kmerLen);
    
    for (ULL i = startIndex; i < endIndex; i++) {
        computeHashesForSeq(i, seqData.get(), added);
    }
}

void HashComputation::computeHashesForSeq(ULL seqIndex,
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
        } else {
            //if hashes are equal, keep occurrences from beginning of the
            //sequence
            if (!seq->isCompl()) continue;
            else {  //or from end of the complement
                mutexes[hashVal].lock();
                hashTable->exchange(hashVal, seqIndex, i);
                mutexes[hashVal].unlock();
                continue;
            }
        }
        
        //add occurrence
        mutexes[hashVal].lock();
        hashTable->add(hashVal, seqIndex, i);
        mutexes[hashVal].unlock();
    }
    
    delete seq;
}
