//
//  PoissonTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__PoissonTests_h__
#define __EchoErrorCorrection__PoissonTests_h__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/Poisson.h"

class PoissonTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PoissonTests);
    CPPUNIT_TEST(poissonLengthTest);
    CPPUNIT_TEST(poissonValuesTest);
    CPPUNIT_TEST(distrEstimationTest);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();
    
    //poisson distribution
    void poissonLengthTest();
    void poissonValuesTest();
    
    //total variation
    void distrEstimationTest();
    
};

#endif