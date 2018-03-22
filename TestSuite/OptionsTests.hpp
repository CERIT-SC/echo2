//
//  OptionsTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 21/11/13.
//

#ifndef __EchoErrorCorrection__OptionsTests__
#define __EchoErrorCorrection__OptionsTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/Options.hpp"

class OptionsTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(OptionsTest);
    CPPUNIT_TEST(emptyOptionsTest);
    CPPUNIT_TEST(inputFileNamesTest);
    CPPUNIT_TEST(outputFileNamesTest);
    CPPUNIT_TEST(logFileNameTest);
    CPPUNIT_TEST(numberOfCoresTest);
    CPPUNIT_TEST(kmerLengthTest);
    CPPUNIT_TEST(minErrTolerTest);
    CPPUNIT_TEST(maxErrTolerTest);
    CPPUNIT_TEST(minOverlapTest);
    CPPUNIT_TEST(maxOverlapTest);
    CPPUNIT_TEST(heterRateTest);
    CPPUNIT_TEST(paramSampleSizeTest);
    CPPUNIT_TEST(hashTableSizeTest);
    CPPUNIT_TEST(stateAndErrMessageTest);
    CPPUNIT_TEST(allInputsTest);
    CPPUNIT_TEST(unknownFlagTest);
    CPPUNIT_TEST(endFlagOptionTest);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp();
    void tearDown();
    
    //tests
    void emptyOptionsTest();
    void inputFileNamesTest();
    void outputFileNamesTest();
    void logFileNameTest();
    void numberOfCoresTest();
    void kmerLengthTest();
    void minErrTolerTest();
    void maxErrTolerTest();
    void minOverlapTest();
    void maxOverlapTest();
    void heterRateTest();
    void paramSampleSizeTest();
    void hashTableSizeTest();
    void stateAndErrMessageTest();
    void allInputsTest();
    void unknownFlagTest();
    void endFlagOptionTest();
    
};

#endif /* defined(__EchoErrorCorrection__OptionsTests__) */
