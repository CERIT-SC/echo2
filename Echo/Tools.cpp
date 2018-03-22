//
//  Tools.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 03/03/14.
//

#include "Tools.hpp"

unsigned getHash(const char *startPos,
                 const char *endPos,
                 bool& isComputed) {
    unsigned hash = 0;
    isComputed = false;
    
    for (const char* it = startPos; it != endPos; it++) {
        if(*it < 4) {hash = 65599 * hash + *it; isComputed = true; }
    }
    
    return hash ^ (hash >> 16);
}


//N44MATRIX
N44Matrix::N44Matrix(unsigned length) {
    matrix.resize(length);
    
    for(double*& el: matrix) {
        el = new double[16];
        
        for (short i1=0; i1<4; i1++) {
            for (short i2=0; i2<4; i2++) {
                el[i1*4 + i2] = 0;
            }
        }
    }
}

N44Matrix::N44Matrix(const N44Matrix& mat2) {
    matrix.resize(mat2.matrix.size());
    
    for (unsigned index=0; index < matrix.size(); index++) {
        matrix[index] = new double[16];
        
        for (unsigned pos=0; pos < 16; pos++) {
            matrix[index][pos] = mat2.matrix[index][pos];
        }
    }
}

N44Matrix::~N44Matrix() {
    for(double*& el: matrix) {
        delete [] el;
    }
}

N44Matrix& N44Matrix::operator=(const N44Matrix& mat2) {
    if (matrix.size() != mat2.matrix.size()) {
        for(double*& el: matrix) delete [] el;
        
        matrix.resize(mat2.matrix.size());
        for (double*& el: matrix) el = new double[16];
    }
    
    for (unsigned index=0; index < matrix.size(); index++) {
        for (unsigned pos=0; pos < 16; pos++) {
            matrix[index][pos] = mat2.matrix[index][pos];
        }
    }
    
    return *this;
}

N44Matrix& N44Matrix::operator+=(const N44Matrix& mat2) {
    assert(matrix.size() == mat2.matrix.size());
    
    for (unsigned index=0; index<matrix.size(); index++) {
        for (short pos=0; pos < 16; pos++) {
            matrix[index][pos] += mat2.matrix[index][pos];
        }
    }
    
    return *this;
}

double &N44Matrix::operator()(unsigned indexN, unsigned row, unsigned column) {
    assert(indexN < matrix.size());
    assert(row < 4);
    assert(column < 4);
    
    return matrix[indexN][row*4 + column];
}

const double& N44Matrix::operator()(unsigned indexN, unsigned row, unsigned column) const {
    assert(indexN < matrix.size());
    assert(row < 4);
    assert(column < 4);
    
    return matrix[indexN][row*4 + column];
}

unsigned N44Matrix::getSize() const {
    return static_cast<unsigned>(matrix.size());
}


//N44Matrix ostream function
ostream& operator<<(ostream& os, N44Matrix& matrix) {
    for (vector<double*>::size_type index = 0; index < matrix.matrix.size(); index++) {
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                os << matrix.matrix[index][x*4+y];
                if (y<3) os << " ";
            }
            if(x < 3) os << "\n";
        }
        if(index+1 < matrix.matrix.size()) os << "\n\n";
    }
    
    return os;
}


//FUNCTIONS
vector<Hypothesis> generateHypothesis(bool isHeterozygous) {
    vector<Hypothesis> hypArr;
    
    if (isHeterozygous) {
        for (int b1=0; b1 < 4; b1++) {
            for (int b2 = b1; b2 < 4; b2++) {
                hypArr.push_back(Hypothesis(b1, b2, b1*4+b2));
            }
        }
    } else {
        for (int b=0; b<4; b++) {
            hypArr.push_back(Hypothesis(b, b, b*4+b));
        }
    }
    
    return hypArr;
}


void mlEstimation(unsigned baseIndex, const vector<vector<VoteInfo>>& votes, vector<Hypothesis>& hypothesis,
                  N44Matrix& logLikelihood, float heterozygousRate,
                  Hypothesis& expectedBase, array<double, 16>& baseLogQuality) {
    
    array<double, 16> baseLogLikelihood;
    for (double &i: baseLogLikelihood) i = 0;
    for (double &i: baseLogQuality) i = 0;
    
    for (const VoteInfo& baseVote: votes[baseIndex]) {
        for (const Hypothesis& hyp: hypothesis) {
            
            short readB1 = baseVote.isComplement ? 3-hyp.b1 : hyp.b1;
            short readB2 = baseVote.isComplement ? 3-hyp.b2 : hyp.b2;
            
            if (hyp.b1 == hyp.b2) { //Homozygous case
                double prob = logLikelihood(baseVote.pos, baseVote.base, readB1);
                
                baseLogLikelihood[hyp.b1b2] += prob;
                if (!baseVote.isPrior) baseLogQuality[hyp.b1b2] += prob;
                
            } else { //(b1 < b2) Heterozygous case
                double prob = 0.5*exp(logLikelihood(baseVote.pos, baseVote.base, readB1));
                prob += 0.5*exp(logLikelihood(baseVote.pos, baseVote.base, readB2));
                prob = log(prob);
                
                baseLogLikelihood[hyp.b1b2] += prob;
                if(!baseVote.isPrior) baseLogQuality[hyp.b1b2] += prob;
            }
        }
    }
    
    double maxLogLikelihood = -numeric_limits<double>::infinity();
    for (const Hypothesis& hyp: hypothesis) {
        
        if (hyp.b1 == hyp.b2) {
            baseLogLikelihood[hyp.b1b2] += log(0.25) + log(1.0 - heterozygousRate);
            baseLogQuality[hyp.b1b2] += log(0.25) + log(1.0 - heterozygousRate);
        } else { //b1 < b2
            baseLogLikelihood[hyp.b1b2] += -log(6.0) + log(heterozygousRate);
            baseLogQuality[hyp.b1b2] += -log(6.0) + log(heterozygousRate);
        }
        
        if (baseLogLikelihood[hyp.b1b2] > maxLogLikelihood) {
            maxLogLikelihood = baseLogLikelihood[hyp.b1b2];
            expectedBase = hyp;
        }
    }
}


void confToLikelihood(N44Matrix confMat, N44Matrix& logLikelihood) {
    assert(logLikelihood.getSize() == confMat.getSize());
    
    vector<vector<int>> total(confMat.getSize());
    for (auto &i: total) i.resize(4);
    
    for (unsigned i = 0; i < confMat.getSize(); i++) {
        for (short b1=0; b1 < 4; b1++) {
            for (short b2 = 0; b2 < 4; b2++) {
                confMat(i, b1, b2) += 1;
                total[i][b2] += confMat(i, b1, b2);
            }
        }
    }
    
    //normalize and take the log
    for (unsigned i=0; i < logLikelihood.getSize(); i++) {
        for (short b1=0; b1 < 4; b1++) {
            for (short b2=0; b2 < 4; b2++) {
                logLikelihood(i, b1, b2) = log(confMat(i, b1, b2)) - log(total[i][b2]);
            }
        }
    }
}


//returns starting position of overlap for both sequences + size of overlap
void getOverlapInfo(ULL seqId, Neighb neighbInfo, const RandomisedAccess& access,
                    unsigned& seqPos, unsigned& neighbPos, unsigned& overlapSize) {
    
    Interpreter* seq = access[seqId];
    Interpreter* neighb = access[get<0>(neighbInfo)];
    
    if (get<1>(neighbInfo) >= 0) { //offset
        seqPos = static_cast<unsigned>(get<1>(neighbInfo));
        neighbPos = 0;
        overlapSize = min(seq->getLength() - seqPos, neighb->getLength());
    } else {
        seqPos = 0;
        neighbPos = static_cast<unsigned>(-get<1>(neighbInfo));
        overlapSize = min(seq->getLength(), neighb->getLength() - neighbPos);
    }
    
    delete seq;
    delete neighb;
}

void getOverlapInfo(Occur seqOccur, Occur neighbOccur, const RandomisedAccess& access,
                    unsigned& seqPos, unsigned& neighbPos, unsigned& overlapSize) {
    
    Interpreter* seq = access[seqOccur.first];
    Interpreter* neighb = access[neighbOccur.first];
    
    long long offset = static_cast<long long>(seqOccur.second) - neighbOccur.second;
    if (offset >= 0) {
        seqPos = static_cast<unsigned>(offset);
        neighbPos = 0;
        overlapSize = min(seq->getLength() - seqPos, neighb->getLength());
    } else {
        seqPos = 0;
        neighbPos = static_cast<unsigned>(-offset);
        overlapSize = min(seq->getLength(), neighb->getLength() - neighbPos);
    }
    
    delete seq;
    delete neighb;
}

vector<ULL> getOriginalOrder(const RandomisedAccess& access) {
    vector<ULL> origToRand(access.size()/2);
    
    for (ULL i=0; i<access.size(); i++) {
        if(access.isComplement(i)) continue;
        
        ULL origIndex = access.getOrigIndex(i);
        assert(origIndex < origToRand.size());
        
        origToRand[origIndex] = i;
    }
    
    return origToRand;
}
