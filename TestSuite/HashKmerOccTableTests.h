//
//  HashKmerOccTableTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 30/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__HashKmerOccTableTests__
#define __EchoErrorCorrection__HashKmerOccTableTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/HashKmerOccTable.h"

class HashKmerOccTableTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HashKmerOccTableTests);
    CPPUNIT_TEST(basicTableTest);
    CPPUNIT_TEST(addElementTest);
    CPPUNIT_TEST(addMultipleElementsTest);
    CPPUNIT_TEST(exchangeTest);
    CPPUNIT_TEST(eraseTest);
    CPPUNIT_TEST(shrinkTest);
    CPPUNIT_TEST_SUITE_END();
    
    
    HashKmerOccTable * table, *table2;
public:
    void setUp();
    void tearDown();
    
    void basicTableTest();
    void addElementTest();
    void addMultipleElementsTest();
    void exchangeTest();
    void eraseTest();
    void shrinkTest();
};

#endif /* defined(__EchoErrorCorrection__HashKmerOccTableTests__) */
