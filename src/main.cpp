//
// Created by os on 8/21/26.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"
#include "../h/RiscV.hpp"
#include "../h/syscall_c.hpp"

namespace MemoryAllocatorTests {
    void runTests();
}

int main() {
    RiscV::w_stvec((uint64) &RiscV::supervisorTrap);

    printString("A");
    printString("NIGGER");

    void* x = mem_alloc(4);
    void* y = mem_alloc(10);
    void* z = mem_alloc(4);

    mem_free(y);

    void* k = mem_alloc(7);

    mem_free(x);
    mem_free(z);
    mem_free(k);

    printString("ASDSAD");
    return 0;
}