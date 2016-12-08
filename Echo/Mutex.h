//
//  Mutex.h
//  EchoErrorCorrection
//
//  Created by Miloš Šimek on 11/02/14.
//  Copyright (c) 2014 Miloš Šimek. All rights reserved.
//

#ifndef EchoErrorCorrection_Mutex_h
#define EchoErrorCorrection_Mutex_h

#include <atomic>
using namespace std;

//std::mutex is not used for it's memory requirements (64B)
//atomic<bool> has 1B

class Mutex {
    atomic<bool> access;
    
public:
    inline Mutex() : access(true) {}
    inline Mutex(const Mutex&) { access.store(true); }
    
    inline void lock() {
        bool val = true;
        while (!access.compare_exchange_strong(val, false)) val = true;
    }
    inline void unlock() { access.store(true); }
};

#endif
