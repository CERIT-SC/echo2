//
//  HashComputationTests.cpp
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 08/02/14.
//

#include "HashComputationTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(HashComputationTests);

HashComputationTests::HashComputationTests() {
    //create path to testing files
    path = __FILE__;
    size_t end = path.find_last_of("/") + 1;
    path = path.substr(0, end);
    path += "TestSuiteFiles/";
}

void HashComputationTests::setUp() {
    loader = new SequenceFileLoader;
    seqArray.clear();
    loader->load(path + "input.txt", seqArray);
    
    access = new RandomisedAccess(seqArray);
    
    computation = new HashComputation(*access, 76, 20);
    
    hashTable1 = HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>);
    hashTable3 = HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>);
    hashTable4 = HashPtr(new HashKmerOccTable_Impl<KmerOcc_UnsChar>);
    hashTable1->setSize(1024);
    hashTable3->setSize(1024);
    hashTable4->setSize(1024);
    
    computation->operator()(hashTable1, 1);
    computation->operator()(hashTable3, 3);
    computation->operator()(hashTable4, 4);
    
    tables = {hashTable1, hashTable3, hashTable4};
}

void HashComputationTests::tearDown() {
    delete loader;
    delete access;
    delete computation;
}

//id's has to be unique for each hash in table
void HashComputationTests::uniqueIdTest() {
    for (auto hashTable: tables) {
        
        for (unsigned hash = 0; hash < 1024; hash++) {
            if(hash == 50) hash = 500;
            if(hash == 510) hash = 1000;
            for (unsigned index = 0; index < hashTable->getOccurrCount(hash); index++) {
                for (unsigned index2 = index+1; index2 < hashTable->getOccurrCount(hash); index2++) {
                    
                    ULL id1 = hashTable->getOccurr(hash, index).first;
                    ULL id2 = hashTable->getOccurr(hash, index2).first;
                    
                    CPPUNIT_ASSERT(id1 != id2);
                }
            }
        }
    }
}

//position of k-mer must stay within bounds of sequence [0, length-kmer_size]
void HashComputationTests::validPositionTest() {
    
    for (auto hashTable: tables) {
        
        for (unsigned hash = 0; hash < 1024; hash++) {
            for (unsigned index = 0; index < hashTable->getOccurrCount(hash); index++) {
                unsigned position = hashTable->getOccurr(hash, index).second;
                
                CPPUNIT_ASSERT(position <= 76-20);
            }
        }
    }
}

//all id's in table has to be valid
void HashComputationTests::allIdFromTableTest() {
    for (auto hashTable: tables) {
        
        for (unsigned hash = 0; hash < 1024; hash++) {
            for (unsigned index = 0; index < hashTable->getOccurrCount(hash); index++) {
                ULL id = hashTable->getOccurr(hash, index).first;
                
                CPPUNIT_ASSERT(id < access->size());
            }
        }
    }
}

//all sequences must have occurrence in table
void HashComputationTests::allIdInTableTest() {
    for (auto hashTable: tables) {
        set<ULL> ids;
        for(unsigned i=0; i<access->size();i++) ids.insert(i);
        
        for (unsigned hash = 0; hash < 1024; hash++) {
            for (unsigned index = 0; index < hashTable->getOccurrCount(hash); index++) {
                ULL id = hashTable->getOccurr(hash, index).first;
                
                ids.erase(id);
            }
        }
        
        CPPUNIT_ASSERT(ids.size() == 0);
    }
}

//sequence has to be in table under all it's hashes
void HashComputationTests::seqUnderAllTest() {
    //computes all hashes for sequence and checks if exists occurrence under particular
    //hash in table, that have sequence's id
    
    for (auto hashTable: tables) {
        for(unsigned seqId=0; seqId < access->size(); seqId++) {
            
            Interpreter* seq = access->operator[](seqId);
            char seqVect[seq->getLength()]; unsigned pos = 0;
            transform(seqVect, seqVect+seq->getLength(), seqVect, [&](char){return seq->at(pos++);});
            delete seq;
            
            for(unsigned pos=0; pos <= 76-20; pos++) {
                unsigned h = hashFunction(seqVect+pos, seqVect+pos+20);
                h &= 1023;
                
                bool found = false;
                for (unsigned index=0; index < hashTable->getOccurrCount(h); index++) {
                    if (hashTable->getOccurr(h, index).first == seqId) {found = true; continue;}
                }
                
                CPPUNIT_ASSERT(found == true);
            }
        }
    }
}

//every occurrence from table has to have valuable data
void HashComputationTests::occurrenceCorrectnessTest() {
    //this test goes from opossite direction than previous -
    //goes through every occurrence and checks, that it contains correct data
    //takes occurrence finds it's sequence and checks, that hash is computed from given
    //position
    
    for (auto hashTable: tables) {
        for (unsigned hash = 0; hash < hashTable->getSize(); hash++) {
            for (unsigned index = 0; index < hashTable->getOccurrCount(hash); index++) {
                Occur occur = hashTable->getOccurr(hash, index);
                
                //get sequence data
                Interpreter* seq = access->operator[](occur.first);
                char seqVect[seq->getLength()]; unsigned pos = 0;
                transform(seqVect, seqVect+seq->getLength(), seqVect,
                          [&](char){return seq->at(pos++);});
                delete seq;
                
                //get sequence-position hash
                unsigned h = hashFunction(seqVect + occur.second, seqVect + occur.second + 20);
                h &= 1023;
                
                CPPUNIT_ASSERT_EQUAL(hash, h);
            }
        }
    }
}
