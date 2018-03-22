//
//  ErrorCorrection.hpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//

#ifndef EchoErrorCorrection_ErrorCorrection_h
#define EchoErrorCorrection_ErrorCorrection_h

#include <vector>
#include <string>

#include "MessageLogger.hpp"
#include "NeighbourOccurrence.hpp"
#include "NeighbourTable.hpp"
#include "NeighbourFinder.hpp"
#include "RandomisedAccess.hpp"
#include "SequenceRepair.hpp"
#include "Params.hpp"
using namespace std;

class ErrorCorrection {
    MessageLogger& logger;
    
public:
    ErrorCorrection(MessageLogger& logger) : logger(logger) {}
    
    void operator()(const Params & params, HashPtr hashTable, const RandomisedAccess& access,
                    vector<string>& repairedSeq, vector<string>& quality, unsigned numOfThreads) {
        logger.log("Error correction phase (4/4)");
        
        logger.log("Finding neighbours for sequences.");
        
        //prepare neighbour table
        NeighbTablePtr neighbTable(new NeighbourTable_Impl<NeighOccurrence>());
        neighbTable->setSize(access.size());
        
        //find neighbours
        NeighbourFinder finder(hashTable, access, params.besth, params.beste);
        finder(neighbTable, 0, access.size(), params.estCov, numOfThreads);
        
        //free memory for output data
        hashTable->clear();
        hashTable.reset();
        
        logger.log("Repairing sequences.");
        
        SequenceRepair seqRep(access, neighbTable, params.estCov, params.maxCov,
                              params.heterRate, params.confMat);
        
        seqRep(repairedSeq, quality, numOfThreads);
        
        logger.log("Error correction complete.");
    }
};

#endif
