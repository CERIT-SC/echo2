//
//  KmerOccurrence.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "KmerOccurrence.hpp"

//KmerOcc_UnsChar
KmerOcc_UnsChar::KmerOcc_UnsChar(ULL seqId, unsigned pos) {
    setId(seqId);
    setPos(pos);
}

ULL KmerOcc_UnsChar::getId() const {
    return *reinterpret_cast<const unsigned*>(data);
}

unsigned KmerOcc_UnsChar::getPos() const {
    return data[4];
}

void KmerOcc_UnsChar::setId(ULL seqId) {
    (*reinterpret_cast<unsigned*>(data)) = static_cast<unsigned>(seqId);
}

void KmerOcc_UnsChar::setPos(unsigned pos) {
    data[4] = pos;
}




//KmerOcc_UnsShort
KmerOcc_UnsShort::KmerOcc_UnsShort(ULL seqId, unsigned pos) {
    setId(seqId);
    setPos(pos);
}

ULL KmerOcc_UnsShort::getId() const {
    return *reinterpret_cast<const unsigned*>(data);
}

unsigned KmerOcc_UnsShort::getPos() const {
    return *reinterpret_cast<const unsigned short*>(data+4);
}

void KmerOcc_UnsShort::setId(ULL seqId) {
    (*reinterpret_cast<unsigned*>(data)) = static_cast<unsigned>(seqId);
}

void KmerOcc_UnsShort::setPos(unsigned pos) {
    (*reinterpret_cast<unsigned short*>(data+4)) = pos;
}




//KmerOcc_UnsUns
KmerOcc_UnsUns::KmerOcc_UnsUns(ULL seqId, unsigned pos) {
    this->seqId = static_cast<unsigned>(seqId);
    this->pos = pos;
}

ULL KmerOcc_UnsUns::getId() const {
    return seqId;
}

unsigned KmerOcc_UnsUns::getPos() const {
    return pos;
}

void KmerOcc_UnsUns::setId(ULL seqId) {
    this->seqId = static_cast<unsigned>(seqId);
}

void KmerOcc_UnsUns::setPos(unsigned pos) {
    this->pos = pos;
}



//ULL id versions
//KmerOcc_UllChar
KmerOcc_UllChar::KmerOcc_UllChar(ULL seqId, unsigned pos) {
    setId(seqId);
    setPos(pos);
}

ULL KmerOcc_UllChar::getId() const {
    return *reinterpret_cast<const ULL*>(data);
}

unsigned KmerOcc_UllChar::getPos() const {
    return data[8];
}

void KmerOcc_UllChar::setId(ULL seqId) {
    (*reinterpret_cast<ULL*>(data)) = seqId;
}

void KmerOcc_UllChar::setPos(unsigned pos) {
    data[8] = pos;
}




//KmerOcc_UllShort
KmerOcc_UllShort::KmerOcc_UllShort(ULL seqId, unsigned pos) {
    setId(seqId);
    setPos(pos);
}

ULL KmerOcc_UllShort::getId() const {
    return *reinterpret_cast<const ULL*>(data);
}

unsigned KmerOcc_UllShort::getPos() const {
    return *reinterpret_cast<const unsigned short*>(data+8);
}

void KmerOcc_UllShort::setId(ULL seqId) {
    (*reinterpret_cast<ULL*>(data)) = seqId;
}

void KmerOcc_UllShort::setPos(unsigned pos) {
    (*reinterpret_cast<unsigned short*>(data+8)) = pos;
}




//KmerOcc_UllUns
KmerOcc_UllUns::KmerOcc_UllUns(ULL seqId, unsigned pos) {
    setId(seqId);
    setPos(pos);
}

ULL KmerOcc_UllUns::getId() const {
    return *reinterpret_cast<const ULL*>(data);
}

unsigned KmerOcc_UllUns::getPos() const {
    return *reinterpret_cast<const unsigned*>(data+8);
}

void KmerOcc_UllUns::setId(ULL seqId) {
    (*reinterpret_cast<ULL*>(data)) = seqId;
}

void KmerOcc_UllUns::setPos(unsigned pos) {
    (*reinterpret_cast<unsigned*>(data+8)) = pos;
}
