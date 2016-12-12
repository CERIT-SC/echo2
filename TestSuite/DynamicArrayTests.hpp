//
//  DynamicArrayTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 30/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__DynamicArrayTests__
#define __EchoErrorCorrection__DynamicArrayTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/DynamicArray.h"

class DynamicArrayTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(DynamicArrayTests);
    CPPUNIT_TEST(emptyArrayTest);
    CPPUNIT_TEST(addElementTest);
    CPPUNIT_TEST(addMultipleElementsTest);
    CPPUNIT_TEST(copyConstructorTest);
    CPPUNIT_TEST(shrinkToFitTest);
    CPPUNIT_TEST(reserveTest);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();
    
    void emptyArrayTest();
    void addElementTest();
    void addMultipleElementsTest();
    void copyConstructorTest();
    //void invalidAccessTest(); //invalid access on operators [] is not tested because is not implemented
    void shrinkToFitTest();
    void reserveTest();
};

#endif /* defined(__EchoErrorCorrection__DynamicArrayTests__) */
