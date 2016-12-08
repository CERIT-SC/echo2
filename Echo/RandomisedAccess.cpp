//
//  RandomisedAccess.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "RandomisedAccess.h"

RandomisedAccess::RandomisedAccess(const vector<Sequence>& sequences,
                                   bool randomise) : sequences(sequences) {
    assert(sizeof(ULL) >= sizeof(vector<size_t>::size_type));
    
    //setup index array
    ULL index = 0;
    indexes.resize(sequences.size()*2); //every sequence is interpreted 2x (as sequence and complement)
    generate(indexes.begin(), indexes.end(), [&]{ return (index++)/2;}); //0 0 1 1 2 2 ...
    
    //setup complement array
    bool val = true;
    isCompl.resize(sequences.size()*2, false);
    generate(isCompl.begin(), isCompl.end(), [&]()->bool{ val = !val; return val; } );
    
    if (randomise) {
        mt19937_64 generator(1); //same sequence of numbers every run
        vector<ULL>::size_type size = indexes.size();
        ULL randNum;
        
        for (vector<ULL>::size_type i = 0; i < size; i++) {
            randNum = generator()%size;
            
            swap(indexes[i], indexes[randNum]);
            swap(isCompl[i], isCompl[randNum]);
        }
    }
}

ULL RandomisedAccess::size() const {
    return indexes.size();
}

bool RandomisedAccess::isComplement(ULL seqRandIndex) const {
    return isCompl[seqRandIndex];
}

unsigned RandomisedAccess::getSeqLength(ULL seqRandIndex) const {
    assert(seqRandIndex < indexes.size());
    
    return sequences[indexes[seqRandIndex]].getLength();
}

Interpreter* RandomisedAccess::operator[](ULL seqRandIndex) const {
    assert(seqRandIndex < indexes.size());
    const Sequence& seq = sequences[indexes[seqRandIndex]];
    
    if (!isCompl[seqRandIndex]) {
        return seq.interpretAsSeq();
    } else {
        return seq.interpretAsCompl();
    }
}

ULL RandomisedAccess::getOrigIndex(ULL seqRandIndex) const {
    return indexes[seqRandIndex];
}