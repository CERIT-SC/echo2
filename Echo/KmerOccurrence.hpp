//
//  KmerOccurrence.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 14/09/13.
//

/*
 This way of saving KmerOccurrences is for memory savings.
 The goal is to use sufficient one with least memory requirements.
 Other ways like pair class members rounds up memory utilization.
 */

#ifndef __EchoErrorCorrection__KmerOccurrence__
#define __EchoErrorCorrection__KmerOccurrence__

typedef unsigned long long ULL;


//unsigned id versions
class KmerOcc_UnsChar { //KmerOccurrence: unsigned id, char position version
    unsigned char data[5];
    
public:
    KmerOcc_UnsChar(ULL seqId = 0, unsigned pos = 0);
    
    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};



class KmerOcc_UnsShort {
    unsigned char data[6];
    
public:
    KmerOcc_UnsShort(ULL seqId = 0, unsigned pos = 0);
    
    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};



class KmerOcc_UnsUns {
    unsigned seqId;
    unsigned pos;
    
public:
    KmerOcc_UnsUns(ULL seqId = 0, unsigned pos = 0);
    
    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};




//ULL id versions
class KmerOcc_UllChar {
    unsigned char data[9];
    
public:
    KmerOcc_UllChar(ULL seqId = 0, unsigned pos = 0);

    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};



class KmerOcc_UllShort {
    unsigned char data[10];
    
public:
    KmerOcc_UllShort(ULL seqId = 0, unsigned pos = 0);

    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};



class KmerOcc_UllUns {
    unsigned char data[12];
    
public:
    KmerOcc_UllUns(ULL seqId = 0, unsigned pos = 0);
    
    ULL getId() const;
    unsigned getPos() const;
    void setId(ULL seqId);
    void setPos(unsigned pos);
};

#endif /* defined(__EchoErrorCorrection__KmerOccurrence__) */
