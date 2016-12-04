//
//  Poisson.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 01/07/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
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

//void poissonTotalVar(vector<double> histogram, unsigned& estMean, double& estTotalVar);
void poissonDistEstimation(vector<double> distribution, unsigned& estimMean, double& variation);

vector<long double> poissonDist(unsigned len, unsigned mean);

#endif