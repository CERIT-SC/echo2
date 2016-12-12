//
//  KmerOccurrenceTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 02/12/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "KmerOccurrenceTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(KmerOccurrenceTests);

void KmerOccurrenceTests::unsChar() {
    KmerOcc_UnsChar occ(UNS_MAX, 1);
    
    CPPUNIT_ASSERT_EQUAL(UNS_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(1u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_CHAR_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(UNS_CHAR_MAX, occ.getPos());
}

void KmerOccurrenceTests::unsShort() {
    KmerOcc_UnsShort occ(UNS_MAX, 2);
    
    CPPUNIT_ASSERT_EQUAL(UNS_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(2u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_SHORT_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(UNS_SHORT_MAX, occ.getPos());
}

void KmerOccurrenceTests::unsUns() {
    KmerOcc_UnsUns occ(UNS_MAX, 3);
    
    CPPUNIT_ASSERT_EQUAL(UNS_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(3u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(static_cast<unsigned>(UNS_MAX), occ.getPos());
}

void KmerOccurrenceTests::ullChar() {
    KmerOcc_UllChar occ(ULL_MAX, 4);
    
    CPPUNIT_ASSERT_EQUAL(ULL_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(4u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_CHAR_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(UNS_CHAR_MAX, occ.getPos());
}

void KmerOccurrenceTests::ullShort() {
    KmerOcc_UllShort occ(ULL_MAX, 5);
    
    CPPUNIT_ASSERT_EQUAL(ULL_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(5u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_SHORT_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(UNS_SHORT_MAX, occ.getPos());
}

void KmerOccurrenceTests::ullUns() {
    KmerOcc_UllUns occ(ULL_MAX, 6);
    
    CPPUNIT_ASSERT_EQUAL(ULL_MAX, occ.getId());
    CPPUNIT_ASSERT_EQUAL(6u, occ.getPos());
    
    occ.setId(1000);
    occ.setPos(UNS_MAX);
    CPPUNIT_ASSERT_EQUAL(1000ull, occ.getId());
    CPPUNIT_ASSERT_EQUAL(static_cast<unsigned>(UNS_MAX), occ.getPos());
}
