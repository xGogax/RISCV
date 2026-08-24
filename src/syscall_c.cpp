//
// Created by os on 8/24/26.
//

#include "../h/syscall_c.hpp"

#include "../h/print.h"

void* mem_alloc(size_t size) {
    if (size == 0) return nullptr;

    __asm__ volatile("mv a1, %0" : : "r"(size));
    __asm__ volatile("li a0, 0x01");

    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    printString("Uspeli smo, ret: "); printInteger((uint64)ret); printString("\n");
    printString("-------------------------------\n");

    return ret;
}

int mem_free(void* ptr) {
    if (!ptr) return 0;

    __asm__ volatile ("mv a1, %0" : : "r"(ptr));
    __asm__ volatile ("li a0, 0x02");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    printString("-------------------------------\n");

    return ret;
}