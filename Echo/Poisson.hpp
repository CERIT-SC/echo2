//
//  Poisson.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//

#ifndef __EchoErrorCorrection__Poisson_H__
#define __EchoErrorCorrection__Poisson_H__

#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <limits>
using namespace std;

void poissonDistEstimation(vector<double> distribution, unsigned& estimMean, double& variation);

vector<long double> poissonDist(unsigned len, unsigned mean);

#endif