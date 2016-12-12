//
//  SequenceRepair.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 23/03/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__SequenceRepair__
#define __EchoErrorCorrection__SequenceRepair__

#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
#include <array>
#include <thread>

#include "RandomisedAccess.hpp"
#include "Tools.hpp"
#include "debug.hpp"
using namespace std;

typedef unsigned long long ULL;

class SequenceRepair {
    //input
    const RandomisedAccess& access;
    const ConstNeighbTablePtr neighbTable;
    
    //parameters
    const unsigned estCoverage, maxCoverage;
    const float heterozygousRate;
    vector<Hypothesis> hypothesis;
    
    //matrices
    const N44Matrix& confMat;
    N44Matrix logLikelihood;
    
    //result
    vector<string>* repairedSeq;
    vector<string>* quality;
    
public:
    SequenceRepair(const RandomisedAccess& access, const ConstNeighbTablePtr neighbTable,
                   unsigned estCoverage, unsigned maxCoverage, float heterozygousRate,
                   const N44Matrix& confMat);
    
    void operator()(vector<string>& repairedSeq, vector<string>& quality, unsigned numOfThreads);
    
private:
    void repairRange(ULL startId, ULL stopId);
    
    void collectVotes(ULL seqId, vector<vector<VoteInfo>>& votes, vector<unsigned>& numOfVotes);
    double getErrorProbability(unsigned baseNeighbCount, short origBase, Hypothesis expectedBase,
                               const array<double, 16> &baseLogQuality);
    
    char getRepairedBase(unsigned baseNeighbCount, unsigned maxCov, Hypothesis expectedBase, short origBase);
    char computeBaseQuality(double errProb, unsigned baseNeighbCount);
};

#endif /* defined(__EchoErrorCorrection__SequenceRepair__) */
