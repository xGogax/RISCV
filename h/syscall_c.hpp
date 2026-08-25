//
// Created by os on 8/24/26.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP

#include "TCB.hpp"
#include "../lib/hw.h"

// Memory management
void* mem_alloc(size_t size);
int mem_free(void* ptr);

// Thread management
typedef TCB* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

// Semaphore management

// Console management


#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
