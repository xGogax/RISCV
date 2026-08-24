//
// Created by os on 8/21/26.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"
#include "../h/RiscV.hpp"

namespace MemoryAllocatorTests {
    void runTests();
}

int main() {
    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);

    printString("PRE ECALL\n");

    __asm__ volatile("ecall");

    printString("POSLE ECALL\n");

    return 0;
}