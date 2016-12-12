//
//  HashComputationTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 08/02/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__HashComputationTests__
#define __EchoErrorCorrection__HashComputationTests__

#include <vector>
#include <memory>
#include <string>
#include <set>
using namespace std;

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/HashComputation.h"
#include "../Echo/SequenceFileLoader.h"
#include "../Echo/Sequence.h"
#include "../Echo/RandomisedAccess.h"

class HashComputationTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HashComputationTests);
    CPPUNIT_TEST(uniqueIdTest);
    CPPUNIT_TEST(validPositionTest);
    CPPUNIT_TEST(allIdFromTableTest);
    CPPUNIT_TEST(allIdInTableTest);
    CPPUNIT_TEST(seqUnderAllTest);
    CPPUNIT_TEST(occurrenceCorrectnessTest);
    CPPUNIT_TEST_SUITE_END();
    
    
    SequenceFileLoader* loader;
    vector<Sequence> seqArray;
    RandomisedAccess* access;
    HashComputation* computation;
    HashPtr hashTable1, hashTable3, hashTable4;
    vector<HashPtr> tables;
    
    string path; //path to TestSuite files
public:
    HashComputationTests();
    
    void setUp();
    void tearDown();
    
    void uniqueIdTest();
    void validPositionTest();
    void allIdFromTableTest();
    void allIdInTableTest();
    void seqUnderAllTest();
    void occurrenceCorrectnessTest();
    //possible tests:
        //if sequence has 2 or more same hashes - one closer to beginning has to be stored
        //tests for file, in with sequence length varies
    
private:
    unsigned hashFunction(const char *startPos, const char *endPos) { //same hash function that HashComputation uses
        unsigned hash = 0;
        for (const char* it = startPos; it != endPos; it++) {
            if(*it < 4) hash = 65599 * hash + *it;
        }
        
        return hash ^ (hash >> 16);
    }
};

#endif /* defined(__EchoErrorCorrection__HashComputationTests__) */
