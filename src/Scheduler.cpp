//
// Created by os on 8/25/26.
//

#include "../h/Scheduler.hpp"

#include "../h/print.h"

List<TCB> Scheduler::readyThreadQueue;

TCB* Scheduler::get() {
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb) {
    readyThreadQueue.addLast(tcb);
}