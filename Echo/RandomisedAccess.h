//
//  RandomisedAccess.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__RandomisedAccess__
#define __EchoErrorCorrection__RandomisedAccess__

#include <vector>
#include <algorithm>
#include <random>
#include <memory>

#include "Sequence.h"
#include "debug.h"
using namespace std;

typedef unsigned long long ULL;

class RandomisedAccess {
    const vector<Sequence>& sequences;
    
    vector<ULL> indexes;
    vector<bool> isCompl;
public:
    RandomisedAccess(const vector<Sequence>& sequences, bool randomise = true);
    
    ULL size() const;
    bool isComplement(ULL seqRandIndex) const;
    unsigned getSeqLength(ULL seqRandIndex) const;
    Interpreter* operator[](ULL seqRandIndex) const;
            //every sequence is interpreted once as sequence and once as complement

    ULL getOrigIndex(ULL seqRandIndex) const;
};

#endif /* defined(__EchoErrorCorrection__RandomisedAccess__) */
