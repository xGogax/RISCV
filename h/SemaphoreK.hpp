//
// Created by os on 8/26/26.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHOREK_HPP
#define PROJECT_BASE_V1_1_SEMAPHOREK_HPP

#include "list.hpp"
#include "TCB.hpp"

class SemaphoreK {
public:
    SemaphoreK(unsigned val) : s(val), closed(false) {}

    int signal();
    int wait();
    int signal_n(unsigned n);
    int wait_n(unsigned n);

    int close();

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
