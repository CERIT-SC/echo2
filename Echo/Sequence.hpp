//
//  Sequence.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 08/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//


/*
 The Sequence class saves sequences in a way, that every DNA base is saved into 3 bits
 to save memory. There can be 10 bases in unsigned field (32 bits).
 Conversion table:
    A   0   000
    C   1   001
    G   2   010
    T   3   011
 
    N   4   100
    Z   5   101
 empty  7   111
 
 Interpreters are access objects, that interprets data as sekvence or as reverse complement.
 */

/* 
 Exceptions:
 Exception class SequenceException
 SequenceException::what() returns:
 incorrect_base_value   - Sequence::Sequence() can return this
 */

#ifndef __EchoErrorCorrection__Sequence__
#define __EchoErrorCorrection__Sequence__

#include <string>
#include <cmath>
#include <memory>
#include <stdexcept>

#include "debug.hpp"
using namespace std;

//forward declarations
class Interpreter;
class SeqInterpreter;
class ComplInterpreter;


//SEQUENCE
class Sequence {
    unsigned *seq;
    unsigned seqLen;
    
public:
    Sequence();
    Sequence(string);
    Sequence(const Sequence&);
    ~Sequence();
    
    Sequence& operator=(const Sequence&);
    
    string getAsString() const;
    unsigned getLength() const;
    //void load(string);
    
    Interpreter* interpretAsSeq() const;
    Interpreter* interpretAsCompl() const;
    
private:
    char translateToChar(short base) const;
};


//INTERPRETERS
class Interpreter {
protected:
    const unsigned * seq;
    unsigned seqLen;
    
public:
    Interpreter(const unsigned * seq, unsigned seqLen);
    virtual ~Interpreter() {}
    
    virtual short at(unsigned) = 0;
    virtual bool  isCompl() = 0;
    unsigned getLength();
    
protected:
    inline short baseValue(unsigned index);
};



class SeqInterpreter : public Interpreter {
public:
    SeqInterpreter(const unsigned * seq, unsigned seqLen);
    
    virtual short at(unsigned);
    virtual bool  isCompl();
};



class ComplInterpreter : public Interpreter {
    
public:
    ComplInterpreter(const unsigned * seq, unsigned seqLen);
    
    virtual short at(unsigned);
    virtual bool  isCompl();
    
private:
    inline short reverseToCompl(unsigned short base);
};


//EXCEPTION
class SequenceException : public std::runtime_error {
public:
    SequenceException(string m) : runtime_error(m) {}
};

#endif /* defined(__EchoErrorCorrection__Sequence__) */
