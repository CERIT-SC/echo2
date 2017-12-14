//
//  SequenceFileLoader.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 10/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#ifndef __EchoErrorCorrection__SequenceFileLoader__
#define __EchoErrorCorrection__SequenceFileLoader__

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>

#include "Sequence.hpp"
#include "MessageLogger.hpp"

/*
 Exceptions:
 Exception class SequenceException
 SequenceException::what() returns:
 cannot_open_file
 incorrect_fastq_signature
 incorrect_base_value
 
 */

//BUGS:
//inputFile does not open file without extension


class SequenceFileLoader {
    ifstream inputFile;
    
public:
    SequenceFileLoader();
    void load(string inputFileName, vector<Sequence>& seqArray);
    
private:
    void loadStandardFile(vector<Sequence>& seqArray);
    void loadFastqFile(vector<Sequence>& seqArray);
};



//EXCEPTIONS
class LoaderException : public std::runtime_error {
public:
    LoaderException(string s) : runtime_error(s) {}
};

#endif /* defined(__EchoErrorCorrection__SequenceFileLoader__) */
