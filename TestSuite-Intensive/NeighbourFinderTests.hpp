//
//  NeighbourFinderTests.hpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 12/02/14.
//

#ifndef __EchoErrorCorrection__NeighbourFinderTests__
#define __EchoErrorCorrection__NeighbourFinderTests__

//Tests to do:
//- no more than 1000 neighbours must exist for each sequence (create input of 2000 same sequences)
//- test that there are all sequences (no one is missing)

#include <vector>
#include <string>
using namespace std;

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/NeighbourTable.hpp"
#include "../Echo/NeighbourFinder.hpp"
#include "../Echo/RandomisedAccess.hpp"
#include "../Echo/Sequence.hpp"
#include "../Echo/SequenceFileLoader.hpp"
#include "../Echo/HashComputation.hpp"
#include "../Echo/HashKmerOccTable.hpp"
#include "../Echo/NeighbourOccurrence.hpp"

class NeighbourFinderTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NeighbourFinderTests);
    CPPUNIT_TEST(errComputationTest);
    CPPUNIT_TEST(rangeNeighbourTest);
    CPPUNIT_TEST(uniqueNeighborsTest);
    CPPUNIT_TEST(areNeighboursTest);
    CPPUNIT_TEST(mutualNeighbourhoodTest);
    CPPUNIT_TEST(sameResult);
    CPPUNIT_TEST(isBestTest);
    CPPUNIT_TEST_SUITE_END();
    
    //error computation test
    vector<Sequence> sequences;
    RandomisedAccess *access;
    ErrorComputation *errComp;
    
    //NeighbourFinder tests
    SequenceFileLoader *loader;
    vector<Sequence> sequenceArray;
    RandomisedAccess *access2;
    HashComputation *hashComp;
    HashPtr hashTable;
    NeighbourFinder *finder;
    NeighbTablePtr neighbTable;
    
    string path; //path to TestSuite files
public:
    NeighbourFinderTests();
    ~NeighbourFinderTests();
    void setUp();
    void tearDown();
    
    void errComputationTest();
    
    void rangeNeighbourTest();
    void uniqueNeighborsTest();
    void areNeighboursTest();
    void mutualNeighbourhoodTest();
    void sameResult();
    void isBestTest();
};

#endif /* defined(__EchoErrorCorrection__NeighbourFinderTests__) */
