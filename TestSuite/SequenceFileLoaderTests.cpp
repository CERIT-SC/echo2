//
//  SequenceFileLoaderTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 28/11/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "SequenceFileLoaderTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(SequenceFileLoaderTests);

SequenceFileLoaderTests::SequenceFileLoaderTests() {
    //create path to testing files
    path = __FILE__;
    size_t end = path.find_last_of("/") + 1;
    path = path.substr(0, end);
    path += "TestSuiteFiles/";
}

void SequenceFileLoaderTests::setUp() {
    seqArray.empty();
}

void SequenceFileLoaderTests::tearDown() {}



void SequenceFileLoaderTests::areAllFilesForTestPrepared() {
    string message = "Test file missing";
    ifstream f1(path + "correct.txt"), f2(path + "correct2.txt"),
        f3(path + "correct.fastq"), f4(path + "corrupted.txt"),
        f5(path + "corrupted2.txt"), f6(path + "corrupted.fastq"),
        f7(path + "corrupted2.fastq"), f8(path + "corrupted3.fastq"),
        f9(path + "corrupted4.fastq");
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f1.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f2.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f3.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f4.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f5.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f6.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f7.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f8.is_open());
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, true, f9.is_open());
}

void SequenceFileLoaderTests::emptyLoaderTest() {
    CPPUNIT_ASSERT_THROW(loader.load("", seqArray);, LoaderException);
    try { loader.load("", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("cannot_open_file"), string(e.what()));
    }
}

void SequenceFileLoaderTests::loadStandardFileTest() {
    CPPUNIT_ASSERT_NO_THROW(loader.load(path + "correct.txt", seqArray););
    
    CPPUNIT_ASSERT_EQUAL(static_cast<SIZE>(123), seqArray.size());
    string first = "GGTACTTTTATTTTCGGAGCTGTGGGCAACATTCAAATACTAAGCGAAGCTCCGGCATGAAGTGCTTATGGACTGT";
    string line53 = "GTCGCATTAGCAATAAGGCTACCATATCTTGTGTGCTCTATAGCAAACAACAATCCATACTGGAGACGGAACGGAG";
    string last = "ATGAAATGTGGGTGAAAAAAGTAAGAAAATCATAGCAGATGGATATAGAACGAGTAACCATTTTAGCCAAGAACAA";
    CPPUNIT_ASSERT_EQUAL(first, seqArray.front().getAsString());
    CPPUNIT_ASSERT_EQUAL(line53, seqArray[52].getAsString());
    CPPUNIT_ASSERT_EQUAL(last, seqArray.back().getAsString());
}

void SequenceFileLoaderTests::loadStandardFileWithEmptyLines() {
    //correct2.txt  has empty lines at the beginning, at the end and in the middle
    //              but contains same sequences as correct.txt
    
    CPPUNIT_ASSERT_NO_THROW(loader.load(path + "correct2.txt", seqArray););
    
    CPPUNIT_ASSERT_EQUAL(static_cast<SIZE>(123), seqArray.size());
    string first = "GGTACTTTTATTTTCGGAGCTGTGGGCAACATTCAAATACTAAGCGAAGCTCCGGCATGAAGTGCTTATGGACTGT";
    string line53 = "GTCGCATTAGCAATAAGGCTACCATATCTTGTGTGCTCTATAGCAAACAACAATCCATACTGGAGACGGAACGGAG";
    string last = "ATGAAATGTGGGTGAAAAAAGTAAGAAAATCATAGCAGATGGATATAGAACGAGTAACCATTTTAGCCAAGAACAA";
    CPPUNIT_ASSERT_EQUAL(first, seqArray.front().getAsString());
    CPPUNIT_ASSERT_EQUAL(line53, seqArray[52].getAsString());
    CPPUNIT_ASSERT_EQUAL(last, seqArray.back().getAsString());
}

void SequenceFileLoaderTests::loadFastqFileTest() {
    CPPUNIT_ASSERT_NO_THROW(loader.load(path + "correct.fastq", seqArray););
    
    CPPUNIT_ASSERT_EQUAL(static_cast<SIZE>(123), seqArray.size());
    string first = "NAAAACAAATTATTTATATTTATTTTTAATTTTAATTTTTTTTATTTTAATTTTTCCGGGCTTGCGGGCCNNCCGTGGCCCATTCGGGGTAA";
    string line54 = "TGCTCTTTATGCAGATATCACATATAGCATGCTTCACACTGATCTTAGGTATGCCATATACAAGTTCTTTAGAATTTAAATCTCTTAAGCTT";
    string last = "ATATTATATATGCAGAAATAGAAAATATGGGAGTTGCATTTTCACCACCAAATATGGGAGTATTTTTTATGATTAATAGTCTAACTTGATAT";
    CPPUNIT_ASSERT_EQUAL(first, seqArray.front().getAsString());
    CPPUNIT_ASSERT_EQUAL(line54, seqArray[53].getAsString());
    CPPUNIT_ASSERT_EQUAL(last, seqArray.back().getAsString());
}

void SequenceFileLoaderTests::loadNonExistingFileTest() {
    CPPUNIT_ASSERT_THROW(loader.load(path + "Unknown.txt", seqArray);, LoaderException);
    
    try { loader.load(path + "Unknown.txt", seqArray); } catch (LoaderException& e) {
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
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted.txt", seqArray);, LoaderException);
    try { loader.load(path + "corrupted.txt", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //U
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted2.txt", seqArray);, LoaderException);
    try { loader.load(path + "corrupted2.txt", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //small a
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted.fastq", seqArray);, LoaderException);
    try { loader.load(path + "corrupted.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //I
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted2.fastq", seqArray);, LoaderException);
    try { loader.load(path + "corrupted2.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_base_value"), string(e.what()));
    }
    
    //surplus newline
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted3.fastq", seqArray);, LoaderException);
    try { loader.load(path + "corrupted3.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_fastq_signature"), string(e.what()));
    }
    
    //missing +
    CPPUNIT_ASSERT_THROW(loader.load(path + "corrupted4.fastq", seqArray);, LoaderException);
    try { loader.load(path + "corrupted4.fastq", seqArray); } catch (LoaderException& e) {
        CPPUNIT_ASSERT_EQUAL(string("incorrect_fastq_signature"), string(e.what()));
    }
    
}
