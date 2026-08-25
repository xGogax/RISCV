//
// Created by os on 8/25/26.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP
#include "Scheduler.hpp"
#include "../lib/hw.h"

enum ThreadPrivilege {
    P_USER,
    P_SUPERVISOR
};

class TCB {
public:
    ~TCB() { delete[] stack; }

    static TCB* running;

    using Body = void(*)(void *);

    static TCB* createThread(Body body, void* arg, void* stack);  // 0x11
    int thread_exit();                                            // 0x12
    static void dispatch();                                       // 0x13

    static void yield();

    bool isFinished() const { return finished; }
    void setFinished(bool val) { finished = val; }

    uint64 getTimeSlice() const { return timeSlice; }

    ThreadPrivilege getPrivilege() const { return privilege; }
    void setPrivilege(ThreadPrivilege val) { privilege = val; }

private:
    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;

    friend class RiscV;

    TCB(Body body, void* arg, void* stack) :
    body(body),
    arg(arg),
    stack((uint64*) stack),
    context({(uint64) &threadWrapper, this->stack != nullptr ? (uint64) &this->stack[STACK_SIZE] : 0}),
    timeSlice(TIME_SLICE),
    finished(false),
    privilege(P_USER) {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context {
        uint64 ra; // return address
        uint64 sp; // stack pointer
    };

    Body body;
    void* arg;
    uint64* stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    ThreadPrivilege privilege;

    static void contextSwitch(Context* oldContext, Context* newContext);    // contextSwitch.S
    static void threadWrapper();
};

#endif //PROJECT_BASE_V1_1_TCB_HPP
