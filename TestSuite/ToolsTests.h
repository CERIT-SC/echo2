//
//  ToolsTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/03/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__ToolsTests__
#define __EchoErrorCorrection__ToolsTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/Tools.h"

class N44MatrixTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(N44MatrixTests);
    CPPUNIT_TEST(emptyMatrixTest);
    CPPUNIT_TEST(emptyValueTest);
    CPPUNIT_TEST(saveValuesTest);
    CPPUNIT_TEST(copyTest);
    CPPUNIT_TEST(addOperatorTest);
    CPPUNIT_TEST_SUITE_END();
    
    N44Matrix emptyMatrix, *matrix;
public:
    void setUp();
    void tearDown();
    
    void emptyMatrixTest();
    void emptyValueTest();
    void saveValuesTest();
    void copyTest();
    void addOperatorTest();
};


#include "../Echo/RandomisedAccess.h"
#include "../Echo/NeighbourTable.h"
#include "../Echo/HashKmerOccTable.h"
#include "../Echo/Sequence.h"

class GetOverapInfoTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GetOverapInfoTests);
    CPPUNIT_TEST(smallNeighbOverlapTest);
    CPPUNIT_TEST(bigNeighbOverlapTest);
    CPPUNIT_TEST(neighbOffsetTest);
    CPPUNIT_TEST(seqOffsetTest);
    CPPUNIT_TEST_SUITE_END();
    
    vector<Sequence> sequences;
    RandomisedAccess * access;
    Neighb info;
    Occur seqOccur, neighbOccur;
    unsigned seqPos, neighbPos, overalpSize;
public:
    void setUp();
    void tearDown();
    
    void smallNeighbOverlapTest();
    void bigNeighbOverlapTest();
    void neighbOffsetTest();
    void seqOffsetTest();
};

#endif /* defined(__EchoErrorCorrection__ToolsTests__) */
