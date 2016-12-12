//
//  FastqSave.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 25/03/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__FastqSave__
#define __EchoErrorCorrection__FastqSave__

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "debug.h"
using namespace std;

typedef unsigned long long ULL;

//EXCEPTIONS:
//saving to file can return FastqSaveException with what message:
//cannot_open_file - is thrown when file cannot be opened (thrown by open)
//cannot_save_to_file - is thrown when saving to file cannot continue (thrown by operator())

class FastqSave {
    ofstream file;
    string fileName;
    
public:
    FastqSave() {}
    
    void open(string fileName);
    void operator()(const vector<string>& sequences,
                    const vector<string>& quality,
                    const vector<ULL>& order);
    
    string getFileName();
};

class FastqSaveException : public std::runtime_error {
public:
    FastqSaveException(string s) : runtime_error(s) {}
};

#endif /* defined(__EchoErrorCorrection__FastqSave__) */
