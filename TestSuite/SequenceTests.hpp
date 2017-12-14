//
//  SequenceTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 24/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__SequenceTests__
#define __EchoErrorCorrection__SequenceTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/Sequence.hpp"


#include <string>
#include <stdexcept>
using namespace std;

class SequenceTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SequenceTests);
    CPPUNIT_TEST(emptySequenceTest);
    CPPUNIT_TEST(fullSeq);
    CPPUNIT_TEST(copyConstrSeqTest);
    CPPUNIT_TEST(copyOperatorSeqTest);
    CPPUNIT_TEST(incorrectBaseTest);
    
    CPPUNIT_TEST(interpreterGetLengthTest);
    CPPUNIT_TEST(seqInterprAtTest1);
    CPPUNIT_TEST(seqInterprAtTest2);
    CPPUNIT_TEST(complInterprAtTest1);
    CPPUNIT_TEST(complInterprAtTest2);
    CPPUNIT_TEST_SUITE_END();
    
    
    string rawSeq, rawSeq2;
    unsigned rawLen, rawLen2;
    Sequence *seq, *seq2;
public:
    void setUp();
    void tearDown();
    
    void emptySequenceTest();
    void fullSeq();
    void copyConstrSeqTest();
    void copyOperatorSeqTest();
    void incorrectBaseTest();
    
    void interpreterGetLengthTest();
    void seqInterprAtTest1();
    void seqInterprAtTest2();
    void complInterprAtTest1();
    void complInterprAtTest2();
};

#endif /* defined(__EchoErrorCorrection__SequenceTests__) */
