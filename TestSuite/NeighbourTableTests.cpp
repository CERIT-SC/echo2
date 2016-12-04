//
//  NeighbourTableTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 11/02/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#include "NeighbourTableTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(NeighbourTableTests);

void NeighbourTableTests::setUp() {
    table = new NeighbourTable_Impl<NeighOccurrence>;
    table2 = new NeighbourTable_Impl<NeighOccurrence>;
    
    table2->setSize(41);
    table2->add(9, 1, 11, 111);
    table2->add(29, 2, 22, 222);
    table2->add(9, 3, 33, 333);
    table2->add(29, 4, 44, 444);
    table2->add(9, 5, 55, 555);
}

void NeighbourTableTests::tearDown() {
    delete table;
    delete table2;
}



void NeighbourTableTests::basicTableTest() {
    CPPUNIT_ASSERT_EQUAL(0ull, table->getSize());
    table->setSize(20);
    CPPUNIT_ASSERT_EQUAL(20ull, table->getSize());
    
    for(int i=0; i<20; i++) {
        CPPUNIT_ASSERT_EQUAL(0u, table->getNeighbCount(i));
    }
    
    //index overflow tested using assert
}

void NeighbourTableTests::addElementTest() {
    table->setSize(30);
    table->add(2, 7, 20, 30);
    
    CPPUNIT_ASSERT_EQUAL(1u, table->getNeighbCount(2));
    
    Neighb occ = table->getNeighb(2, 0);
    CPPUNIT_ASSERT_EQUAL(7ull, get<0>(occ));
    CPPUNIT_ASSERT_EQUAL(20ll, get<1>(occ));
    CPPUNIT_ASSERT_EQUAL(30u, get<2>(occ));
}

void NeighbourTableTests::addMultipleElementsTest() {
    
    CPPUNIT_ASSERT_EQUAL(3u, table2->getNeighbCount(9));
    CPPUNIT_ASSERT_EQUAL(2u, table2->getNeighbCount(29));
    
    auto occ = table2->getNeighb(9, 0);
    CPPUNIT_ASSERT_EQUAL(1ull, get<0>(occ));
    CPPUNIT_ASSERT_EQUAL(11ll, get<1>(occ));
    CPPUNIT_ASSERT_EQUAL(111u, get<2>(occ));
    
    occ = table2->getNeighb(29, 1);
    CPPUNIT_ASSERT_EQUAL(4ull, get<0>(occ));
    CPPUNIT_ASSERT_EQUAL(44ll, get<1>(occ));
    CPPUNIT_ASSERT_EQUAL(444u, get<2>(occ));
}

void NeighbourTableTests::exchangeTest() {
    
    //exchange with value that is already there
    table2->exchange(9, 3, 33, 333);
    auto occ = table2->getNeighb(9, 1);
    CPPUNIT_ASSERT_EQUAL(33ll, get<1>(occ));
    
    //exchange with different value
    table2->exchange(9, 3, 65, 66);
    occ = table2->getNeighb(9, 1);
    CPPUNIT_ASSERT_EQUAL(65ll, get<1>(occ));
    
    //others should not be altered
    occ = table2->getNeighb(9, 0);
    CPPUNIT_ASSERT_EQUAL(11ll, get<1>(occ));
    occ = table2->getNeighb(9, 2);
    CPPUNIT_ASSERT_EQUAL(5ull, get<0>(occ));
    
    //size should stay the same
    CPPUNIT_ASSERT_EQUAL(3u, table2->getNeighbCount(9));
}

void NeighbourTableTests::shrinkTest() {
    table2->shrink_to_fit(1);
    
    CPPUNIT_ASSERT_EQUAL(2u, table2->getNeighbCount(29));
    CPPUNIT_ASSERT_EQUAL(3u, table2->getNeighbCount(9));
    
    
    tearDown();
    setUp();
    
    table2->shrink_to_fit(7);
    
    CPPUNIT_ASSERT_EQUAL(2u, table2->getNeighbCount(29));
    CPPUNIT_ASSERT_EQUAL(3u, table2->getNeighbCount(9));
}