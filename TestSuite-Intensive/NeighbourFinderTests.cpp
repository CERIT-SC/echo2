//
//  NeighbourFinderTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 12/02/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#include "NeighbourFinderTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(NeighbourFinderTests);

NeighbourFinderTests::NeighbourFinderTests() {
    //error computation test
    sequences.push_back(Sequence(
            "GGTACTTTTATTTTCGGAGCTGTGGGCAACATTCAAATACTAAGCGAAGCTCCGGCATGAAGTGCTGGG"));
    sequences.push_back(Sequence(
        "TTTTGGTACGTTTATTTTCGGAGCTGTGAGCAACATTCAAATACTAAGCGAAGCTCCGGCAGGAAGTGCA")); //includes 4 mismatches
    
    access = new RandomisedAccess(sequences, false);
    errComp = new ErrorComputation(*access);
    
    //NeighbourFinder tests
    loader = new SequenceFileLoader;
    loader->load("TestSuiteFiles/normal.txt", sequenceArray);
    access2 = new RandomisedAccess(sequenceArray, false);
    hashComp = new HashComputation(*access2, 100, 15);
    hashTable = HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>);
    hashTable->setSize(65536);
    hashComp->operator()(hashTable, 8);
    finder = new NeighbourFinder(hashTable, *access2,38, 0.15);
}

NeighbourFinderTests::~NeighbourFinderTests() {
    //error computation test
    sequences.clear();
    delete access;
    delete errComp;
    
    //NeighbourFinder tests
    delete loader;
    sequenceArray.clear();
    delete access2;
    delete hashComp;
    delete finder;
}

void NeighbourFinderTests::setUp() {
    neighbTable = NeighbTablePtr(new NeighbourTable_Impl<NeighOccurrence>);
    neighbTable->setSize(access2->size());
}

void NeighbourFinderTests::tearDown() {
    
}

void NeighbourFinderTests::errComputationTest() {
    errComp->setFirstSeq(0);
    errComp->setSecondSeq(2);
    
    unsigned err = errComp->operator()(0, 4, 66);
    CPPUNIT_ASSERT_EQUAL(4u, err);
    
    errComp->setFirstSeq(2);
    errComp->setSecondSeq(0);
    err = errComp->operator()(4, 0, 66);
    CPPUNIT_ASSERT_EQUAL(4u, err);
}

//testing that neighbours are computed only within range
void NeighbourFinderTests::rangeNeighbourTest() {
    finder->operator()(neighbTable, 2000, 15000,0, 8);
    
    for (auto i = 0; i < 2000; i++) {
        CPPUNIT_ASSERT_EQUAL(0u, neighbTable->getNeighbCount(i));
    }
    
    for (auto i = 15000; i < 20000; i++) {
        CPPUNIT_ASSERT_EQUAL(0u, neighbTable->getNeighbCount(i));
    }
    
    //every sequence in range must have at least itself as neighbour
    //complements shouldn't have anything
    for (auto i = 2000; i < 15000; i++) {
        auto interp = access2->operator[](i);
        if (interp->isCompl()) {
            CPPUNIT_ASSERT(neighbTable->getNeighbCount(i) == 0);
        } else {
            CPPUNIT_ASSERT(neighbTable->getNeighbCount(i) > 0);
        }
        delete interp;
    }
}

void NeighbourFinderTests::uniqueNeighborsTest() {
    finder->operator()(neighbTable, 2000, 15000,0, 8);
    
    //all neighbours must be unique (have different id)
    for (int i=2000; i<15000; i++) {
        for (int a=0; a<neighbTable->getNeighbCount(i); a++) {
            for (int b=a+1; b<neighbTable->getNeighbCount(i); b++) {
                CPPUNIT_ASSERT(get<0>(neighbTable->getNeighb(i, a)) != get<0>(neighbTable->getNeighb(i, b)));
            }
        }
    }
}

void NeighbourFinderTests::areNeighboursTest() {
    finder->operator()(neighbTable, 2000, 15000,0, 8);
    
    //test if found neighbours are really neighbours
    for (int id=2000; id<15000; id++) {
        for (int index=0; index<neighbTable->getNeighbCount(id); index++) {
            Interpreter* seq = access2->operator[](id);
            Neighb neighbInfo = neighbTable->getNeighb(id, index);
            Interpreter* neighbour = access2->operator[](get<0>(neighbInfo));
            
            //get overlap size
            unsigned overlap;
            if (get<1>(neighbInfo) >= 0) { //offset
                overlap = seq->getLength() - static_cast<unsigned>(get<1>(neighbInfo));
            } else {
                overlap = neighbour->getLength() - static_cast<unsigned>(-get<1>(neighbInfo));
            }
            
            //overlap must be 38 or higher
            CPPUNIT_ASSERT(overlap >= 38);
            
            ErrorComputation errComp(*access2);
            errComp.setFirstSeq(id);
            errComp.setSecondSeq(get<0>(neighbInfo));
            
            unsigned s1, s2;
            if (get<1>(neighbInfo) >= 0) {
                s1 = static_cast<unsigned>(get<1>(neighbInfo)); s2 = 0;
            } else {
                s1 = 0; s2 = static_cast<unsigned>(-get<1>(neighbInfo));
            }
            
            unsigned numOfErr = errComp(s1, s2, overlap);
            
            //test for error tolerance
            CPPUNIT_ASSERT(static_cast<double>(numOfErr)/overlap <= 0.15);
            
            //test if error counts corresponds
            CPPUNIT_ASSERT_EQUAL(numOfErr, get<2>(neighbInfo));
            
            delete seq;
            delete neighbour;
        }
    }
}

//if sequence has another sequence as a neighbour, it must apply vice versa
void NeighbourFinderTests::mutualNeighbourhoodTest() {
    finder->operator()(neighbTable, 0, access2->size(),0, 8);
    
    for (int id = 0; id < neighbTable->getSize(); id++) {
        
        for (int index=0; index<neighbTable->getNeighbCount(id); index++) {
            Neighb neighbInfo = neighbTable->getNeighb(id, index);
            ULL neighbId = get<0>(neighbInfo);
            
            if (access2->isComplement(neighbId)) continue; //excluding complements
            
            bool found = false;
            for (int i=0; i<neighbTable->getNeighbCount(neighbId); i++) {
                if (get<0>(neighbTable->getNeighb(neighbId, i)) == id) found = true;
            }
            
            CPPUNIT_ASSERT_EQUAL(true, found);
        }
    }
}

//result must be independent from order in hash table
//order in hash table is different every run (because of multithreading)
void NeighbourFinderTests::sameResult() {
    //first version
    finder->operator()(neighbTable, 0, access2->size(),0, 8);
    
    //second version
    HashPtr hashTableSecond = HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>);
    hashTableSecond->setSize(65536);
    hashComp->operator()(hashTableSecond, 8); //same data different table
    //hashTableSecond->sortTable();
    
    NeighbTablePtr neighbTableSecond = NeighbTablePtr(new NeighbourTable_Impl<NeighOccurrence>);
    neighbTableSecond->setSize(access2->size());
    NeighbourFinder finderSecond(hashTableSecond, *access2,38, 0.15);
    finderSecond(neighbTableSecond, 0, access2->size(),0, 8);
    
    
    //test number of neighbours
    string message("Index: ");
    for (int i=0; i < access2->size(); i++) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(message + to_string(i),
            neighbTable->getNeighbCount(i), neighbTableSecond->getNeighbCount(i));
    }
}

//test if found overlap has best quality from all overlaps
void NeighbourFinderTests::isBestTest() {
    //- test if neighbourhood is the best one (test sequences and their neighbours for different overlap and check if the one computed by algorithm is the best one (lowest numOfErr/překryv)
    
    
}

