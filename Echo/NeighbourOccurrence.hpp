//
//  NeighbourOccurrence.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 20/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__NeighbourOccurrence__
#define __EchoErrorCorrection__NeighbourOccurrence__

typedef unsigned long long ULL;


class NeighOccurrence {
    ULL ID;
    long long offset;
    unsigned numOfErr;
    
public:
    NeighOccurrence(ULL ID = 0, long long offset = 0, unsigned numOfErr = 0) :
        ID(ID), offset(offset), numOfErr(numOfErr) {}
    
    ULL getId() const { return ID; }
    void setId(ULL ID) { this->ID = ID; }

    long long getOffset() const { return offset; }
    void setOffset(long long off) { offset = off; }

    unsigned getNumOfErr() const { return numOfErr; }
    void setNumOfErr(unsigned errNum) { numOfErr = errNum; }
    
    bool operator==(NeighOccurrence& occ) { return ID == occ.ID; }

};


#endif /* defined(__EchoErrorCorrection__NeighbourOccurrence__) */
