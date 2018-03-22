//
//  DynamicArray.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 15/09/13.
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

#include "debug.hpp"
using namespace std;


template <class ArrType>
class DynamicArray {
    ArrType* array;
    unsigned _size, _capacity;
    
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
    unsigned capacity();
    void resize(unsigned size);
    void shrink_to_fit();
    
    ArrType* begin();
    ArrType* end();
    
private:
    void reallocate(unsigned newCapacity);
};




//TEMPLATE IMPLEMENTATION

template <class ArrType>
DynamicArray<ArrType>::DynamicArray() {
    array = nullptr;
    _size = _capacity = 0;
}

template <class ArrType>
DynamicArray<ArrType>::DynamicArray(unsigned capacity) {
    array = nullptr;
    _size = capacity = 0;
    
    reallocate(capacity);
}

template <class ArrType>
DynamicArray<ArrType>::DynamicArray(const DynamicArray& arr) {
    if (!arr.array) {
        _size = _capacity = 0;
        array = nullptr;
        return;
    }
    
    array = new ArrType[arr._size];
    _size = _capacity = arr._size;
    
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
    
    if (!array) reallocate(1);
    if (_size == _capacity) {
        if (_capacity <= numeric_limits<unsigned>::max()/2) reallocate(_capacity*2);
        else reallocate(numeric_limits<unsigned>::max());
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
    reallocate(newCapacity);
}

template <class ArrType>
unsigned DynamicArray<ArrType>::capacity() {
    return _capacity;
}

template <class ArrType>
void DynamicArray<ArrType>::resize(unsigned size) {
    assert(size <= _size);
    
    _size = size;
}

template <class ArrType>
void DynamicArray<ArrType>::shrink_to_fit() {
    if(_capacity > _size) reallocate(_size);
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
void DynamicArray<ArrType>::reallocate(unsigned newCapacity) {
    if (newCapacity < _size) return;
    if (_capacity == newCapacity) return;
    
    ArrType * newArray = new ArrType[newCapacity];
    _capacity = newCapacity;
    
    move(array, array+_size, newArray);
    
    if(array) delete [] array;
    array = newArray;
}

#endif
