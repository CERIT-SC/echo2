//
//  ToolsTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 09/03/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#include "ToolsTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(N44MatrixTests);

void N44MatrixTests::setUp() {
    matrix = new N44Matrix(10);
}

void N44MatrixTests::tearDown() {
    delete matrix;
}

void N44MatrixTests::emptyMatrixTest() {
    //tested via assert in implementation
}

void N44MatrixTests::emptyValueTest() {
    CPPUNIT_ASSERT_EQUAL(10u, matrix->getSize());
    
    for (int i=0; i<10; i++) {
        for (int b1=0; b1 < 4; b1++) {
            for (int b2 = 0; b2 < 4; b2++) {
                CPPUNIT_ASSERT_EQUAL(0.0, matrix->operator()(i, b1, b2));
            }
        }
    }
}

//tests if matrix retains values
void N44MatrixTests::saveValuesTest() {
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 1, 2) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    CPPUNIT_ASSERT_EQUAL(4.0, matrix->operator()(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, matrix->operator()(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, matrix->operator()(9, 3, 3));
}

void N44MatrixTests::copyTest() {
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 1, 2) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    N44Matrix mat1 = *matrix;
    N44Matrix mat2;
    mat2 = *matrix;
    
    CPPUNIT_ASSERT_EQUAL(10u, mat1.getSize());
    CPPUNIT_ASSERT_EQUAL(10u, mat2.getSize());
    
    CPPUNIT_ASSERT_EQUAL(4.0, mat1(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, mat1(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, mat1(9, 3, 3));
    
    CPPUNIT_ASSERT_EQUAL(4.0, mat2(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, mat2(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, mat2(9, 3, 3));
    
    mat1(0, 0, 0) = mat1(5, 1, 2) = mat1(9, 3, 3) = 0;
    mat2(0, 0, 0) = mat2(5, 1, 2) = mat2(9, 3, 3) = 0;
    
    for (int i=0; i<10; i++) {
        for (int b1=0; b1 < 4; b1++) {
            for (int b2 = 0; b2 < 4; b2++) {
                CPPUNIT_ASSERT_EQUAL(0.0, mat1(i, b1, b2));
                CPPUNIT_ASSERT_EQUAL(0.0, mat2(i, b1, b2));
            }
        }
    }
}

void N44MatrixTests::addOperatorTest() {
    N44Matrix mat2(10);
    
    mat2(0,0,0) = 2;
    mat2(5,2,1) = 3;
    mat2(9,3,3) = 4;
    
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 2, 1) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    mat2 += *matrix;
    
    CPPUNIT_ASSERT_EQUAL(6.0, mat2(0,0,0));
    CPPUNIT_ASSERT_EQUAL(8.0, mat2(5,2,1));
    CPPUNIT_ASSERT_EQUAL(10.0, mat2(9,3,3));
}


//OVERLAP INFO TESTS
CPPUNIT_TEST_SUITE_REGISTRATION(GetOverapInfoTests);

void GetOverapInfoTests::setUp() {
    //sequeces are aranged in their overlap manner
    sequences = { Sequence("AAAAAAAAAA"),    //id 0 length 10
                  Sequence("AAAAA"),         //id 2 length 5
                  Sequence("AAAAAAAAA"),     //id 4 length 9
                  Sequence("AAAAAAAAAAAA")}; //id 6 length 12
    
    access = new RandomisedAccess(sequences, false);
}

void GetOverapInfoTests::tearDown() {
    delete access;
}

//AAAAAAAAAA
//   AAAAA
void GetOverapInfoTests::smallNeighbOverlapTest() {
    info = {2, 3, 0};
    getOverlapInfo(0, info, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(3u, seqPos);
    CPPUNIT_ASSERT_EQUAL(0u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
    
    seqOccur = {0, 4};
    neighbOccur = {2, 1};
    getOverlapInfo(seqOccur, neighbOccur, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(3u, seqPos);
    CPPUNIT_ASSERT_EQUAL(0u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
}

//  AAAAA
//AAAAAAAAAA
void GetOverapInfoTests::bigNeighbOverlapTest() {
    info = {0, -2, 0};
    getOverlapInfo(2, info, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(0u, seqPos);
    CPPUNIT_ASSERT_EQUAL(2u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
    
    seqOccur = {2, 2};
    neighbOccur = {0, 4};
    getOverlapInfo(seqOccur, neighbOccur, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(0u, seqPos);
    CPPUNIT_ASSERT_EQUAL(2u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
}

//AAAAAAAAA
//    AAAAAAAAAAAA
void GetOverapInfoTests::neighbOffsetTest() {
    info = {6, 4, 0};
    getOverlapInfo(4, info, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(4u, seqPos);
    CPPUNIT_ASSERT_EQUAL(0u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
    
    seqOccur = {4, 8};
    neighbOccur = {6, 4};
    getOverlapInfo(seqOccur, neighbOccur, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(4u, seqPos);
    CPPUNIT_ASSERT_EQUAL(0u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(5u, overalpSize);
}

//    AAAAAAAAA
//AAAAAAAAAAAA
void GetOverapInfoTests::seqOffsetTest() {
    info = {6, -4, 0};
    getOverlapInfo(4, info, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(0u, seqPos);
    CPPUNIT_ASSERT_EQUAL(4u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(8u, overalpSize);
    
    seqOccur = {4, 0};
    neighbOccur = {6, 4};
    getOverlapInfo(seqOccur, neighbOccur, *access, seqPos, neighbPos, overalpSize);
    
    CPPUNIT_ASSERT_EQUAL(0u, seqPos);
    CPPUNIT_ASSERT_EQUAL(4u, neighbPos);
    CPPUNIT_ASSERT_EQUAL(8u, overalpSize);
}
