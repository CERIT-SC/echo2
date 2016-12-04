//
//  DynamicArray.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 15/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

/*
 Created for memory savings.
 Have 16B overhead. std::vector has 24B.
 */


#ifndef EchoErrorCorrection_DynamicArray_h
#define EchoErrorCorrection_DynamicArray_h

#include <limits>
#include <stdexcept>
#include <cstring>
#include <algorithm>

#include "debug.h"
using namespace std;


template <class ArrType>
class DynamicArray {
    ArrType* array;
    unsigned _size, capacity;
    
    void operator=(const DynamicArray&); //copy forbidden
public:
    DynamicArray();
    DynamicArray(unsigned capacity);
    DynamicArray(const DynamicArray&);
    ~DynamicArray();
    
    ArrType& operator[](unsigned);
    const ArrType& operator[](unsigned) const;
    
    void push_back(const ArrType& element);
    unsigned size() const;
    void reserve(unsigned newCapacity);
    void resize(unsigned size);
    void shrink_to_fit();
    
    ArrType* begin();
    ArrType* end();
    
private:
    void realocate(unsigned newCapacity);
};




//TEMPLATE IMPLEMENTATION

template <class ArrType>
DynamicArray<ArrType>::DynamicArray() {
    array = nullptr;
    _size = capacity = 0;
}

template <class ArrType>
DynamicArray<ArrType>::DynamicArray(unsigned capacity) {
    array = nullptr;
    _size = capacity = 0;
    
    realocate(capacity);
}

template <class ArrType>
DynamicArray<ArrType>::DynamicArray(const DynamicArray& arr) {
    if (!arr.array) {
        _size = capacity = 0;
        array = nullptr;
        return;
    }
    
    array = new ArrType[arr._size];
    _size = capacity = arr._size;
    
    for (unsigned i = 0; i < _size; i++) {
        array[i] = arr.array[i];
    }
}

template <class ArrType>
DynamicArray<ArrType>::~DynamicArray() {
    delete [] array;
}

template <class ArrType>
ArrType& DynamicArray<ArrType>::operator[](unsigned index) {
    assert(index < _size);
    
    return array[index];
}

template <class ArrType>
const ArrType& DynamicArray<ArrType>::operator[](unsigned index) const {
    assert(index < _size);
    
    return array[index];
}

template <class ArrType>
void DynamicArray<ArrType>::push_back(const ArrType& element) {
    if (_size == numeric_limits<unsigned>::max()) throw length_error("DynamicArray: max length exceeded.");
    
    if (!array) realocate(1);
    if (_size == capacity) {
        if (capacity <= numeric_limits<unsigned>::max()/2) realocate(capacity*2);
        else realocate(numeric_limits<unsigned>::max());
    }
    
    array[_size] = element;
    _size++;
    
}

template <class ArrType>
unsigned DynamicArray<ArrType>::size() const {
    return _size;
}

template <class ArrType>
void DynamicArray<ArrType>::reserve(unsigned newCapacity) {
    realocate(newCapacity);
}

template <class ArrType>
void DynamicArray<ArrType>::resize(unsigned size) {
    assert(size <= _size);
    
    _size = size;
}

template <class ArrType>
void DynamicArray<ArrType>::shrink_to_fit() {
    if(capacity > _size) realocate(_size);
}

template <class ArrType>
ArrType* DynamicArray<ArrType>::begin() {
    return array;
}

template <class ArrType>
ArrType* DynamicArray<ArrType>::end() {
    return array+_size;
}

//private
template <class ArrType>
void DynamicArray<ArrType>::realocate(unsigned newCapacity) {
    if (newCapacity < _size) return;
    if (capacity == newCapacity) return;
    
    ArrType * newArray = new ArrType[newCapacity];
    capacity = newCapacity;
    
    //for (unsigned i = 0; i < _size; i++) {
    //    newArray[i] = array[i];
    //}
    
    move(array, array+_size, newArray);
    
    if(array) delete [] array;
    array = newArray;
}

#endif