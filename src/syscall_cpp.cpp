//
// Created by os on 8/25/26.
//

#include "../h/syscall_cpp.hpp"

// THREAD

Thread::Thread(void(*body)(void*), void* arg) : myHandle(nullptr), body(body), arg(arg) {}
Thread::Thread() : myHandle(nullptr), body(nullptr), arg(nullptr) {}

Thread::~Thread() {}

void Thread::threadWrapper(void *threadPtr) {
    Thread* thread = (Thread*) threadPtr;

    if (thread->body) {
        thread->body(thread->arg);
    } else {
        thread->run();
    }

    thread_exit();
}

int Thread::start() {
    return thread_create(&myHandle, threadWrapper, (void*) this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t) {
    // need to implement
    return 0;
}

// SEMAPHORE

Semaphore::Semaphore(unsigned init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait_n(unsigned n) {
    return sem_wait_n(myHandle, n);
}

int Semaphore::signal_n(unsigned n) {
    return sem_signal_n(myHandle, n);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}