//
//  HistogramComputation.hpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 18/2/14.
//

#ifndef __EchoErrorCorrection__HistogramComputation__
#define __EchoErrorCorrection__HistogramComputation__

#include <vector>
#include <map>
#include <tuple>
#include <set>
#include <algorithm>

#include "RandomisedAccess.hpp"
#include "NeighbourTable.hpp"
#include "Tools.hpp"
using namespace std;

class HistogramComputation {
    const RandomisedAccess& access;
    const ConstNeighbTablePtr neighbTable;
    
    //these 2 variables are altered by algorithm
    //put a locks around them if you want to paralelize this part of algorithm
    vector<unsigned> histogram;
    set<ULL> histReadset;
    
public:
    HistogramComputation(const RandomisedAccess& access, const ConstNeighbTablePtr neighbTable)
    : access(access), neighbTable(neighbTable) {}
    
    vector<unsigned> compute(unsigned minOverlap, double maxErrTolerance, ULL sampleSize);
    
private:
    
    unsigned countNeighbours(ULL seqId, unsigned overlap, double errTolerance);
};

#endif /* defined(__EchoErrorCorrection__HistogramComputation__) */
