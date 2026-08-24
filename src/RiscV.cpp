//
// Created by os on 8/24/26.
//

#include "../h/RiscV.hpp"

#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"

void RiscV::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void RiscV::handleSupervisorTrap() {

    // system code
    uint64 syscode;
    __asm__ volatile ("mv %0, a0" : "=r"(syscode));

    // args (MAX 4) - thread_create
    uint64 arg1, arg2, arg3, arg4;
    __asm__ volatile ("mv %0, a1" : "=r"(arg1));
    __asm__ volatile ("mv %0, a2" : "=r"(arg2));
    __asm__ volatile ("mv %0, a3" : "=r"(arg3));
    __asm__ volatile ("mv %0, a4" : "=r"(arg4));

    printString("syscode: "); printInteger(syscode); printString("\n");
    printString("arg1: "); printInteger(arg1); printString("\n");
    printString("arg2: "); printInteger(arg2); printString("\n");
    printString("arg3: "); printInteger(arg3); printString("\n");
    printString("arg4: "); printInteger(arg4); printString("\n");

    uint64 ret = 0;

    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // interrupt: NO
        // cause code: environment call from U-mode(8) or S-mode(9)

        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch (syscode) {
            case SYS_MEM_ALLOC: {
                // pretvaramo iz blokova nazad
                ret = (uint64) MemoryAllocator::getInstance().mem_alloc(arg1 * MEM_BLOCK_SIZE);
                MemoryAllocator::getInstance().printList();
                break;
            }

            case SYS_MEM_FREE: {
                ret = (uint64) MemoryAllocator::getInstance().mem_free((void*) arg1);
                MemoryAllocator::getInstance().printList();
                break;
            }

            case SYS_THREAD_CREATE: {
                break;
            }

            case SYS_THREAD_EXIT: {
                break;
            }

            case SYS_THREAD_DISPATCH: {
                break;
            }

            case SYS_TIME_SLEEP: {
                break;
            }

            case SYS_SEM_OPEN: {
                break;
            }

            case SYS_SEM_CLOSE: {
                break;
            }

            case SYS_SEM_WAIT: {
                break;
            }

            case SYS_SEM_SIGNAL: {
                break;
            }

            case SYS_SEM_WAIT_N: {
                break;
            }

            case SYS_SEM_SIGNAL_N: {
                break;
            }

            default: {
                printString("wrong code.\n");
                break;
            }
        }
        __asm__ volatile("mv a0, %0" : : "r"(ret) : "a0");

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else {
        // unexpected error
        printString("ERROR\n");
        printString("scause: "); printInteger(scause), printString("\n");
        printString("sepc:   "); printInteger(r_sepc()), printString("\n");
        printString("stval:  "); printInteger(r_stval()), printString("\n");
        printString("\nKernel stopped.\n");

        // emulator stop
        __asm__ volatile("li t0, 0x5555");
        __asm__ volatile("li t1, 0x100000");
        __asm__ volatile("sw t0, 0(t1)");
        return;
    }
}
