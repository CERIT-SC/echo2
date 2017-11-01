//
//  SequenceFileLoader.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 10/09/13.
//  Copyright (c) 2013 Miloš Šimek. All rights reserved.
//

#include "SequenceFileLoader.hpp"

SequenceFileLoader::SequenceFileLoader() {}

void SequenceFileLoader::load(string inputFileName, vector<Sequence>& seqArray) {
    inputFile.open(inputFileName);
    
    try {
        //test of input file
        if (!inputFile.is_open()) {
            throw LoaderException("cannot_open_file");
        }
        
        //load appropriate type of file
        size_t pos = inputFileName.rfind('.');
        if (pos == string::npos) { //not found
            loadStandardFile(seqArray);
            return;
        }
        
        string extension = inputFileName.substr(pos);
        if (extension == ".fastq" || extension == ".fq") {
            loadFastqFile(seqArray);
        } else {
            loadStandardFile(seqArray);
        }
        
    } catch (...) {
        inputFile.close();
        throw;
    }
    
    inputFile.close();
}

void SequenceFileLoader::loadStandardFile(vector<Sequence>& seqArray) {
    string seq;
    
    try {
        while (!inputFile.eof()) {
            inputFile >> seq;
            if(seq.size()) seqArray.push_back(Sequence(seq));
            inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
            seq.clear(); //fixes possible issue with last sequence (could be doubled if there is newline after last sequence)
        }
        
    } catch (SequenceException& e) {
        throw LoaderException(e.what());
    }
}

void SequenceFileLoader::loadFastqFile(vector<Sequence>& seqArray) {
    string seq;
    streamsize unlimited = numeric_limits<streamsize>::max();
    char ch = 0;
    
    try {
    while (!inputFile.eof()) {
        //line 1
        inputFile.ignore(unlimited, '\n');
        
        //line 2
        inputFile >> seq;
        if(seq.size()) seqArray.push_back(Sequence(seq));
        inputFile.ignore(unlimited, '\n');
        
        //line 3
        inputFile.get(ch);
        if (ch != '+') throw LoaderException("incorrect_fastq_signature");
        inputFile.ignore(unlimited, '\n');
        
        //line 4
        inputFile.ignore(unlimited, '\n');
        
        seq.clear(); //same as above
    }
        
    } catch (SequenceException& e) {
        throw LoaderException(e.what());
    }
}
