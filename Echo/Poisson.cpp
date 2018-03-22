//
//  Poisson.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//

#include "Poisson.hpp"


void poissonDistEstimation(vector<double> distribution, unsigned& estimMean, double& variation) {
    
    //alter values to represent probabilities (division by sum)
    double sum = 0;
    for (auto i: distribution) sum += i;
    for (auto&i: distribution) i /= sum;
    
    //test indexes (as a mean) and choose one with lowest variation from Poisson distribution
    variation = numeric_limits<double>::infinity();
    estimMean = 0;
    for (int index = 0; index < distribution.size(); index++) {
        vector<long double> poiDist = poissonDist(static_cast<unsigned>(distribution.size()), index);
        
        double variationForIndex = 0;
        for (unsigned i=0; i < distribution.size(); i++) {
            variationForIndex += abs(poiDist[i] - distribution[i]);
        }
        
        if (variationForIndex < variation) {
            estimMean = index;
            variation = variationForIndex;
        }
    }
    
    //fix in case of distribution.size() == 0
    if (variation == numeric_limits<double>::infinity()) variation = 0;
}

vector<long double> poissonDist(unsigned len, unsigned mean) {
    vector<long double> dist(len);
    long double m = static_cast<long double>(mean);
    
    for(unsigned index = 0; index < dist.size(); index++) {
        dist[index] = 1/exp(m) * pow(m, static_cast<long double>(index));
        
        long double factorial = 1;
        for (unsigned i=2; i<=index; i++) factorial *= i;
        
        dist[index] /= factorial;
    }
    
    return dist;
}
