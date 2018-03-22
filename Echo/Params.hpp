//
//  Params.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 15/09/13.
//

#ifndef EchoErrorCorrection_Params_h
#define EchoErrorCorrection_Params_h

#include <vector>
#include <cmath>
#include "Sequence.hpp"
#include "Options.hpp"
#include "Tools.hpp"
#include "debug.hpp"

struct Params {
    unsigned kmerLength, maxSeqLength;
    unsigned minh, maxh; //h - overlap
    double mine, maxe;   //e - error tolerance
    float heterRate;
    int hashTableSize;
    
    unsigned besth, estCov, maxCov; //h is minimum overlap length
    double beste;
    N44Matrix confMat;
    
    unsigned long long sampleSize; //for param. selection
    
    Params() {
        kmerLength = maxSeqLength = minh = maxh = 0;
        mine = maxe = 0.0;
        heterRate = 0.0f;
        hashTableSize = 0;
        
        besth = estCov = maxCov = 0;
        beste = 0.0;
        
        sampleSize = 0;
    }
};

Params setParams(const Options& opt, const vector<Sequence>& seqArray) {
    Params params;
    
    //max length
    unsigned maxLen = 0;
    for (auto it = seqArray.begin(); it != seqArray.end(); it++) {
        if (it->getLength() > maxLen) maxLen = it->getLength();
    }
    params.maxSeqLength = maxLen;
    
    //kmer length
    if (opt.kmerLenght()) {
        params.kmerLength = opt.kmerLenght();
    } else {
        unsigned k1 = maxLen/5.0;
        unsigned k2 = ceil(log(seqArray.size()*2*maxLen)/log(4.0));
        params.kmerLength = max(k1,k2);
    }
    
    //minh
    if (opt.minOverlap()) {
        params.minh = opt.minOverlap();
    } else {
        params.minh = maxLen*0.5;
    }
    
    //maxh
    if (opt.maxOverlap()) {
        params.maxh = opt.maxOverlap();
    } else {
        params.maxh = maxLen*0.66;
    }
    
    //mine
    if (opt.minErrToler()) {
        params.mine = opt.minErrToler();
    } else {
        params.mine = 0.05;
    }
    
    //maxe
    if (opt.maxErrToler()) {
        params.maxe = opt.maxErrToler();
    } else {
        params.maxe = 0.15;
    }
    
    //sample size
    if (opt.paramSampleSize()) {
        params.sampleSize = opt.paramSampleSize();
    } else {
        params.sampleSize = 100000;
    }
    
    //heterozygous rate
    if (opt.heterRate()) {
        params.heterRate = opt.heterRate();
    }
    
    //hash table size exponent
    if (opt.hashTableSize()) {
        params.hashTableSize = opt.hashTableSize();
    }
    
//#ifdef SHORT_OUTPUT_DEB
//    cout << "TEST - max. len: " << params.maxSeqLength << endl;
//    cout << "TEST - kmerLength: " << params.kmerLength << endl;
//#endif
    
    return params;
}

#endif
