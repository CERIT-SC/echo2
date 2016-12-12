//
//  SequenceRepair.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 23/03/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#include "SequenceRepair.hpp"

SequenceRepair::SequenceRepair(const RandomisedAccess& access,
                               const ConstNeighbTablePtr neighbTable,
                               unsigned estCoverage, unsigned maxCoverage, float heterozygousRate,
                               const N44Matrix& confMat)
: access(access), neighbTable(neighbTable), estCoverage(estCoverage), maxCoverage(maxCoverage),
heterozygousRate(heterozygousRate), confMat(confMat) {}


void SequenceRepair::operator()(vector<string>& repairedSeq, vector<string>& quality, unsigned numOfThreads) {
    hypothesis = generateHypothesis(heterozygousRate > 0);
    logLikelihood = N44Matrix(confMat.getSize());
    confToLikelihood(confMat, logLikelihood);
    
    //setup output
    this->repairedSeq = &repairedSeq;
    this->quality = &quality;
    repairedSeq.resize(access.size());
    quality.resize(access.size());
    
    //parallelize
    vector<thread> threads(numOfThreads);
    ULL sectStartId = 0;
    ULL sectSize = access.size()/numOfThreads;
    
    //start threads
    for (auto &thr: threads) {
        thr = thread(&SequenceRepair::repairRange, this, sectStartId, sectStartId + sectSize);
        sectStartId += sectSize;
    }
    
    //wait for them to finish
    for (auto & thr: threads) thr.join();
    
    //fix remainder
    repairRange(sectStartId, access.size());
}

//threaded method
void SequenceRepair::repairRange(ULL startId, ULL stopId) {
    vector<string>& repairedSeq = *this->repairedSeq;
    vector<string>& quality = *this->quality;
    
    for (ULL seqId = startId; seqId < stopId; seqId++) {
        if(access.isComplement(seqId)) continue;
        
        //collect votes
        vector<vector<VoteInfo>> votes;
        vector<unsigned> numOfVotes;
        collectVotes(seqId, votes, numOfVotes);
        
        //prepare sequence
        Interpreter * seq = access[seqId];
        unsigned seqLength = seq->getLength();
        repairedSeq[seqId].resize(seqLength);
        quality[seqId].resize(seqLength);
        
        //going through bases of sequence
        for (unsigned baseIndex = 0; baseIndex < seq->getLength(); baseIndex++) {
            //ML estimation
            Hypothesis expectedBase; //mse (mean square error)
            array<double, 16> baseLogQuality; //logarithmic value
            mlEstimation(baseIndex, votes, hypothesis, logLikelihood, heterozygousRate,
                         expectedBase, baseLogQuality);
            
            //get probability
            double errProb = getErrorProbability(numOfVotes[baseIndex], seq->at(baseIndex), expectedBase,
                                                 baseLogQuality);
            
            //save results
            repairedSeq[seqId][baseIndex] = getRepairedBase(numOfVotes[baseIndex], maxCoverage,
                                                            expectedBase, seq->at(baseIndex));
            quality[seqId][baseIndex] = computeBaseQuality(errProb, numOfVotes[baseIndex]);
        }
    }
}

void SequenceRepair::collectVotes(ULL seqId, vector<vector<VoteInfo>>& votes, vector<unsigned>& numOfVotes) {
    unsigned seqLen = access.getSeqLength(seqId);
    votes.resize(seqLen);
    numOfVotes.resize(seqLen);
    
    //go through all neighbours
    for (unsigned neighbIndex = 0; neighbIndex < neighbTable->getNeighbCount(seqId); neighbIndex++) {
        Neighb neighb = neighbTable->getNeighb(seqId, neighbIndex);
        
        unsigned seqPos, neighbPos, overlapSize;
        getOverlapInfo(seqId, neighb, access, seqPos, neighbPos, overlapSize);
        
        Interpreter * neighbSeq = access[get<0>(neighb)];
        
        //collect votes for all bases in overlap
        for (unsigned overlapIndex=0; overlapIndex < overlapSize; overlapIndex++) {
            unsigned pos = neighbPos + overlapIndex;
            short base = neighbSeq->at(pos);
            if (base >= 4) continue;
            
            //get position and base of original sequence
            if (neighbSeq->isCompl()) {
                base = 3-base;
                pos = neighbSeq->getLength() - pos - 1;
            }
            
            //save votes
            assert(seqPos+overlapIndex < access.getSeqLength(seqId));
            votes[seqPos+overlapIndex].push_back(VoteInfo(pos, base, neighbSeq->isCompl()));
            numOfVotes[seqPos+overlapIndex] += 1;
            
            //original reads gets to vote multiple times. (I would like to know why.)
            if (seqId == get<0>(neighb) && heterozygousRate == 0.0f) {
                int nprior = max(1, static_cast<int>(floor(sqrt(static_cast<double>(estCoverage)))));
                for (int z=0; z < nprior; z++) {
                    votes[seqPos+overlapIndex].push_back(
                        VoteInfo(pos, base, neighbSeq->isCompl(), true));
                }
            }
            
        }
        
        delete neighbSeq;
    }
}

double SequenceRepair::getErrorProbability(unsigned baseNeighbCount, short origBase, Hypothesis expectedBase,
                                                const array<double, 16> &baseLogQuality) {
    double totalProb = 0, errorProb = 0;
    
    //set original base if received too many/few votes
    if(baseNeighbCount == 0 || (baseNeighbCount > maxCoverage && maxCoverage != 0)) {
        expectedBase.b1 = expectedBase.b2 = origBase;
    }
    
    for (const Hypothesis& hyp: hypothesis) {
        double prob = exp(baseLogQuality[hyp.b1b2] - baseLogQuality[expectedBase.b1b2]);
        totalProb += prob;
        
        if (hyp.b1 != expectedBase.b1 || hyp.b2 != expectedBase.b2) errorProb += prob;
    }
    errorProb /= totalProb;
    
    return errorProb;
}

char SequenceRepair::getRepairedBase(unsigned baseNeighbCount, unsigned maxCov, Hypothesis expectedBase, short origBase) {
    assert(origBase <= 5);
    
    auto getCharBase = [](int b)->char {
        switch(b) {
            case 0: return 'A';
            case 1: return 'C';
            case 2: return 'G';
            case 3: return 'T';
                
            case 4: return 'N';
            case 5: return 'Z';
        }
        
        return 0;
    };
    
    //accept correction
    if (baseNeighbCount!=0 && (baseNeighbCount<=maxCov || maxCov==0)) {
        if(expectedBase.b1==expectedBase.b2) return getCharBase(expectedBase.b1);
        
        if(expectedBase.b1==0) {
            if(expectedBase.b2==1) return 'M';
            if(expectedBase.b2==2) return 'R';
            if(expectedBase.b2==3) return 'W';
        } else if(expectedBase.b1==1) {
            if(expectedBase.b2==2) return 'S';
            if(expectedBase.b2==3) return 'Y';
        } else if(expectedBase.b1==2) {
            if(expectedBase.b2==3) return 'K';
        }
    }
    
    //if received too many/few votes reject correction
    return getCharBase(origBase);
}

char SequenceRepair::computeBaseQuality(double errProb, unsigned baseNeighbCount) {
    if (errProb >= 1e-100) {
        double vote = max(1.0, static_cast<double>(baseNeighbCount));
        int interm = static_cast<int>(floor(-10.0*log(errProb) / log(10.0) / vote));
        
        interm = max(0, interm);
        return min(93, interm) + 33;
    }
        
    return 33+93;
}
