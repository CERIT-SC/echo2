//
//  Sequence.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 08/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "Sequence.h"

//SEQUENCE
Sequence::Sequence() {
    seqLen = 0;
    seq = nullptr;
}

Sequence::Sequence(string str) {
    //initialize array
    // 10 bases are in 1 array field
    unsigned arrayLen = ceil(static_cast<double>(str.length())/10);
    seqLen = static_cast<unsigned>(str.length());
    seq = new unsigned[arrayLen];
    
    for (unsigned index = 0; index < arrayLen; index++) {
        seq[index] = 0;
    }
    
    //fill array with sequence
    unsigned arrIndex;
    short shift;
    for (unsigned index = 0; index < str.length(); index++) {
        arrIndex = index/10;
        shift = (index%10) * 3; //3 bits per base
        
        switch (str[index]) {
            case 'A': break; //means 0
            case 'C': seq[arrIndex] |= 1 << shift; break;
            case 'G': seq[arrIndex] |= 2 << shift; break;
            case 'T': seq[arrIndex] |= 3 << shift; break;
                
            case 'N': seq[arrIndex] |= 4 << shift; break;
            case 'Z': seq[arrIndex] |= 5 << shift; break;
                
            default: {
                delete [] seq;
                throw SequenceException("incorrect_base_value");
            }
        }
    }
}

Sequence::Sequence(const Sequence& sequence) {
    seqLen = sequence.seqLen;
    unsigned arrayLen = ceil(static_cast<double>(seqLen)/10);
    seq = new unsigned[arrayLen];
    
    for (unsigned i=0; i<arrayLen; i++) {
        seq[i] = sequence.seq[i];
    }
}

Sequence::~Sequence() {
    if (seq) {
        delete [] seq;
    }
}

Sequence& Sequence::operator=(const Sequence& s) {
    if (this == &s) return *this;
    if (seq) delete [] seq;
    
    seqLen = s.seqLen;
    unsigned arrayLen = ceil(static_cast<double>(seqLen)/10);
    seq = new unsigned[arrayLen];
    
    for (unsigned i=0; i<arrayLen; i++) {
        seq[i] = s.seq[i];
    }
    
    return *this;
}

string Sequence::getAsString() const {
    string result;
    unsigned arrIndex, shift;
    short numBase;
    
    for (unsigned seqIndex = 0; seqIndex < seqLen; seqIndex++) {
        arrIndex = seqIndex/10;
        shift = (seqIndex%10)*3;
        
        numBase = (seq[arrIndex] & 7<<shift) >> shift;
        result.push_back(translateToChar(numBase));
    }
    
    return result;
}

unsigned Sequence::getLength() const {
    return seqLen;
}

Interpreter* Sequence::interpretAsSeq() const {
    return new SeqInterpreter(seq, seqLen);
}

Interpreter* Sequence::interpretAsCompl() const {
    return new ComplInterpreter(seq, seqLen);
}

//private
char Sequence::translateToChar(short base) const {
    switch (base) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'G';
        case 3: return 'T';
            
        case 4: return 'N';
        case 5: return 'Z';
            
        default: return 'E'; //empty or error
    }
}



//INTERPRETERS
Interpreter::Interpreter(const unsigned * seq, unsigned seqLen) {
    this->seq = seq;
    this->seqLen = seqLen;
}

unsigned Interpreter::getLength() {
    return seqLen;
}

short Interpreter::baseValue(unsigned index) {
    assert(index < seqLen);
    
    unsigned arrIndex = index/10;
    short shift = (index%10) * 3;
    
    return (seq[arrIndex] & 7<<shift) >> shift;
}


//SeqInterpreter
SeqInterpreter::SeqInterpreter(const unsigned * seq, unsigned seqLen) : Interpreter(seq, seqLen) {}

short SeqInterpreter::at(unsigned index) {
    return baseValue(index);
}

bool SeqInterpreter::isCompl() {
    return false;
}

//ComplInterpreter
ComplInterpreter::ComplInterpreter(const unsigned * seq, unsigned seqLen) : Interpreter(seq, seqLen) {}

short ComplInterpreter::at(unsigned index) {
    short base = baseValue(seqLen-1-index);
    
    return reverseToCompl(base);
}

bool ComplInterpreter::isCompl() {
    return true;
}

//private
short ComplInterpreter::reverseToCompl(unsigned short base) {
    if (base < 4) {
        return 3-base;
    }
    
    return base;
}