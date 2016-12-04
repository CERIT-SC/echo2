//
//  SequenceFileLoaderTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 28/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__SequenceFileLoaderTests__
#define __EchoErrorCorrection__SequenceFileLoaderTests__

#include <fstream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../EchoErrorCorrection/SequenceFileLoader.h"

class SequenceFileLoaderTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SequenceFileLoaderTests);
    CPPUNIT_TEST(areAllFilesForTestPrepared);
    CPPUNIT_TEST(emptyLoaderTest);
    CPPUNIT_TEST(loadStandardFileTest);
    CPPUNIT_TEST(loadFastqFileTest);
    CPPUNIT_TEST(loadNonExistingFileTest);
    CPPUNIT_TEST(loadCorruptedFileTest);
    CPPUNIT_TEST_SUITE_END();
    
    
    vector<Sequence> seqArray;
    SequenceFileLoader loader;
    typedef vector<Sequence>::size_type SIZE;
    
public:
    void setUp();
    void tearDown();
    
    void areAllFilesForTestPrepared();
    void emptyLoaderTest();
    void loadStandardFileTest();
    void loadFastqFileTest();
    void loadNonExistingFileTest();
    void loadCorruptedFileTest();
};

//pokud se během načítání nastaví file.fail, tak vyhoď výjimku

#endif /* defined(__EchoErrorCorrection__SequenceFileLoaderTests__) */
