//
// Created by os on 8/25/26.
//

#include "../h/TCB.hpp"

#include "../h/RiscV.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB* TCB::createThread(Body body, void* arg, void* stack) {
    return new TCB(body, arg, stack);
}

int TCB::thread_exit() {
    if (running == nullptr) return -1;

    running->setFinished(true);
    TCB::yield();

    return 0;
}

void TCB::dispatch() {
    TCB* old = running;
    if(old && !old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();
    if (running && running != old) TCB::contextSwitch(&old->context, &running->context);
}

void TCB::yield() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}

void TCB::threadWrapper() {
    RiscV::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}