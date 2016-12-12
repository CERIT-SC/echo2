//
//  HashKmerOccTable.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

/*
 Table in which all occurrences of given K-mer represented by hash
 are strored.
 */


#ifndef __EchoErrorCorrection__HashKmerOccTable__
#define __EchoErrorCorrection__HashKmerOccTable__

#include <vector>
#include <thread>
#include <algorithm>

#include "KmerOccurrence.hpp"
#include "DynamicArray.hpp"
#include "debug.hpp"
using namespace std;

class HashKmerOccTable;

typedef unsigned long long ULL;
typedef pair<ULL, unsigned> Occur; //id and position
typedef shared_ptr<HashKmerOccTable> HashPtr;
typedef shared_ptr<const HashKmerOccTable> ConstHashPtr;


class HashKmerOccTable {
public:
    virtual ~HashKmerOccTable() {}
    
    virtual void setSize(ULL size, unsigned hashCapacity = 0) = 0;
    virtual ULL getSize() const = 0;
    virtual void add(ULL hash, ULL seqId, unsigned pos) = 0;
    virtual void exchange(ULL hash, ULL seqId, unsigned pos) = 0;
   
    virtual unsigned getOccurrCount(ULL hash) const = 0;
    virtual Occur getOccurr(ULL hash, unsigned index) const = 0;
    
    //virtual void sortTable() = 0;
    virtual void clear() = 0;
    virtual void shrink_to_fit(unsigned numberOfThreads) = 0;
};



template<class KmerOccType>
class HashKmerOccTable_Impl : public HashKmerOccTable {
    vector<DynamicArray<KmerOccType>> table;
    
public:
    //virtual ~HashKmerOccTable_Impl() {}
    
    virtual void setSize(ULL size, unsigned hashCapacity = 0);
    virtual ULL getSize() const;
    virtual void add(ULL hash, ULL seqId, unsigned pos);
    virtual void exchange(ULL hash, ULL seqId, unsigned pos);
    
    virtual unsigned getOccurrCount(ULL hash) const;
    virtual Occur getOccurr(ULL hash, unsigned index) const;
    
    //virtual void sortTable();
    virtual void clear();
    virtual void shrink_to_fit(unsigned numberOfThreads);
    
private:
    void shrinkSection(ULL startIndex, ULL stopIndex);
    
};





//TEMPLATE IMPLEMENTATION

template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::setSize(ULL size, unsigned hashCapacity) {
    ULL prevSize = table.size();
    table.resize(size);
    
    if (hashCapacity) {
        for (ULL index = prevSize; index < table.size(); index++) {
            table[index].reserve(hashCapacity);
        }
    }
}

template <class KmerOccType>
ULL HashKmerOccTable_Impl<KmerOccType>::getSize() const {
    return table.size();
}

template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::add(ULL hash, ULL seqId, unsigned pos) {
    assert(hash < table.size());
    
    table[hash].push_back(KmerOccType(seqId, pos));
}

template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::exchange(ULL hash, ULL seqId, unsigned pos) {
    assert(hash < table.size());
    
    auto found = find_if(table[hash].begin(), table[hash].end(),
            [&](KmerOccType& elm) { return elm.getId() == seqId; });
    if (found != table[hash].end()) {
        *found = KmerOccType(seqId, pos);
    } else {
        table[hash].push_back(KmerOccType(seqId, pos));
    }
}

template <class KmerOccType>
unsigned HashKmerOccTable_Impl<KmerOccType>::getOccurrCount(ULL hash) const {
    assert(hash < table.size());
    
    return table[hash].size();
}

template <class KmerOccType>
Occur HashKmerOccTable_Impl<KmerOccType>::getOccurr(ULL hash, unsigned index) const {
    assert(hash < table.size());
    assert(index < table[hash].size());
    
    const KmerOccType& result = table[hash][index];
    
    return Occur(result.getId(), result.getPos());
}

//template <class KmerOccType>
//void HashKmerOccTable_Impl<KmerOccType>::sortTable() {
//    for (auto &hash: table) {
//        
//        sort(hash.begin(), hash.end(),
//                  [](KmerOccType& first, KmerOccType& second)
//                        {return first.getId() < second.getId();});
//    }
//}

template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::clear() {
    table.clear();
    table.shrink_to_fit();
}

template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::shrink_to_fit(unsigned numberOfThreads) {
    assert(numberOfThreads != 0);
    
    //launch shrinking in multiple threads
    vector<thread> threads(numberOfThreads);
    ULL sectStart = 0;
    ULL sectSize = table.size()/numberOfThreads;
    
    for (auto thr = threads.begin(); thr != threads.end(); thr++) {
        *thr = thread(&HashKmerOccTable_Impl<KmerOccType>::shrinkSection, this, sectStart, sectStart+sectSize);
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
template <class KmerOccType>
void HashKmerOccTable_Impl<KmerOccType>::shrinkSection(ULL startIndex, ULL stopIndex) { //computed for [start,stop)
    
    for (ULL index = startIndex; index < stopIndex; index++) {
        table[index].shrink_to_fit();
    }
}

#endif /* defined(__EchoErrorCorrection__HashKmerOccTable__) */
