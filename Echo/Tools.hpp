//
//  Tools.hpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 03/03/14.
//

#ifndef __EchoErrorCorrection__Tools__
#define __EchoErrorCorrection__Tools__

#include <vector>
#include <tuple>
#include <array>

#include "Sequence.hpp"
#include "RandomisedAccess.hpp"
#include "NeighbourTable.hpp"
#include "HashKmerOccTable.hpp"
#include "debug.hpp"
using namespace std;

typedef unsigned long long ULL;


unsigned getHash(const char *startPos,
                  const char *endPos,
                  bool& isComputed);


class N44Matrix {
    vector<double*> matrix;
    
public:
    N44Matrix() {}
    N44Matrix(unsigned length);
    N44Matrix(const N44Matrix&);
    ~N44Matrix();
    
    N44Matrix& operator=(const N44Matrix&);
    N44Matrix& operator+=(const N44Matrix&);
    double &operator()(unsigned indexN, unsigned row, unsigned column);
    const double& operator()(unsigned indexN, unsigned row, unsigned column) const;
    unsigned getSize() const;
    
    friend ostream& operator<<(ostream& os, N44Matrix &);
};



struct Hypothesis {
    int b1, b2, b1b2;
    
    Hypothesis() : b1(0), b2(0), b1b2(0) {}
    Hypothesis(int b1, int b2, int b1b2): b1(b1), b2(b2), b1b2(b1b2) {}
};

vector<Hypothesis> generateHypothesis(bool isHeterozygous);


struct VoteInfo {
    unsigned pos;
    short base;
    bool isComplement, isPrior;
    
    VoteInfo(unsigned pos, short base, bool isComplement, bool isPrior = false)
    : pos(pos), base(base), isComplement(isComplement), isPrior(isPrior){}
};


//maximum likelihood estimation
//fills up expectedBase and baseLogQuality
void mlEstimation(unsigned baseIndex, const vector<vector<VoteInfo>>& votes,
                  vector<Hypothesis>& hypothesis,
                  N44Matrix& logLikelihood, float heterozygousRate,
                  Hypothesis& expectedBase, array<double, 16>& baseLogQuality);

//converts confusion matrix to logarithmic likelihood matrix
void confToLikelihood(N44Matrix confMat, N44Matrix& logLikelihood);

//overlaps
void getOverlapInfo(ULL seqId, Neighb neighbInfo, const RandomisedAccess& access,
                    unsigned& seqPos, unsigned& neibhbPos, unsigned& overlapSize);

void getOverlapInfo(Occur seqOccur, Occur neighbOccur, const RandomisedAccess& access,
                    unsigned& seqPos, unsigned& neighbPos, unsigned& overlapSize);

//resulting array has saved random index in field that has original sequence index
//orig index -> random index
vector<ULL> getOriginalOrder(const RandomisedAccess& access);

#endif /* defined(__EchoErrorCorrection__Tools__) */
