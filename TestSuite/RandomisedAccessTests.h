//
//  RandomisedAccessTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 25/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__RandomisedAccessTests__
#define __EchoErrorCorrection__RandomisedAccessTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/RandomisedAccess.h"

#include <vector>
using namespace std;

class RandomisedAccessTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(RandomisedAccessTests);
    CPPUNIT_TEST(emptyAccessTest);
    CPPUNIT_TEST(sizeTest);
    CPPUNIT_TEST(countInterpretedAsSeq);
    CPPUNIT_TEST(countInterpretedAsCompl);
    CPPUNIT_TEST(isEverySeqThereTest);
    CPPUNIT_TEST(isEveryComplThereTest);
    CPPUNIT_TEST(seqAccessTest);
    CPPUNIT_TEST(getSeqLengthTest);
    CPPUNIT_TEST(getOrigIndexTest);
    CPPUNIT_TEST_SUITE_END();
    
    vector<Sequence> sourceArray;
    RandomisedAccess *access, *seqAcc;
public:
    void setUp();
    void tearDown();
    
    void emptyAccessTest();
    void sizeTest();
    void countInterpretedAsSeq();
    void countInterpretedAsCompl();
    void isEverySeqThereTest();
    void isEveryComplThereTest();
    void seqAccessTest();
    void getSeqLengthTest();
    void getOrigIndexTest();
    
    //test that every sequence/complement is there only once
};

#endif /* defined(__EchoErrorCorrection__RandomisedAccessTests__) */
