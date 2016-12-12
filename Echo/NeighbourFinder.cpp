//
//  NeighbourFinder.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 21/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "NeighbourFinder.hpp"

NeighbourFinder::NeighbourFinder(const ConstHashPtr hash, const RandomisedAccess& access,
                                 unsigned minOverlap, float errTolerance)
: hashTable(hash), access(access), minOverlap(minOverlap), errTolerance(errTolerance) {}

void NeighbourFinder::operator()(NeighbTablePtr neighbTable, ULL startSeqId, ULL stopSeqId,
                                 unsigned maxCoverage, unsigned numOfThreads) {
    
    assert(numOfThreads > 0);
    assert(startSeqId <= stopSeqId);
    assert(stopSeqId <= access.size());
    assert(stopSeqId <= neighbTable->getSize());
    
    this->neighbTable = neighbTable;
    this->startSeqId = startSeqId;
    this->stopSeqId = stopSeqId;
    this->maxCoverage = maxCoverage;
    
    vector<thread> threads(numOfThreads);
    locks.resize(neighbTable->getSize());
    
    nextHash = 0;
    
    //start threads
    for (auto it = threads.begin(); it != threads.end(); it++) {
        *it = thread(&NeighbourFinder::threadedComputation, this);
    }
    
    //wait for them to finish
    for (auto thr = threads.begin(); thr != threads.end(); thr++) {
        thr->join();
    }
}

void NeighbourFinder::threadedComputation() {
    ULL hash;
    while (1) {
        hash = nextHash++; //atomic operation
        
        if (hash >= hashTable->getSize()) break;
        unsigned compenzation = pow(2, 32) / hashTable->getSize();
        if (maxCoverage && hashTable->getOccurrCount(hash) >= 3*maxCoverage*compenzation) continue;
        
        computeNeighbForHash(hash);
    }
}

void NeighbourFinder::computeNeighbForHash(ULL hash) {
    unsigned occCount = hashTable->getOccurrCount(hash);
    ErrorComputation errorComputation(access);
    
    //check neighbours for all combinations in hash table
    bool first;
    for (unsigned occIndex1 = 0; occIndex1 < occCount; occIndex1++) {
        Occur occurrence1 = hashTable->getOccurr(hash, occIndex1);
        
        if (!doComputeCondition(occurrence1.first)) continue;
        first = true;
        
        for (unsigned occIndex2 = 0; occIndex2 < occCount; occIndex2++) {
            Occur occurrence2 = hashTable->getOccurr(hash, occIndex2);
            
            if(neighbTable->getNeighbCount(occurrence1.first) >= NEIGHB_LIMIT) break;
            
            if(first) { errorComputation.setFirstSeq(occurrence1.first); first = false; }
            errorComputation.setSecondSeq(occurrence2.first);
            
            computeNeighbours(occurrence1, occurrence2, errorComputation);
        }
    }
}

bool NeighbourFinder::doComputeCondition(ULL seqId) {
    if(seqId < startSeqId || seqId >= stopSeqId) return false;
    if(access.isComplement(seqId)) return false;
    if(neighbTable->getNeighbCount(seqId) >= NEIGHB_LIMIT) return false;
    
    return true;
}

void NeighbourFinder::computeNeighbours(Occur occurrence1, Occur occurrence2,
                                        ErrorComputation& errorComputation) {
    
    //getting info about possible neighbours
    unsigned startPos1, startPos2, overlapSize, numOfErr;
    getOverlapInfo(occurrence1, occurrence2, access, startPos1, startPos2, overlapSize);
    
    //test for neighbourhood
    if (overlapSize < minOverlap) return;
    numOfErr = errorComputation(startPos1, startPos2, overlapSize);
    if (static_cast<float>(numOfErr)/overlapSize > errTolerance) return;
    
    //add neighbours to table
    long long offset = static_cast<long long>(occurrence1.second) - occurrence2.second;
    
    //insert second sequence as a neighbour to the first
    locks[occurrence1.first].lock();
    insertNeighbour(occurrence1, occurrence2, overlapSize, offset, numOfErr);
    locks[occurrence1.first].unlock();
}

void NeighbourFinder::insertNeighbour(Occur seqOccur, Occur neighbOccur, unsigned overlapSize, long long offset, unsigned numOfErr) {
    
    //find same neighbour in neighbour table
    unsigned count = neighbTable->getNeighbCount(seqOccur.first);
    for (unsigned index = 0; index < count; index++) {
        Neighb neighbInfo = neighbTable->getNeighb(seqOccur.first, index);
        
        if (get<0>(neighbInfo) == neighbOccur.first) { //if neighbour occurrence in table exists
            
            //1) if offsets are equal don't add
            long long offset = static_cast<long long>(seqOccur.second) - neighbOccur.second;
            if (get<1>(neighbInfo) == offset) return;
            
            //2) check if new found neighbourhood is better then one previously computed
            unsigned pos1, pos2;
            if (get<1>(neighbInfo) >= 0) { //offset
                pos1 = static_cast<unsigned>(get<1>(neighbInfo));
                pos2 = 0;
            } else {
                pos1 = 0;
                pos2 = static_cast<unsigned>(-get<1>(neighbInfo));
            }
            
            Interpreter* seq1 = access[seqOccur.first];
            Interpreter* seq2 = access[get<0>(neighbInfo)];
            unsigned overlap2Size = min(seq1->getLength()-pos1, seq2->getLength()-pos2);
            delete seq1; delete seq2;
            
            //if neighbourhood in table is better, don't add
            double errValue1 = static_cast<double>(numOfErr)/overlapSize; //actual
            double errValue2 = static_cast<double>(get<2>(neighbInfo))/overlap2Size; //from table
            if (errValue1 < errValue2) {
                neighbTable->exchange(seqOccur.first, index, neighbOccur.first, offset, numOfErr);
            }
            
            return;
        }
    }
    
    if (neighbTable->getNeighbCount(seqOccur.first) < NEIGHB_LIMIT)
        neighbTable->add(seqOccur.first, neighbOccur.first, offset, numOfErr);
    
}
