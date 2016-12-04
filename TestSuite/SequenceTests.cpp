//
//  SequenceTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 24/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "SequenceTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SequenceTests);

void SequenceTests::setUp() {
    rawSeq = "NAAAACAAATTATTTATATTTATTTTTAATTTTAATTTTTTTTATTTTAATTTTTCCGGGCTTGCGGGCCNNCCGTGGCCCATTCGGGGTAACCTATATTT";
    rawSeq2 = "TATTTGGGTACATGTGTATGAATCGTCTATATTTGAGGANZZZCCAAAGTTAAAATCAAACAAAACACACACACACATAAGATTTTATTTTATTTTTATAATGTT";
    rawLen = static_cast<unsigned>(rawSeq.length());
    rawLen2 = static_cast<unsigned>(rawSeq2.length());
    
    seq = new Sequence(rawSeq);
    seq2 = new Sequence(rawSeq2);
}

void SequenceTests::tearDown() {
    delete seq;
    delete seq2;
}



void SequenceTests::emptySequenceTest() {
    Sequence seq;
    
    CPPUNIT_ASSERT_EQUAL(0u, seq.getLength());
    CPPUNIT_ASSERT_EQUAL(string(""), seq.getAsString());
    
    auto seqInterp = seq.interpretAsSeq();
    auto complInterp = seq.interpretAsCompl();
    
    //getLength
    CPPUNIT_ASSERT_EQUAL(0u, seqInterp->getLength());
    CPPUNIT_ASSERT_EQUAL(0u, complInterp->getLength());
    
    //isCompl
    CPPUNIT_ASSERT_EQUAL(false, seqInterp->isCompl());
    CPPUNIT_ASSERT_EQUAL(true, complInterp->isCompl());
}

void SequenceTests::fullSeq() {
    
    //get length
    CPPUNIT_ASSERT_EQUAL(rawLen, seq->getLength());
    CPPUNIT_ASSERT_EQUAL(rawLen2, seq2->getLength());
    
    //get as string
    CPPUNIT_ASSERT_EQUAL(rawSeq, seq->getAsString());
    CPPUNIT_ASSERT_EQUAL(rawSeq2, seq2->getAsString());
}

void SequenceTests::copyConstrSeqTest() {
    Sequence *seq = new Sequence(rawSeq);
    
    Sequence seq2(*seq);
    delete seq;
    
    CPPUNIT_ASSERT_EQUAL(rawLen, seq2.getLength());
    CPPUNIT_ASSERT_EQUAL(rawSeq, seq2.getAsString());
}

void SequenceTests::copyOperatorSeqTest() {
    Sequence *seq = new Sequence(rawSeq);
    Sequence seq2;
    
    seq2 = *seq;
    delete seq;
    
    CPPUNIT_ASSERT_EQUAL(rawLen, seq2.getLength());
    CPPUNIT_ASSERT_EQUAL(rawSeq, seq2.getAsString());
}

void SequenceTests::incorrectBaseTest() {
    string incSeq = "ACGUCCT", incSeq2 = "TATTTGGGTAcATGTGTA";
    
    CPPUNIT_ASSERT_THROW(Sequence s(incSeq);, SequenceException);
    try { Sequence s(incSeq); } catch (SequenceException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    CPPUNIT_ASSERT_THROW(Sequence s(incSeq2);, SequenceException);
    try { Sequence s(incSeq2); } catch (SequenceException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
}


//INTERPRETER TESTS
void SequenceTests::interpreterGetLengthTest() {
    
    auto seqI1 = seq->interpretAsSeq();
    auto complI1 = seq->interpretAsCompl();
    auto seqI2 = seq2->interpretAsSeq();
    auto complI2 = seq2->interpretAsSeq();
    
    //getLength()
    CPPUNIT_ASSERT_EQUAL(rawLen, seqI1->getLength());
    CPPUNIT_ASSERT_EQUAL(rawLen, complI1->getLength());
    CPPUNIT_ASSERT_EQUAL(rawLen2, seqI2->getLength());
    CPPUNIT_ASSERT_EQUAL(rawLen2, complI2->getLength());
}

void SequenceTests::seqInterprAtTest1() {
    auto seqIt = seq->interpretAsSeq();
    string message;
    
    for (unsigned i=0; i<seqIt->getLength(); i++) {
        message = string("Index: ") + to_string(i);
        
        if (rawSeq[i] == 'A') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(0), seqIt->at(i));
        if (rawSeq[i] == 'C') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(1), seqIt->at(i));
        if (rawSeq[i] == 'G') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(2), seqIt->at(i));
        if (rawSeq[i] == 'T') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(3), seqIt->at(i));
        
        if (rawSeq[i] == 'N') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(4), seqIt->at(i));
        if (rawSeq[i] == 'Z') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(5), seqIt->at(i));
    }
    
}

void SequenceTests::seqInterprAtTest2() {
    auto seqIt = seq2->interpretAsSeq();
    string message;
    
    for (unsigned i=0; i<seqIt->getLength(); i++) {
        message = string("Index: ") + to_string(i);
        
        if (rawSeq2[i] == 'A') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(0), seqIt->at(i));
        if (rawSeq2[i] == 'C') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(1), seqIt->at(i));
        if (rawSeq2[i] == 'G') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(2), seqIt->at(i));
        if (rawSeq2[i] == 'T') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(3), seqIt->at(i));
        
        if (rawSeq2[i] == 'N') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(4), seqIt->at(i));
        if (rawSeq2[i] == 'Z') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(5), seqIt->at(i));
    }
}

void SequenceTests::complInterprAtTest1() {
    auto complIt = seq->interpretAsCompl();
    string message;
    unsigned len = complIt->getLength();
    
    for (unsigned i=0; i<len; i++) {
        message = string("Index: ") + to_string(i);
        
        if (rawSeq[i] == 'A') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(3), complIt->at(len-i-1));
        if (rawSeq[i] == 'C') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(2), complIt->at(len-i-1));
        if (rawSeq[i] == 'G') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(1), complIt->at(len-i-1));
        if (rawSeq[i] == 'T') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(0), complIt->at(len-i-1));
        
        if (rawSeq[i] == 'N') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(4), complIt->at(len-i-1));
        if (rawSeq[i] == 'Z') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(5), complIt->at(len-i-1));
    }
    
}

void SequenceTests::complInterprAtTest2() {
    auto complIt = seq2->interpretAsCompl();
    string message;
    unsigned len = complIt->getLength();
    
    for (unsigned i=0; i<len; i++) {
        message = string("Index: ") + to_string(i);
        
        if (rawSeq2[i] == 'A') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(3), complIt->at(len-i-1));
        if (rawSeq2[i] == 'C') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(2), complIt->at(len-i-1));
        if (rawSeq2[i] == 'G') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(1), complIt->at(len-i-1));
        if (rawSeq2[i] == 'T') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(0), complIt->at(len-i-1));
        
        if (rawSeq2[i] == 'N') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(4), complIt->at(len-i-1));
        if (rawSeq2[i] == 'Z') CPPUNIT_ASSERT_EQUAL_MESSAGE(message,static_cast<short>(5), complIt->at(len-i-1));
    }
}
