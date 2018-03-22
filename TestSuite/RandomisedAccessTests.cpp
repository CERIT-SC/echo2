//
//  RandomisedAccessTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 25/11/13.
//

#include "RandomisedAccessTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(RandomisedAccessTests);
#include <iostream>
void RandomisedAccessTests::setUp() {
    sourceArray = vector<Sequence>{Sequence("A"), Sequence("AA"), Sequence("AAA"),
        Sequence("AAAA"), Sequence("AAAAA"), Sequence("AAAAAA"),
        Sequence("AAAAAAA"), Sequence("AAAAAAAA"), Sequence("AAAAAAAAA"),
        Sequence("AAAAAAAAAA")};
    
    access = new RandomisedAccess(sourceArray);
    seqAcc = new RandomisedAccess(sourceArray, false);
}

void RandomisedAccessTests::tearDown() {
    delete access;
    delete seqAcc;
}



void RandomisedAccessTests::emptyAccessTest() {
    vector<Sequence> array;
    RandomisedAccess access(array);
    
    CPPUNIT_ASSERT_EQUAL(0ull, access.size());
    //access[0];
}

void RandomisedAccessTests::sizeTest() {
    CPPUNIT_ASSERT_EQUAL(20ull, access->size());
}

void RandomisedAccessTests::countInterpretedAsSeq() {
    //counts how many sequences are interpreted as sequence
    int count = 0;
    for (int i=0; i<20; i++) {
        Interpreter* it = access->operator[](i);
        if (it->isCompl()) count++;
        delete it;
    }
    
    CPPUNIT_ASSERT_EQUAL(10, count);
}

void RandomisedAccessTests::countInterpretedAsCompl() {
    //counts how many sequences are interpreted as complement
    int count = 0;
    for (int i=0; i<20; i++) {
        Interpreter* it = access->operator[](i);
        if (!it->isCompl()) count++;
        delete it;
    }
    
    CPPUNIT_ASSERT_EQUAL(10, count);
}

void RandomisedAccessTests::isEverySeqThereTest() {
    int count = 0;
    for (int i=0; i<20; i++) {
        Interpreter* it = access->operator[](i);
        if (!it->isCompl()) count += it->getLength();
        delete it;
    }
    
    CPPUNIT_ASSERT_EQUAL(55, count);
}

void RandomisedAccessTests::isEveryComplThereTest() {
    int count = 0;
    for (int i=0; i<20; i++) {
        Interpreter* it = access->operator[](i);
        if (it->isCompl()) count += it->getLength();
        delete it;
    }
    
    CPPUNIT_ASSERT_EQUAL(55, count);
}

void RandomisedAccessTests::seqAccessTest() {
    for (unsigned i=0; i < seqAcc->size(); i+=2) {
        CPPUNIT_ASSERT(seqAcc->isComplement(i) == false);
    }
    
    for (unsigned i=1; i < seqAcc->size(); i+=2) {
        CPPUNIT_ASSERT(seqAcc->isComplement(i) == true);
    }
    
    for (unsigned i=0; i < seqAcc->size(); i+=2) {
        Interpreter * interp = seqAcc->operator[](i);
        CPPUNIT_ASSERT_EQUAL(i/2 + 1, interp->getLength());
        delete interp;
    }
}

void RandomisedAccessTests::getSeqLengthTest() {
    for (unsigned i=0; i< seqAcc->size(); i+=2) {
        CPPUNIT_ASSERT_EQUAL(i/2+1, seqAcc->getSeqLength(i));
    }
    
    for (unsigned i=1; i< seqAcc->size(); i+=2) {
        CPPUNIT_ASSERT_EQUAL(i/2+1, seqAcc->getSeqLength(i));
    }
}

void RandomisedAccessTests::getOrigIndexTest() {
    
    for (ULL randIndex = 0; randIndex < access->size(); randIndex++) {
        Interpreter* i = access->operator[](randIndex);
        
        CPPUNIT_ASSERT_EQUAL((ULL)i->getLength() - 1, access->getOrigIndex(randIndex));
        
    }
}
