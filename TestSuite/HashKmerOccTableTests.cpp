//
//  HashKmerOccTableTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 30/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "HashKmerOccTableTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(HashKmerOccTableTests);

void HashKmerOccTableTests::setUp() {
    table = new HashKmerOccTable_Impl<KmerOcc_UnsChar>;
    table2 = new HashKmerOccTable_Impl<KmerOcc_UnsChar>;
    
    table2->setSize(41);
    table2->add(9, 1, 11);
    table2->add(29, 2, 22);
    table2->add(9, 3, 33);
    table2->add(29, 4, 44);
    table2->add(9, 5, 55);
}

void HashKmerOccTableTests::tearDown() {
    delete table;
    delete table2;
}



void HashKmerOccTableTests::basicTableTest() {
    CPPUNIT_ASSERT_EQUAL(0ull, table->getSize());
    table->setSize(20);
    CPPUNIT_ASSERT_EQUAL(20ull, table->getSize());
    
    for(int i=0; i<20; i++) {
        CPPUNIT_ASSERT_EQUAL(0u, table->getOccurrCount(i));
    }
    
    //index overflow tested using assert
}

void HashKmerOccTableTests::addElementTest() {
    table->setSize(30);
    table->add(2, 7, 20);
    
    CPPUNIT_ASSERT_EQUAL(1u, table->getOccurrCount(2));
    
    auto occ = table->getOccurr(2, 0);
    CPPUNIT_ASSERT_EQUAL(7ull, occ.first);
    CPPUNIT_ASSERT_EQUAL(20u, occ.second);
}

void HashKmerOccTableTests::addMultipleElementsTest() {
    
    CPPUNIT_ASSERT_EQUAL(3u, table2->getOccurrCount(9));
    CPPUNIT_ASSERT_EQUAL(2u, table2->getOccurrCount(29));
    
    auto occ = table2->getOccurr(9, 0);
    CPPUNIT_ASSERT_EQUAL(1ull, occ.first);
    occ = table2->getOccurr(29, 1);
    CPPUNIT_ASSERT_EQUAL(44u, occ.second);
}

void HashKmerOccTableTests::exchangeTest() {
    
    //exchange with value that is already there
    table2->exchange(9, 3, 33);
    auto occ = table2->getOccurr(9, 1);
    CPPUNIT_ASSERT_EQUAL(33u, occ.second);
    
    //exchange with different value
    table2->exchange(9, 3, 65);
    occ = table2->getOccurr(9, 1);
    CPPUNIT_ASSERT_EQUAL(65u, occ.second);
    
    //others should not be altered
    occ = table2->getOccurr(9, 0);
    CPPUNIT_ASSERT_EQUAL(11u, occ.second);
    occ = table2->getOccurr(9, 2);
    CPPUNIT_ASSERT_EQUAL(5ull, occ.first);
}

void HashKmerOccTableTests::eraseTest() {
    table2->clear();
    CPPUNIT_ASSERT_EQUAL(0ull, table2->getSize());
}

void HashKmerOccTableTests::shrinkTest() {
    table2->shrink_to_fit(1);
    
    CPPUNIT_ASSERT_EQUAL(2u, table2->getOccurrCount(29));
    CPPUNIT_ASSERT_EQUAL(3u, table2->getOccurrCount(9));
    
    
    tearDown();
    setUp();
    
    table2->shrink_to_fit(8);
    
    CPPUNIT_ASSERT_EQUAL(2u, table2->getOccurrCount(29));
    CPPUNIT_ASSERT_EQUAL(3u, table2->getOccurrCount(9));
}