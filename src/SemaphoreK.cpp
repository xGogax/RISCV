//
// Created by os on 8/26/26.
//

#include "../h/SemaphoreK.hpp"

void SemaphoreK::block() {
    TCB::running->setBlocked(true);
    blockedQueue.addLast(TCB::running);
    TCB::dispatch();
}

void SemaphoreK::unblock() {
    TCB* thread = blockedQueue.removeFirst();
    if (thread == nullptr) return;
    thread->setBlocked(false);
    Scheduler::put(thread);
}

int SemaphoreK::wait() {
    return wait_n(1);
}

int SemaphoreK::signal() {
    return signal_n(1);
}

int SemaphoreK::wait_n(unsigned n) {
    if (closed || n == 0) return -1;

    if (s >= (int)n) {
        s -= n;
        return 0;
    }

    //else
    TCB::running->setSemWaitN(n);
    block();

    if (closed) return -1;
    return 0;
}

int SemaphoreK::signal_n(unsigned n) {
    if (closed || n == 0) return -1;

    s += n;

    while (blockedQueue.peekFirst()) {
        TCB* thread = blockedQueue.peekFirst();

        if (s < (int) thread->getSemWaitN()) break;

        s -= thread->getSemWaitN();
        thread->setSemWaitN(0);
        unblock();
    }

    return 0;
}

int SemaphoreK::close() {
    if (closed) return -1;

    closed = true;

    while (blockedQueue.peekFirst()) unblock();

    return 0;
}