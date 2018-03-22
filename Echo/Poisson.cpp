//
//  Poisson.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//

#include "Poisson.hpp"


//void poissonTotalVar(vector<double> histogram, unsigned& estMean, double& estTotalVar) {
//    //divide each element by histogram sum
//    double sum = accumulate(histogram.begin(), histogram.end(), 0.0);
//    transform(histogram.begin(), histogram.end(), histogram.begin(),
//              [=](double el) { return el/sum; });
//    
//    //get index of max element
//    unsigned mode = 0; double maxVal = -numeric_limits<double>::infinity();
//    for(unsigned index = 2; index < histogram.size(); index++) {
//        if (histogram[index] > maxVal) {
//            maxVal = histogram[index];
//            mode = index;
//        }
//    }
//    
//    estMean = mode+1;
//    estTotalVar = 1;
//    
//    for (unsigned m = mode-2; m <= mode+2; m++) {
//        vector<long double> poisson = poissonDist(static_cast<unsigned>(histogram.size()), m);
//        long double residualMass = 1.0L - accumulate(poisson.begin(), poisson.end(), 0.0L);
//        
//        //find variation between poisson and histogram distributions
//        long double totalVariation = 0;
//        auto h = histogram.begin(); auto p = poisson.begin();
//        for (; h != histogram.end(); h++, p++) totalVariation += abs(*h - *p);
//        totalVariation = 0.5*totalVariation + 0.5*residualMass;
//        
//        if (totalVariation < estTotalVar) {
//            estTotalVar = totalVariation;
//            estMean = m;
//        }
//    }
//}

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
