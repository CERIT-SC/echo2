//
//  SequenceFileLoaderTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 28/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "SequenceFileLoaderTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SequenceFileLoaderTests);

void SequenceFileLoaderTests::setUp() {
    seqArray.empty();
}
void SequenceFileLoaderTests::tearDown() {}



void SequenceFileLoaderTests::areAllFilesForTestPrepared() {
    string message = "Test file missing";
    ifstream f1("TestSuiteFiles/normal.txt"), f2("TestSuiteFiles/normal.fastq"),
        f3("TestSuiteFiles/corrupted.txt"), f4("TestSuiteFiles/corrupted2.txt"),
        f5("TestSuiteFiles/corrupted.fastq"), f6("TestSuiteFiles/corrupted2.fastq"),
        f7("TestSuiteFiles/corrupted3.fastq"), f8("TestSuiteFiles/corrupted4.fastq");
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f1.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f2.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f3.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f4.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f5.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f6.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f7.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f8.is_open());
}

void SequenceFileLoaderTests::emptyLoaderTest() {
    CPPUNIT_ASSERT_THROW(loader.load("", seqArray);, LoaderException);
    try { loader.load("", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("cannot_open_file"), string(e.what()));
    }
}

void SequenceFileLoaderTests::loadStandardFileTest() {
    CPPUNIT_ASSERT_NO_THROW(loader.load("TestSuiteFiles/normal.txt", seqArray););
    
    CPPUNIT_ASSERT_EQUAL(static_cast<SIZE>(10000), seqArray.size());
    string s = "GGTACTTTTATTTTCGGAGCTGTGGGCAACATTCAAATACTAAGCGAAGCTCCGGCATGAAGTGCTTATGGACTGT";
    string s2 = "ACATCAAATGGTCAATACAGCACAATGGGCGGCTTCTACAACAACCAAAAACCGGGTCAAGCCTCCTTTAATCCCA";
    CPPUNIT_ASSERT_EQUAL(s, seqArray.front().getAsString());
    CPPUNIT_ASSERT_EQUAL(s2, seqArray.back().getAsString());
    
}

void SequenceFileLoaderTests::loadFastqFileTest() {
    CPPUNIT_ASSERT_NO_THROW(loader.load("TestSuiteFiles/normal.fastq", seqArray););
    
    CPPUNIT_ASSERT_EQUAL(static_cast<SIZE>(1000), seqArray.size());
    string s = "NAAAACAAATTATTTATATTTATTTTTAATTTTAATTTTTTTTATTTTAATTTTTCCGGGCTTGCGGGCCNNCCGTGGCCCATTCGGGGTAA";
    string s2 = "TTTGCTTCCCCCCAAATTGGGGGGATTTGGAGGGGAGGGGAGGGAAGTTTTTTTAAAGTTTATTAAACTGACAAAATTATCCTCAATATATT";
    CPPUNIT_ASSERT_EQUAL(s, seqArray.front().getAsString());
    CPPUNIT_ASSERT_EQUAL(s2, seqArray.back().getAsString());
}

void SequenceFileLoaderTests::loadNonExistingFileTest() {
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/Unknown.txt", seqArray);, LoaderException);
    
    try { loader.load("TestSuiteFiles/Unknown.txt", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("cannot_open_file"), string(e.what()));
    }
}

void SequenceFileLoaderTests::loadCorruptedFileTest() {
    //corrupted.txt     has small g instead of G in the middle of 3rd line
    //corrupted2.txt    has U on 5th line
    //corrupted.fastq   has small a at the beggining of second sequence
    //corrupted2.fastq  has I in the 3rd sequence
    //corrupted3.fastq  surplus newline after 3rd sequence
    //corrupted4.fastq  missing + on 7th line
    
    //small g
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted.txt", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted.txt", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //U
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted2.txt", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted2.txt", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //small a
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted.fastq", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //I
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted2.fastq", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted2.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //surplus newline
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted3.fastq", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted3.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_fastq_signature"), string(e.what()));
    }
    
    //missing +
    CPPUNIT_ASSERT_THROW(loader.load("TestSuiteFiles/corrupted4.fastq", seqArray);, LoaderException);
    try { loader.load("TestSuiteFiles/corrupted4.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_fastq_signature"), string(e.what()));
    }
    
}