//
//  KmerOccurrenceTests.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 02/12/13.
//

#ifndef __EchoErrorCorrection__KmerOccurrenceTests__
#define __EchoErrorCorrection__KmerOccurrenceTests__

#include <limits>
using namespace std;

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../Echo/KmerOccurrence.hpp"

#define UNS_CHAR_MAX static_cast<unsigned>(numeric_limits<unsigned char>::max())
#define UNS_SHORT_MAX static_cast<unsigned>(numeric_limits<unsigned short>::max())

#define UNS_MAX static_cast<ULL>(numeric_limits<unsigned>::max())
#define ULL_MAX numeric_limits<ULL>::max()

class KmerOccurrenceTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(KmerOccurrenceTests);
    CPPUNIT_TEST(unsChar);
    CPPUNIT_TEST(unsShort);
    CPPUNIT_TEST(unsUns);
    CPPUNIT_TEST(ullChar);
    CPPUNIT_TEST(ullShort);
    CPPUNIT_TEST(ullUns);
    CPPUNIT_TEST_SUITE_END();
    
    
public:
    void unsChar();
    void unsShort();
    void unsUns();
    void ullChar();
    void ullShort();
    void ullUns();
};

#endif /* defined(__EchoErrorCorrection__KmerOccurrenceTests__) */
