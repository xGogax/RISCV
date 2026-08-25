//
// Created by os on 8/21/26.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"
#include "../h/RiscV.hpp"
#include "../h/syscall_c.hpp"
#include "../h/workers.hpp"

namespace MemoryAllocatorTests {
    void runTests();
}

int main() {
    TCB* threads[5] = {};  // nullptr init, defanzivno

    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);

    // Ručno kreiramo "main thread" TCB, bez prolaska kroz thread_create()
    // koji odbija poziv kad je start_routine == nullptr
    __asm__ volatile("mv a4, %0" : : "r"(nullptr));      // stack = nullptr
    __asm__ volatile("mv a3, %0" : : "r"(nullptr));      // arg = nullptr
    __asm__ volatile("mv a2, %0" : : "r"(nullptr));      // body = nullptr
    __asm__ volatile("mv a1, %0" : : "r"(&threads[0]));  // handle
    __asm__ volatile("li a0, 0x11");
    __asm__ volatile("ecall");

    TCB::running = threads[0];

    thread_create(&threads[1], workerBodyA, nullptr);
    printString("Thread A created\n");
    thread_create(&threads[2], workerBodyB, nullptr);
    printString("Thread B created\n");
    thread_create(&threads[3], workerBodyC, nullptr);
    printString("ThreadC created\n");
    thread_create(&threads[4], workerBodyD, nullptr);
    printString("ThreadD created\n");

    MemoryAllocator::getInstance().printList();

    while (!(threads[1]->isFinished() && threads[2]->isFinished() && threads[3]->isFinished() && threads[4]->isFinished())) {
        thread_dispatch();
    }

    // threads[0] nema svoj heap-alociran stek, pa ga ne brišemo isto
    printString("All threads finished, cleaning up...\n");
    MemoryAllocator::getInstance().printList();
    for (int i = 1; i < 5; ++i) {
        delete threads[i];
        MemoryAllocator::getInstance().printList();
    }
    delete threads[0];
    MemoryAllocator::getInstance().printList();
    return 0;
}