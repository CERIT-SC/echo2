//
//  HistogramComputation.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 18/2/14.
//

#include "HistogramComputation.hpp"


vector<unsigned> HistogramComputation::compute(unsigned minOveralp, double maxErrTolerance, ULL sampleSize) {
    assert(sampleSize <= access.size());
    histogram.clear();
    histReadset.clear();
    
    //for all sequences (from sample)
    for (ULL seqId = 0; seqId < sampleSize; seqId++) {
        if (access.isComplement(seqId)) continue;
        
        unsigned numOfNeighb = countNeighbours(seqId, minOveralp, maxErrTolerance);
        
        if (histReadset.find(seqId) == histReadset.end()) { //every seq. is counted at most 1
            
            //not every neighbour will vote
            unsigned size = neighbTable->getNeighbCount(seqId);
            for (unsigned neighbIndex = 0; neighbIndex < size; neighbIndex++) {
                histReadset.insert(get<0>(neighbTable->getNeighb(seqId, neighbIndex)));
            }
            
            //histogram calculation
            if (numOfNeighb >= histogram.size()) histogram.resize(numOfNeighb+1);
            assert(numOfNeighb < histogram.size());
            histogram[numOfNeighb] += 1;
        }
    }
    
    return histogram;
}

unsigned HistogramComputation::countNeighbours(ULL seqId, unsigned overlap, double errTolerance) {
    unsigned numOfNeighb = 0;
    
    for (unsigned neighbIndex = 0; neighbIndex < neighbTable->getNeighbCount(seqId); neighbIndex++) {
        Neighb neighb = neighbTable->getNeighb(seqId, neighbIndex);
        if (get<1>(neighb) > 0) continue; //offset
        
        unsigned seqPos, neighbPos, overlapSize;
        getOverlapInfo(seqId, neighb, access, seqPos, neighbPos, overlapSize);
        
        //test for neighbourhood with actual (stricter) overlap and errTolerance
        if (overlapSize < overlap) continue;
        if (static_cast<float>(get<2>(neighb))/overlapSize > errTolerance) continue;
        
        numOfNeighb += 1;
    }
    
    return numOfNeighb;
}
