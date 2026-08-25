//
// Created by os on 8/25/26.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
private:
    static List<TCB> readyThreadQueue;
public:
    static TCB *get();
    static void put(TCB* tcb);
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
