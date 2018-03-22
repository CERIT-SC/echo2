//
//  OptionsTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 21/11/13.
//

#include "OptionsTests.hpp"
#include <string>
#include <vector>
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( OptionsTest );

void OptionsTest::setUp() {}
void OptionsTest::tearDown() {}

//tests
void OptionsTest::emptyOptionsTest() {
    const char *arg[] = {"some path"};
    Options opt(1, arg);
    
    CPPUNIT_ASSERT(0 == opt.inputFileNames().size());
    CPPUNIT_ASSERT(0 == opt.outputFileNames().size());
    CPPUNIT_ASSERT(string("") == opt.logFileName());
    CPPUNIT_ASSERT(1 == opt.numberOfCores());
    CPPUNIT_ASSERT(0 == opt.kmerLenght());
    CPPUNIT_ASSERT(0.0 == opt.minErrToler());
    CPPUNIT_ASSERT(0.0 == opt.maxErrToler());
    CPPUNIT_ASSERT(0 == opt.minOverlap());
    CPPUNIT_ASSERT(0 == opt.maxOverlap());
    CPPUNIT_ASSERT(0 == opt.heterRate());
    CPPUNIT_ASSERT(0 == opt.paramSampleSize());
    CPPUNIT_ASSERT(0 == opt.hashTableSize());
}

void OptionsTest::inputFileNamesTest() {
    const char *arg[] = {"some path","input1" };
    const char *arg2[] = {"some path","input2", "input3", "-o", "output", "input4" };
    Options opt(2, arg), opt2(6, arg2);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(1), opt.inputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("input1"), opt.inputFileNames()[0]);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(3), opt2.inputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("input2"), opt2.inputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("input3"), opt2.inputFileNames()[1]);
    CPPUNIT_ASSERT_EQUAL(string("input4"), opt2.inputFileNames()[2]);
}

void OptionsTest::outputFileNamesTest() {
    const char *argum[] =  {"some path", "-o", "OutputFile.txt" };
    const char *argum2[] = {"some path", "--output", "OutputFile2.txt" };
    const char *argum3[] = {"some path", "input1", "-o", "output1", "--output", "output2", "input2"};
    Options opt(3,argum), opt2(3,argum2), opt3(7, argum3);
    
    CPPUNIT_ASSERT_EQUAL(string("OutputFile.txt"), opt.outputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("OutputFile2.txt"), opt2.outputFileNames()[0]);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(2), opt3.outputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("output1"), opt3.outputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("output2"), opt3.outputFileNames()[1]);
}

void OptionsTest::logFileNameTest() {
    const char *argum[] = {"some path", "-l", "LogFile.txt" };
    const char *argum2[] = {"some path", "--log", "LogFile2.txt" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(string("LogFile.txt"), opt.logFileName());
    CPPUNIT_ASSERT_EQUAL(string("LogFile2.txt"), opt2.logFileName());
}

void OptionsTest::numberOfCoresTest() {
    const char *argum[] = {"some path", "-n", "10" };
    const char *argum2[] = {"some path", "--ncpu", "15" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(10, opt.numberOfCores());
    CPPUNIT_ASSERT_EQUAL(15, opt2.numberOfCores());
}

void OptionsTest::kmerLengthTest() {
    const char *argum[] = {"some path", "-k", "10" };
    const char *argum2[] = {"some path", "--kmer", "15" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(10, opt.kmerLenght());
    CPPUNIT_ASSERT_EQUAL(15, opt2.kmerLenght());
}

void OptionsTest::minErrTolerTest() {
    const char *argum[] = {"some path", "-e", "4.5" };
    const char *argum2[] = {"some path", "--min_error_tolerance", "5.6" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4.5, opt.minErrToler());
    CPPUNIT_ASSERT_EQUAL(5.6, opt2.minErrToler());
}

void OptionsTest::maxErrTolerTest() {
    const char *argum[] = {"some path", "-E", "4.5" };
    const char *argum2[] = {"some path", "--max_error_tolerance", "5.6" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4.5, opt.maxErrToler());
    CPPUNIT_ASSERT_EQUAL(5.6, opt2.maxErrToler());
}

void OptionsTest::minOverlapTest() {
    const char *argum[] = {"some path", "-h", "4" };
    const char *argum2[] = {"some path", "--min_min_overlap", "5" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4, opt.minOverlap());
    CPPUNIT_ASSERT_EQUAL(5, opt2.minOverlap());
}

void OptionsTest::maxOverlapTest() {
    const char *argum[] = {"some path", "-H", "4" };
    const char *argum2[] = {"some path", "--max_min_overlap", "5" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4, opt.maxOverlap());
    CPPUNIT_ASSERT_EQUAL(5, opt2.maxOverlap());
}

void OptionsTest::heterRateTest() {
    const char *argum[] = {"some path", "-r", "4.1" };
    const char *argum2[] = {"some path", "--heterozygous_rate", "5.2" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4.1f, opt.heterRate());
    CPPUNIT_ASSERT_EQUAL(5.2f, opt2.heterRate());
}

void OptionsTest::paramSampleSizeTest() {
    const char *argum[] = {"some path", "-s", "4" };
    const char *argum2[] = {"some path", "--sample_size", "5" };
    Options opt(3,argum), opt2(3, argum2);
    
    CPPUNIT_ASSERT_EQUAL(4, opt.paramSampleSize());
    CPPUNIT_ASSERT_EQUAL(5, opt2.paramSampleSize());
}

void OptionsTest::hashTableSizeTest() {
    const char *argum[] = {"some path", "--hash_size", "25"};
    Options opt(3, argum);
    
    CPPUNIT_ASSERT_EQUAL(25, opt.hashTableSize());
}

void OptionsTest::stateAndErrMessageTest() {
    //test empty
    const char * arg[] = {"some path"};
    Options opt(1, arg);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt.getState());
    CPPUNIT_ASSERT(opt.getErrMessage().find("Too few arguments.") != string::npos);
    
    //test for help
    const char * arg2[] = {"some path", "--help" };
    Options opt2(2, arg2);
    CPPUNIT_ASSERT_EQUAL(ST_HELP, opt2.getState());
    CPPUNIT_ASSERT_EQUAL(string(""), opt2.getErrMessage());
    
    //at least one input file must be set
    const char * arg3[] = {"some path", "-o", "output" };
    Options opt3(3, arg3);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt3.getState());
    CPPUNIT_ASSERT(opt3.getErrMessage().find("At least one input file must be set.") != string::npos);
    
    //test for incorrect value
    const char * arg4[] = {"some path", "input", "-h", "output" };
    const char * arg7[] = {"some path", "input", "-e", "-7.2" };
    Options opt4(4, arg4), opt7(4,arg7);;
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt4.getState());
    CPPUNIT_ASSERT(opt4.getErrMessage().find("Incorrect value of parameter.") != string::npos);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt7.getState());
    CPPUNIT_ASSERT(opt7.getErrMessage().find("Incorrect value") != string::npos);
    
    //test for switch without value
    const char * arg5[] = {"some path", "input", "-h" };
    Options opt5(3, arg5);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt5.getState());
    CPPUNIT_ASSERT(opt5.getErrMessage().find("Last flag requires value.") != string::npos);
    
    //number of output files must be less/equal then input files
    const char * arg6[] = {"some path", "input", "-o", "output1", "-o", "output2" };
    Options opt6(6, arg6);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt6.getState());
    CPPUNIT_ASSERT(opt6.getErrMessage().find("There are more output files, than input files.") != string::npos);
    
    //correct input
    const char * argCorrect[] = {"some path", "-o", "output", "-k", "15", "-n", "80", "input/input.fastq" };
    Options optCorrect(8, argCorrect);
    CPPUNIT_ASSERT_EQUAL(ST_OK, optCorrect.getState());
    CPPUNIT_ASSERT_EQUAL(string(""), optCorrect.getErrMessage());
    
    //get application version
    const char * argVersion[] = {"some path", "--version"};
    Options optVersion(2, argVersion);
    CPPUNIT_ASSERT_EQUAL(ST_VERSION, optVersion.getState());
}

void OptionsTest::allInputsTest() {
    const char *argum[] = {"some path", "input", "-o", "output", "-l", "log", "-n", "8",
        "-k", "15", "-e", "3.2", "-E", "4.5", "-h", "10", "-H", "20", "-r", "7", "-s", "9",
        "input2", "--hash_size", "30" };
    Options opt(25, argum);
    
    CPPUNIT_ASSERT_EQUAL(ST_OK, opt.getState());
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(2), opt.inputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("input"), opt.inputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("input2"), opt.inputFileNames()[1]);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(1), opt.outputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("output"), opt.outputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("log"), opt.logFileName());
    CPPUNIT_ASSERT_EQUAL(8, opt.numberOfCores());
    CPPUNIT_ASSERT_EQUAL(15, opt.kmerLenght());
    CPPUNIT_ASSERT_EQUAL(3.2, opt.minErrToler());
    CPPUNIT_ASSERT_EQUAL(4.5, opt.maxErrToler());
    CPPUNIT_ASSERT_EQUAL(10, opt.minOverlap());
    CPPUNIT_ASSERT_EQUAL(20, opt.maxOverlap());
    CPPUNIT_ASSERT_EQUAL(7.0f, opt.heterRate());
    CPPUNIT_ASSERT_EQUAL(9, opt.paramSampleSize());
    CPPUNIT_ASSERT_EQUAL(30, opt.hashTableSize());
}

void OptionsTest::unknownFlagTest() {
    const char *arg[] = {"some path", "input", "-unknown", };
    const char *arg2[] = {"some path", "input", "--unknown", "7" };
    const char *arg3[] = {"some path", "-unknown" };
    Options opt(3,arg), opt2(4,arg2), opt3(2,arg3);
    
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt.getState());
    CPPUNIT_ASSERT(opt.getErrMessage().find("Unrecognised argument:") != string::npos);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt2.getState());
    CPPUNIT_ASSERT(opt2.getErrMessage().find("Unrecognised argument:") != string::npos);
    CPPUNIT_ASSERT_EQUAL(ST_INP_ERROR, opt3.getState());
    CPPUNIT_ASSERT(opt3.getErrMessage().find("Unrecognised argument:") != string::npos);
}

void OptionsTest::endFlagOptionTest() {     //testing for -- flag
    const char *arg[] = {"some path", "input", "-l", "log", "--", "-input2", "--input3" };
    Options opt(7, arg);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<string>::size_type>(3), opt.inputFileNames().size());
    CPPUNIT_ASSERT_EQUAL(string("input"), opt.inputFileNames()[0]);
    CPPUNIT_ASSERT_EQUAL(string("-input2"), opt.inputFileNames()[1]);
    CPPUNIT_ASSERT_EQUAL(string("--input3"), opt.inputFileNames()[2]);
}
