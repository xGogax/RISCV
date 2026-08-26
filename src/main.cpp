#include "../h/MemoryAllocator.hpp"
#include "../h/RiscV.hpp"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/print.h"

sem_t sem;   // OVO mora biti iznad workerA i workerB

void workerA(void*) {
    printString("A: pre wait\n");

    sem_wait(sem);

    printString("A: usla u kriticnu sekciju\n");

    for (volatile int i = 0; i < 1000000; i++);

    printString("A: izlazi iz kriticne sekcije\n");

    sem_signal(sem);
}

void workerB(void*) {
    printString("B: pre wait\n");

    sem_wait(sem);

    printString("B: usla u kriticnu sekciju\n");

    for (volatile int i = 0; i < 1000000; i++);

    printString("B: izlazi iz kriticne sekcije\n");

    sem_signal(sem);
}

int main() {
    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    TCB* kernel = TCB::createThread(
        nullptr,
        nullptr,
        mem_alloc(DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE)
    );

    TCB::running = kernel;

    if (sem_open(&sem, 1) != 0) {
        printString("GRESKA: sem_open\n");
        return -1;
    }

    printString("Semaphore otvoren\n");

    Thread* t1 = new Thread(workerA, nullptr);
    Thread* t2 = new Thread(workerB, nullptr);

    t1->start();
    t2->start();

    while (Scheduler::getSize() > 0) {
        Thread::dispatch();
    }

    printString("Sve niti zavrsile\n");

    sem_close(&sem);

    delete t1;
    delete t2;
    delete kernel;

    return 0;
}