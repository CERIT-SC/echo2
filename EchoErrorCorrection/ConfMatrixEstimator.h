//
//  ConfMatrixEstimator.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 19/2/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__ConfMatrixEstimator__
#define __EchoErrorCorrection__ConfMatrixEstimator__

#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <limits>
#include <thread>

#include "RandomisedAccess.h"
#include "NeighbourTable.h"
#include "Tools.h"
#include "Mutex.h"
#include "debug.h"
using namespace std;

typedef unsigned long long ULL;

#define CONFMAT_COV_THRESHOLD 1e-10

class ConfMatrixEstimator {
    const RandomisedAccess& access;
    const ConstNeighbTablePtr neighbTable;
    
    const float heterozygousRate;
    const unsigned estCoverage;
    vector<Hypothesis> hypothesis;
    
    const unsigned minOverlap;
    const double errTolerance;
    
    N44Matrix logLikelihood;
    Mutex confMatMutex;
    
public:
    ConfMatrixEstimator(const RandomisedAccess& access, const ConstNeighbTablePtr neighbTable,
                        float heterozygousRate, unsigned estCoverage, unsigned minOverlap,
                        double errTolerance);
    
    
    N44Matrix compute(unsigned maxSeqLen, ULL sampleSize, unsigned numOfThreads);
    
private:
    N44Matrix iteration(unsigned maxSeqLen, ULL sampleSize, unsigned numOfThreads);
    void rangedMatrixComputation(ULL startId, ULL stopId, N44Matrix* confMatrix);
    double getTotalProbability(Hypothesis expectedBase, const array<double, 16> &baseLogQuality);
    
    vector<vector<VoteInfo>> collectVotes(ULL seqId);
    
    N44Matrix initLogLikelihood(unsigned maxSeqLen);
    double difference(const N44Matrix& matNew, const N44Matrix& matOld);
};

#endif /* defined(__EchoErrorCorrection__ConfMatrixEstimator__) */
