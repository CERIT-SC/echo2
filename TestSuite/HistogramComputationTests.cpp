//
//  HistogramComputationTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 19/2/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#include "HistogramComputationTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(HistCompTests);

void HistCompTests::setUp() {
    sequences = {
        Sequence("AAAAAAAA"), Sequence("AAAAAAAA"), Sequence("AAAAAAAA"), Sequence("AAAAAAAA"),
        Sequence("AAAAACCC"), Sequence("CCCAAAAA"), Sequence("AAGGGGAA")
    };
    //id: 0, 2, 4, 6,
    //    8, 10, 12
    
    access = new RandomisedAccess(sequences, false);
    table = NeighbTablePtr(new NeighbourTable_Impl<NeighOccurrence>);
    
    table->setSize(14); //seq + compl
    table->add(0, 2, 0, 0);
    table->add(0, 4, 0, 0);
    table->add(0, 6, 0, 0);
    table->add(0, 8, 3, 0);
    table->add(0, 10, -3, 0);
    table->add(0, 12, 0, 4);
    
    hComp = new HistogramComputation(*access, table);
}

void HistCompTests::tearDown() {
    sequences.clear();
    delete access;
}


void HistCompTests::basicHistTest() {
    auto hist = hComp->compute(1, 20, 14);
    auto hist2 = hComp->compute(1, 20, 14);
    
    CPPUNIT_ASSERT(5 < hist.size());
    CPPUNIT_ASSERT_EQUAL(1u, hist[5]);
    
    CPPUNIT_ASSERT(5 < hist2.size());
    CPPUNIT_ASSERT_EQUAL(1u, hist2[5]);
    
    for (int i = 0; i < hist.size(); i++) {
        if (i != 5)
            CPPUNIT_ASSERT_EQUAL(0u, hist[i]);
    }
    
    for (int i = 0; i < hist2.size(); i++) {
        if (i != 5)
            CPPUNIT_ASSERT_EQUAL(0u, hist2[i]);
    }
}

void HistCompTests::stricterOverlapTest() {
    auto hist = hComp->compute(6, 20, 14);
    
    CPPUNIT_ASSERT(4 < hist.size());
    CPPUNIT_ASSERT_EQUAL(1u, hist[4]);
}

void HistCompTests::stricterErrTolerTest() {
    auto hist = hComp->compute(1, 0.1, 14);
    
    CPPUNIT_ASSERT(4 < hist.size());
    CPPUNIT_ASSERT_EQUAL(1u, hist[4]);
}

void HistCompTests::stricterCondTest() {
    auto hist = hComp->compute(6, 0.1, 14);
    
    CPPUNIT_ASSERT(3 < hist.size());
    CPPUNIT_ASSERT_EQUAL(1u, hist[3]);
}
