//
//  DynamicArrayTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 30/11/13.
//

#include "DynamicArrayTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(DynamicArrayTests);

void DynamicArrayTests::setUp() {}
void DynamicArrayTests::tearDown() {}


void DynamicArrayTests::emptyArrayTest() {
    DynamicArray<int> arr;
    
    CPPUNIT_ASSERT_EQUAL(0u, arr.size());
    
    int *pointer = nullptr;
    CPPUNIT_ASSERT_EQUAL(pointer, arr.begin());
    CPPUNIT_ASSERT_EQUAL(pointer, arr.end());
}

void DynamicArrayTests::addElementTest() {
    DynamicArray<int> arr;
    arr.push_back(5);
    
    CPPUNIT_ASSERT_EQUAL(1u, arr.size());
    CPPUNIT_ASSERT_EQUAL(5, arr[0]);
    
    //iterators
    CPPUNIT_ASSERT_EQUAL(5, *arr.begin());
    CPPUNIT_ASSERT_EQUAL(arr.begin()+1, arr.end());
    CPPUNIT_ASSERT_EQUAL(5, *(arr.end()-1));
}

void DynamicArrayTests::addMultipleElementsTest() {
    DynamicArray<int> arr;
    for (int i=0; i<10; i++) {
        arr.push_back(i);
    }
    
    CPPUNIT_ASSERT_EQUAL(10U, arr.size());
    for (int i=0; i<10; i++) {
        CPPUNIT_ASSERT_EQUAL(i, arr[i]);
    }
    
    //iterators
    CPPUNIT_ASSERT_EQUAL(0, *arr.begin());
    CPPUNIT_ASSERT_EQUAL(arr.begin()+10, arr.end());
    CPPUNIT_ASSERT_EQUAL(9, *(arr.end()-1));
}

void DynamicArrayTests::copyConstructorTest() {
    DynamicArray<int> *arr = new DynamicArray<int>;
    for (int i=0; i<10; i++) {
        arr->push_back(i);
    }
    
    DynamicArray<int> arr2(*arr);
    delete arr;
    
    CPPUNIT_ASSERT_EQUAL(10u, arr2.size());
    for (int i=0; i<10; i++) {
        CPPUNIT_ASSERT_EQUAL(i, arr2[i]);
    }
}

//void DynamicArrayTests::invalidAccessTest() {
//    DynamicArray<int> arr;
//    
//    CPPUNIT_ASSERT_THROW(arr[0], )
//}

void DynamicArrayTests::shrinkToFitTest() {
    DynamicArray<int> arr(100);
    
    CPPUNIT_ASSERT_EQUAL(0u, arr.size());
    arr.push_back(4);
    arr.push_back(7);
    arr.shrink_to_fit();
    
    CPPUNIT_ASSERT_EQUAL(2u, arr.size());
    CPPUNIT_ASSERT_EQUAL(4, arr[0]);
    CPPUNIT_ASSERT_EQUAL(7, arr[1]);
}

void DynamicArrayTests::reserveTest() {
    DynamicArray<int> arr;
    
    arr.push_back(4);
    arr.push_back(7);
    arr.reserve(100);
    
    CPPUNIT_ASSERT_EQUAL(2u, arr.size());
    CPPUNIT_ASSERT_EQUAL(4, arr[0]);
    CPPUNIT_ASSERT_EQUAL(7, arr[1]);
}
