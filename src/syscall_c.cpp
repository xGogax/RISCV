//
// Created by os on 8/24/26.
//

#include "../h/syscall_c.hpp"

#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"

void* mem_alloc(size_t size) {
    if (size == 0) return nullptr;

    __asm__ volatile("mv a1, %0" : : "r"(size));
    __asm__ volatile("li a0, 0x01");

    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int mem_free(void* ptr) {
    if (!ptr) return 0;

    __asm__ volatile ("mv a1, %0" : : "r"(ptr));
    __asm__ volatile ("li a0, 0x02");

    __asm__ volatile ("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));

    return ret;
}

int thread_create(thread_t* handle, void(*start_routine)(void *), void *arg) {
    if (!handle) { return -1; }
    if (!start_routine) { return -1; }

    void* stack = mem_alloc(DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE);
    if (!stack) {
        return -3;
    }

    __asm__ volatile("mv a4, %0" : : "r"(stack));
    __asm__ volatile("mv a3, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("li a0, 0x11");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_exit() {
    __asm__ volatile("li a0, 0x12");

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void thread_dispatch() {
    __asm__ volatile("li a0, 0x13");

    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    if (!handle) return -1;

    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("li a0, 0x21");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close(sem_t*handle) {
    if (!handle) return -1;

    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("li a0, 0x22");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait(sem_t id) {
    if (!id) return -1;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x23");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    if (!id) return -1;

    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x24");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait_n(sem_t id, unsigned n) {
    if (!id || n == 0) return -1;

    __asm__ volatile("mv a2, %0" : : "r"(n));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x25");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal_n(sem_t id, unsigned n) {
    if (!id || n == 0) return -1;

    __asm__ volatile("mv a2, %0" : : "r"(n));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("li a0, 0x26");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}