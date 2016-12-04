//
//  NeighbourTableTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 11/02/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__NeighbourTableTests__
#define __EchoErrorCorrection__NeighbourTableTests__

#include <tuple>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/NeighbourTable.h"
#include "../EchoErrorCorrection/NeighbourOccurrence.h"

class NeighbourTableTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NeighbourTableTests);
    CPPUNIT_TEST(basicTableTest);
    CPPUNIT_TEST(addElementTest);
    CPPUNIT_TEST(addMultipleElementsTest);
    CPPUNIT_TEST(exchangeTest);
    CPPUNIT_TEST(shrinkTest);
    CPPUNIT_TEST_SUITE_END();
    
    NeighbourTable *table, *table2;
public:
    void setUp();
    void tearDown();
    
    void basicTableTest();
    void addElementTest();
    void addMultipleElementsTest();
    void exchangeTest();
    void shrinkTest();
};

#endif /* defined(__EchoErrorCorrection__NeighbourTableTests__) */
