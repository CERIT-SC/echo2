//
//  ParameterSelection.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//

#ifndef EchoErrorCorrection_ParameterSelection_h
#define EchoErrorCorrection_ParameterSelection_h

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>

#include "MessageLogger.hpp"
#include "HashKmerOccTable.hpp"
#include "NeighbourOccurrence.hpp"
#include "NeighbourFinder.hpp"
#include "HistogramComputation.hpp"
#include "ConfMatrixEstimator.hpp"
#include "Poisson.hpp"
#include "Params.hpp"
using namespace std;

typedef vector<unsigned> Histogram;
typedef unsigned long long ULL;


class ParameterSelection {
    MessageLogger& logger;
    
    struct TotalVariations {
        unsigned overlap;
        double errToler;
        unsigned estCov; //estimated coverage
        double estTotalVar; //estimated total variation
        
        TotalVariations() : overlap(0), errToler(0), estCov(0), estTotalVar(0) {}
    };
    
public:
    ParameterSelection(MessageLogger& logger) : logger(logger) {}
    
    void operator()(Params & params, const ConstHashPtr hashTable, const RandomisedAccess& access, unsigned numOfThreads) {
        logger.log("Parameter selection phase (3/4)");
        
        logger.log("Creating sample.");
        
        //prepare neighbour table
        NeighbTablePtr sampleNeighbTable(new NeighbourTable_Impl<NeighOccurrence>());
        sampleNeighbTable->setSize(access.size());
        ULL sampleSize = getSampleSize(params, access);
        
        //find neighbours
        NeighbourFinder finder(hashTable, access, params.minh, params.maxe);
        finder(sampleNeighbTable, 0, sampleSize, 0, numOfThreads);
        
        
        logger.log("Grid searching optimal parameters.");
        
        HistogramComputation histComp(access, sampleNeighbTable);
        vector<TotalVariations> variations;
        
        for (unsigned overlap = params.minh; overlap <= params.maxh; overlap += 2) {
            for (double errToler = params.mine; errToler < params.maxe+0.01; errToler += 0.05) {
                
                vector<unsigned> histogram = histComp.compute(overlap, errToler, sampleSize);
                
                TotalVariations v;
                v.overlap = overlap;
                v.errToler = errToler;
                poissonDistEstimation(vector<double>(histogram.begin(), histogram.end()), v.estCov, v.estTotalVar);
                
                variations.push_back(v);
            }
        }
        
        //find parameters with lowest variation
        TotalVariations bestParam;
        double minTotVar = numeric_limits<double>::infinity();
        for (unsigned index=0; index < variations.size(); index++) {
            if (variations[index].estTotalVar < minTotVar) {
                minTotVar = variations[index].estTotalVar;
                bestParam = variations[index];
            }
        }
        
        //set parameters
        params.besth = bestParam.overlap;
        params.beste = bestParam.errToler;
        params.estCov = bestParam.estCov;
        double estCov = static_cast<double>(bestParam.estCov);
        params.maxCov = max(3u, static_cast<unsigned>(ceil(estCov + 5*pow(estCov, 0.5))));
        
        //log parameters
        string filler(25, ' ');
        string message("Selected parameters:\n");
        message += filler + string("Estimated coverage: ") + to_string(params.estCov) + "\n";
        message += filler + string("Maximum coverage: ") + to_string(params.maxCov) + "\n";
        message += filler + string("Overlap: ") + to_string(params.besth) + "\n";
        message += filler + string("Error tolerance: ") + to_string(params.beste);
        logger.log(message);
        
        logger.log("Estimating confusion matrix.");
        
        ConfMatrixEstimator matrixEstimator(access, sampleNeighbTable, params.heterRate, estCov, params.besth, params.beste);
        params.confMat = matrixEstimator.compute(params.maxSeqLength, sampleSize, numOfThreads);
        
        logger.log("Parameter selection complete.");
    }
    
private:
    ULL getSampleSize(const Params& params, const RandomisedAccess& access) {
        ULL result = params.sampleSize;
        
        if (result < 1000) result = min(1000ull, access.size());
        if (result > access.size()) result = access.size();
        return result;
    }
};

#endif
