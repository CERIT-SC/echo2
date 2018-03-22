//
//  PoissonTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//

#include "PoissonTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(PoissonTests);

void PoissonTests::setUp() {
    
}

void PoissonTests::tearDown() {
    
}



void PoissonTests::poissonLengthTest() {
    vector<long double> dist = poissonDist(21, 4);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<vector<long double>::size_type>(21), dist.size());
}

void PoissonTests::poissonValuesTest() {
    auto dist1 = poissonDist(20, 1);
    auto dist5 = poissonDist(20, 5);
    
    //dist1
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.367, dist1[0], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.367, dist1[1], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.183, dist1[2], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.003, dist1[5], 0.001);
    
    //dist5
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.006, dist5[0], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.084, dist5[2], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.175, dist5[5], 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.036, dist5[9], 0.001);
    
}

void PoissonTests::distrEstimationTest() {
    vector<double> dist1 = {1,6,16,28,35,35,29,20,13,7,3,1,1};
    vector<double> dist2 = dist1, dist3 = dist1;
    dist2[5] += 1; dist3[5] += 2;
    
    unsigned estimMean1, estimMean2, estimMean3;
    double variation1, variation2, variation3;
    poissonDistEstimation(dist1, estimMean1, variation1);
    poissonDistEstimation(dist2, estimMean2, variation2);
    poissonDistEstimation(dist3, estimMean3, variation3);
    
    CPPUNIT_ASSERT_EQUAL(5u, estimMean1);
    
    CPPUNIT_ASSERT(estimMean1 == estimMean2);
    CPPUNIT_ASSERT(estimMean2 == estimMean3);
    
    CPPUNIT_ASSERT(variation1 < variation2);
    CPPUNIT_ASSERT(variation2 < variation3);
    
}
