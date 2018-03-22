//
//  NeighbourTable.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 21/09/13.
//


//This table stores sequence neighbours.

#ifndef __EchoErrorCorrection__NeighbourTable__
#define __EchoErrorCorrection__NeighbourTable__

#include <vector>
#include <thread>
#include <algorithm>

#include "DynamicArray.hpp"
#include "debug.hpp"
using namespace std;


class NeighbourTable;

typedef unsigned long long ULL;
typedef tuple<ULL, long long, unsigned> Neighb; //neighbour id, offset, numOfErr
typedef shared_ptr<NeighbourTable> NeighbTablePtr;
typedef shared_ptr<const NeighbourTable> ConstNeighbTablePtr;


class NeighbourTable {
public:
    virtual ~NeighbourTable() {}
    
    virtual void setSize(ULL size) = 0;
    virtual ULL getSize() const = 0;
    virtual void add(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr) = 0;
    virtual void exchange(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr) = 0;
    virtual void exchange(ULL seqId, unsigned index, ULL neighbId, long long offset, unsigned numOfErr) = 0;
    
    virtual unsigned getNeighbCount(ULL seqId) const = 0;
    virtual Neighb getNeighb(ULL seqId, unsigned index) const = 0;
    virtual void resizeNeighb(ULL seqId, unsigned newSize) = 0;
    
    virtual void shrink_to_fit(unsigned numberOfThreads) = 0;
};


template <class NeighbOccType>
class NeighbourTable_Impl : public NeighbourTable {
    vector<DynamicArray<NeighbOccType>> table;
    
public:
    virtual void setSize(ULL size);
    virtual ULL getSize() const;
    virtual void add(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr);
    virtual void exchange(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr);
    virtual void exchange(ULL seqId, unsigned index, ULL neighbId, long long offset, unsigned numOfErr);
    
    virtual unsigned getNeighbCount(ULL seqId) const;
    virtual Neighb getNeighb(ULL seqId, unsigned index) const;
    virtual void resizeNeighb(ULL seqId, unsigned newSize);
    
    virtual void shrink_to_fit(unsigned numberOfThreads);
    
private:
    void shrinkSection(ULL startIndex, ULL stopIndex);
    
};



//IMPLEMENTATION
template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::setSize(ULL size) {
    table.resize(size);
}

template <class NeighbOccType>
ULL NeighbourTable_Impl<NeighbOccType>::getSize() const {
    return table.size();
}

template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::add(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr) {
    assert(seqId < table.size());
    
    table[seqId].push_back(NeighbOccType(neighbId, offset, numOfErr));
}

template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::
exchange(ULL seqId, ULL neighbId, long long offset, unsigned numOfErr) {
    assert(seqId < table.size());
    assert(neighbId < table.size());
    
    NeighbOccType actual(neighbId, offset, numOfErr);
    auto found = find_if(table[seqId].begin(), table[seqId].end(),
                         [&](NeighbOccType& occ){ return occ == actual; });
    
    if (found != table[seqId].end()) *found = actual;
    else table[seqId].push_back(actual);
}

template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::
exchange(ULL seqId, unsigned index, ULL neighbId, long long offset, unsigned numOfErr) {
    assert(seqId < table.size());
    assert(neighbId < table.size());
    assert(index < table[seqId].size());
    
    table[seqId][index] = NeighbOccType(neighbId, offset, numOfErr);
}

template <class NeighbOccType>
unsigned NeighbourTable_Impl<NeighbOccType>::getNeighbCount(ULL seqId) const {
    assert(seqId < table.size());
    
    return table[seqId].size();
}

template <class NeighbOccType>
Neighb NeighbourTable_Impl<NeighbOccType>::getNeighb(ULL seqId, unsigned index) const {
    assert(seqId < table.size());
    assert(index < table[seqId].size());
    
    const NeighbOccType& result = table[seqId][index];
    
    return Neighb(result.getId(), result.getOffset(), result.getNumOfErr());
}

template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::resizeNeighb(ULL seqId, unsigned newSize) {
    assert(seqId < table.size());
    
    table[seqId].resize(newSize);
}

template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::shrink_to_fit(unsigned numberOfThreads) {
    assert(numberOfThreads != 0);
    
    //launch shrinking in multiple threads
    vector<thread> threads(numberOfThreads);
    ULL sectStart = 0;
    ULL sectSize = table.size()/numberOfThreads;
    
    for (auto thr = threads.begin(); thr != threads.end(); thr++) {
        *thr = thread(&NeighbourTable_Impl<NeighbOccType>::shrinkSection,
                      this, sectStart, sectStart+sectSize);
        sectStart+=sectSize;
    }
    
    //wait for them to finish
    for (auto thr = threads.begin(); thr != threads.end(); thr++) {
        thr->join();
    }
    
    //fix remainder
    shrinkSection(sectStart, table.size());
}

//PRIVATE
template <class NeighbOccType>
void NeighbourTable_Impl<NeighbOccType>::shrinkSection(ULL startIndex, ULL stopIndex) { //computed for [start,stop)
    
    for (ULL index = startIndex; index < stopIndex; index++) {
        table[index].shrink_to_fit();
    }
}

#endif /* defined(__EchoErrorCorrection__NeighbourTable__) */
