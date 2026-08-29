#include "../h/MemoryAllocator.hpp"
#include "../h/RiscV.hpp"
#include "../h/TCB.hpp"
#include "../h/syscall_c.h"

extern void userMain();

int main() {
    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    TCB* kernel = TCB::createThread(
        nullptr,
        nullptr,
        mem_alloc(DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE)
    );
    kernel->setPrivilege(P_SUPERVISOR);

    TCB::running = kernel;

    userMain();

    delete kernel;

    return 0;
}