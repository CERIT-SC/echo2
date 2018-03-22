//
//  ConfMatrixEstimator.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 19/2/14.
//

#include "ConfMatrixEstimator.hpp"

ConfMatrixEstimator::ConfMatrixEstimator(const RandomisedAccess& access,
                    const ConstNeighbTablePtr neighbTable,
                    float heterozygousRate, unsigned estCoverage, unsigned minOverlap,
                    double errTolerance)
: access(access), neighbTable(neighbTable), heterozygousRate(heterozygousRate),
estCoverage(estCoverage), minOverlap(minOverlap), errTolerance(errTolerance) {}


N44Matrix ConfMatrixEstimator::compute(unsigned maxSeqLen, ULL sampleSize, unsigned numOfThreads) {
    assert(sampleSize <= access.size());
    N44Matrix confMat(maxSeqLen);
    hypothesis = generateHypothesis(heterozygousRate > 0);
    logLikelihood = initLogLikelihood(maxSeqLen);
    
    for (short iter=0; iter < 10; iter++) {
        if (iter != 0) confToLikelihood(confMat, logLikelihood);
        
        N44Matrix newMatrix = iteration(maxSeqLen, sampleSize, numOfThreads);
        
        if (difference(newMatrix, confMat) <= CONFMAT_COV_THRESHOLD) break;
        confMat = newMatrix;
    }
    
    return confMat;
}

N44Matrix ConfMatrixEstimator::iteration(unsigned maxSeqLen, ULL sampleSize, unsigned numOfThreads) {
    N44Matrix confMat(maxSeqLen);
    
    vector<thread> threads(numOfThreads);
    ULL sectStartId = 0;
    ULL sectSize = sampleSize/numOfThreads;
    
    //start threads
    for (auto & thr: threads) {
        thr = thread(&ConfMatrixEstimator::rangedMatrixComputation, this,
                     sectStartId, sectStartId + sectSize, &confMat);
        sectStartId += sectSize;
    }
    
    //wait for them to finish
    for (auto & thr: threads) thr.join();
    
    //remainder
    rangedMatrixComputation(sectStartId, sampleSize, &confMat);
    
    return confMat;
}

void ConfMatrixEstimator::rangedMatrixComputation(ULL startId, ULL stopId, N44Matrix* confMatrix) {
    N44Matrix rangeMatrix(confMatrix->getSize());
    
    for (ULL seqId=startId; seqId < stopId; seqId++) {
        if(access.isComplement(seqId)) continue;
        
        vector<vector<VoteInfo>> votes = collectVotes(seqId);
        Interpreter * seq = access[seqId];
        
        //going through bases of sequence
        for (unsigned baseIndex = 0; baseIndex < seq->getLength(); baseIndex++) {
            Hypothesis expectedBase; //mse (mean square error)
            array<double, 16> baseLogQuality; //logarithmic value
            
            mlEstimation(baseIndex, votes, hypothesis, logLikelihood, heterozygousRate,
                         expectedBase, baseLogQuality);
            
            double totalProbability = getTotalProbability(expectedBase, baseLogQuality);
            short base = seq->at(baseIndex);
            if (totalProbability == 0.0 || base >= 4) continue;
            
            //compute comfusion matrix
            for (const Hypothesis& hyp: hypothesis) {
                double prob = exp(baseLogQuality[hyp.b1b2] - baseLogQuality[expectedBase.b1b2]);
                
                rangeMatrix(baseIndex, hyp.b1, base) += 0.5 * prob/totalProbability;
                rangeMatrix(baseIndex, hyp.b2, base) += 0.5 * prob/totalProbability;
            }
        }
        
        delete seq;
    }
    
    confMatMutex.lock();
    *confMatrix += rangeMatrix;
    confMatMutex.unlock();
}

double ConfMatrixEstimator::getTotalProbability(Hypothesis expectedBase,
                                                const array<double, 16> &baseLogQuality) {
    double totalProb = 0;
    
    for (const Hypothesis& hyp: hypothesis) {
        double prob = exp(baseLogQuality[hyp.b1b2] - baseLogQuality[expectedBase.b1b2]);
        totalProb += prob;
    }
    
    return totalProb;
}

vector<vector<VoteInfo>> ConfMatrixEstimator::collectVotes(ULL seqId) {
    vector<vector<VoteInfo>> votes(access.getSeqLength(seqId));
    
    for (unsigned neighbIndex = 0; neighbIndex < neighbTable->getNeighbCount(seqId); neighbIndex++) {
        Neighb neighb = neighbTable->getNeighb(seqId, neighbIndex);
        
        unsigned seqPos, neighbPos, overlapSize;
        getOverlapInfo(seqId, neighb, access, seqPos, neighbPos, overlapSize);
        
        //test for neighbourhood with stricter overlap and errTolerance
        if (overlapSize < minOverlap) continue;
        if (static_cast<float>(get<2>(neighb))/overlapSize > errTolerance) continue;
        
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
            
            assert(seqPos+overlapIndex < access.getSeqLength(seqId));
            votes[seqPos+overlapIndex].push_back(VoteInfo(pos, base, neighbSeq->isCompl()));
            
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
    
    return votes;
}

N44Matrix ConfMatrixEstimator::initLogLikelihood(unsigned maxSeqLen) {
    N44Matrix matrix(maxSeqLen);
    
    for (unsigned pos = 0; pos < maxSeqLen; pos++) {
        for (short b1 = 0; b1 < 4; b1++) {
            for (short b2 = 0; b2 < 4; b2++) {
                if (b1 == b2)   matrix(pos, b1, b2) = log(0.99);
                else            matrix(pos, b1, b2) = log(0.01/3);
            }
        }
    }
    
    return matrix;
}

double ConfMatrixEstimator::difference(const N44Matrix& matNew, const N44Matrix& matOld) {
    assert(matOld.getSize() == matNew.getSize());
    double sum = 0;
    double diff = 0;
    
    for (unsigned i=0; i < matOld.getSize(); i++) {
        for (short b1 = 0; b1 < 4; b1++) {
            for (short b2 = 0; b2 < 4; b2++) {
                sum += abs(matOld(i, b1, b2));
                diff += abs(matOld(i, b1, b2) - matNew(i,b1,b2));
            }
        }
    }
    
    if (sum == 0) return numeric_limits<double>::infinity();
    return diff/sum;
}
