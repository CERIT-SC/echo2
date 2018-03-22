//
//  FastqSave.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 25/03/14.
//

#include "FastqSave.hpp"

void FastqSave::open(string fileName) {
    this->fileName = fileName;
    
    file.open(fileName);
    if (!file.good()) throw FastqSaveException("cannot_open_file");
}

void FastqSave::operator()(const vector<string>& sequences, const vector<string>& quality,
                           const vector<ULL>& order) {
    
    if (!file.good()) throw FastqSaveException("cannot_save_to_file");
    
    ULL id = 1;
    for (ULL orderIndex = 0; orderIndex < order.size(); orderIndex++) {
        ULL randIndex = order[orderIndex];
        
        assert(randIndex < sequences.size());
        assert(randIndex < quality.size());
        
        //structure of fastq file
        file << '@' << id++ << '\n';
        file << sequences[randIndex] << '\n';
        file << "+\n";
        file << quality[randIndex] << '\n';
        
        if (!file.good()) throw FastqSaveException("cannot_save_to_file");
    }
}

string FastqSave::getFileName() {
    return fileName;
}
