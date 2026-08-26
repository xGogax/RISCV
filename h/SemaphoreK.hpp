//
// Created by os on 8/26/26.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHOREK_HPP
#define PROJECT_BASE_V1_1_SEMAPHOREK_HPP

#include "list.hpp"
#include "TCB.hpp"

class SemaphoreK {
public:
    SemaphoreK(unsigned val) : s(val), closed(false) {} // 0x21

    int signal();               // 0x24
    int wait();                 // 0x23
    int signal_n(unsigned n);   // 0x26
    int wait_n(unsigned n);     // 0x25

    int close();                // 0x22

private:
    int s;
    bool closed;
    List<TCB> blockedQueue;

    SemaphoreK(const SemaphoreK&) = delete;
    SemaphoreK& operator=(const SemaphoreK&) = delete;

    void block();
    void unblock();
};

#endif
