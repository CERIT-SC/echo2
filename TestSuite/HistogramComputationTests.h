//
//  HistogramComputationTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 19/2/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__HistogramComputationTests__
#define __EchoErrorCorrection__HistogramComputationTests__

#include <vector>
using namespace std;

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/HistogramComputation.h"
#include "../EchoErrorCorrection/RandomisedAccess.h"
#include "../EchoErrorCorrection/NeighbourTable.h"
#include "../EchoErrorCorrection/Sequence.h"
#include "../EchoErrorCorrection/NeighbourOccurrence.h"


class HistCompTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HistCompTests);
    CPPUNIT_TEST(basicHistTest);
    CPPUNIT_TEST(stricterOverlapTest);
    CPPUNIT_TEST(stricterErrTolerTest);
    CPPUNIT_TEST(stricterCondTest);
    CPPUNIT_TEST_SUITE_END();
    
    vector<Sequence> sequences;
    NeighbTablePtr table;
    RandomisedAccess * access;
    HistogramComputation * hComp;
public:
    void setUp();
    void tearDown();
    
    void basicHistTest();
    void stricterOverlapTest();
    void stricterErrTolerTest();
    void stricterCondTest();
    
};
#endif /* defined(__EchoErrorCorrection__HistogramComputationTests__) */
