//
// Created by os on 8/21/26.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/RiscV.hpp"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/workers.hpp"

namespace MemoryAllocatorTests {
    void runTests();
}

int main() {
    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);

    // TCB koji predstavlja trenutnu (main/kernel) nit
    TCB* kernel = TCB::createThread(
        nullptr,
        nullptr,
        mem_alloc(DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE)
    );

    TCB::running = kernel;

    // Kreiranje C++ niti
    Thread* threads[4];

    threads[0] = new Thread(workerBodyA, nullptr);
    threads[1] = new Thread(workerBodyB, nullptr);
    threads[2] = new Thread(workerBodyC, nullptr);
    threads[3] = new Thread(workerBodyD, nullptr);

    // Startovanje niti
    threads[0]->start();
    threads[1]->start();
    threads[2]->start();
    threads[3]->start();

    MemoryAllocator::getInstance().printList();

    // Cekamo da se sve niti zavrse
    while (Scheduler::getSize() > 0) {
        if (threads[0] && threads[1] && threads[2] && threads[3]) {
            Thread::dispatch();
        }
    }

    for (Thread* thread : threads) {
        MemoryAllocator::getInstance().printList();
        delete thread;
    }
    delete kernel;
    MemoryAllocator::getInstance().printList();

    return 0;
}