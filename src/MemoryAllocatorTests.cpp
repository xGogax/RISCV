#include "../h/MemoryAllocator.hpp"
#include "../h/print.h"

namespace MemoryAllocatorTests {

static void printTest(const char* name) {
    printString("\n==============================\n");
    printString(name);
    printString("\n==============================\n");
}

static void printResult(bool success) {
    if (success)
        printString("PASS\n");
    else
        printString("FAIL\n");
}


//
// TEST 1
// Jednostavna alokacija
//
static bool testBasicAlloc() {
    printTest("TEST 1 - BASIC ALLOCATION");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    void* p = allocator.mem_alloc(100);

    if (p == nullptr) {
        printResult(false);
        return false;
    }

    printString("Allocation successful!\n");

    allocator.printList();

    allocator.mem_free(p);

    printString("After free:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// TEST 2
// Split
//
static bool testSplit() {
    printTest("TEST 2 - SPLIT");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    void* p1 = allocator.mem_alloc(100);

    if (p1 == nullptr) {
        printResult(false);
        return false;
    }

    printString("After first allocation:\n");
    allocator.printList();

    void* p2 = allocator.mem_alloc(100);

    if (p2 == nullptr) {
        printResult(false);
        return false;
    }

    printString("After second allocation:\n");
    allocator.printList();

    allocator.mem_free(p1);
    allocator.mem_free(p2);

    printString("After freeing both:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// TEST 3
// Best Fit
//
static bool testBestFit() {
    printTest("TEST 3 - BEST FIT");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    //
    // Napravimo nekoliko zauzetih blokova:
    //
    // [A][B][C][D]
    //
    void* a = allocator.mem_alloc(100);
    void* b = allocator.mem_alloc(300);
    void* c = allocator.mem_alloc(100);
    void* d = allocator.mem_alloc(500);

    if (!a || !b || !c || !d) {
        printResult(false);
        return false;
    }

    printString("Initial state:\n");
    allocator.printList();

    //
    // Oslobodimo B i D.
    //
    // Dobijamo:
    //
    // [A][FREE 300][C][FREE 500]
    //
    allocator.mem_free(b);
    allocator.mem_free(d);

    printString("After freeing B and D:\n");
    allocator.printList();

    //
    // Sada trazimo 200 bajtova.
    //
    // Oba free prostora mogu da prime zahtev,
    // ali best fit treba da izabere manji.
    //
    void* x = allocator.mem_alloc(200);

    if (x == nullptr) {
        printResult(false);
        return false;
    }

    printString("After best-fit allocation:\n");
    allocator.printList();

    //
    // Oslobadjamo sve.
    //
    allocator.mem_free(a);
    allocator.mem_free(c);
    allocator.mem_free(x);

    printString("Final state:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// TEST 4
// Free + merge sa sledecim
//
static bool testMergeNext() {
    printTest("TEST 4 - MERGE NEXT");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    void* a = allocator.mem_alloc(100);
    void* b = allocator.mem_alloc(100);
    void* c = allocator.mem_alloc(100);

    if (!a || !b || !c) {
        printResult(false);
        return false;
    }

    printString("Initial:\n");
    allocator.printList();

    //
    // Oslobadjamo B i C.
    //
    // B i C treba da se spoje.
    //
    allocator.mem_free(b);

    printString("After freeing B:\n");
    allocator.printList();

    allocator.mem_free(c);

    printString("After freeing C:\n");
    allocator.printList();

    //
    // Na kraju oslobadjamo i A.
    //
    allocator.mem_free(a);

    printString("Final:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// TEST 5
// Merge sa prethodnim
//
static bool testMergePrevious() {
    printTest("TEST 5 - MERGE PREVIOUS");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    void* a = allocator.mem_alloc(100);
    void* b = allocator.mem_alloc(100);
    void* c = allocator.mem_alloc(100);

    if (!a || !b || !c) {
        printResult(false);
        return false;
    }

    printString("Initial:\n");
    allocator.printList();

    //
    // Prvo oslobodimo A.
    //
    allocator.mem_free(a);

    printString("After freeing A:\n");
    allocator.printList();

    //
    // Onda B.
    //
    // B treba da se spoji sa prethodnim A.
    //
    allocator.mem_free(b);

    printString("After freeing B:\n");
    allocator.printList();

    //
    // Na kraju C.
    //
    allocator.mem_free(c);

    printString("Final:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// TEST 6
// Alokacija nakon merge-a
//
static bool testReuse() {
    printTest("TEST 6 - REUSE FREE SPACE");

    MemoryAllocator& allocator =
        MemoryAllocator::getInstance();

    void* a = allocator.mem_alloc(100);
    void* b = allocator.mem_alloc(100);
    void* c = allocator.mem_alloc(100);

    if (!a || !b || !c) {
        printResult(false);
        return false;
    }

    printString("Initial:\n");
    allocator.printList();

    allocator.mem_free(b);

    printString("After freeing B:\n");
    allocator.printList();

    //
    // Ovaj zahtev treba da iskoristi slobodan prostor B.
    //
    void* x = allocator.mem_alloc(50);

    if (x == nullptr) {
        printResult(false);
        return false;
    }

    printString("After allocating 50 bytes:\n");
    allocator.printList();

    allocator.mem_free(a);
    allocator.mem_free(c);
    allocator.mem_free(x);

    printString("Final:\n");
    allocator.printList();

    printResult(true);
    return true;
}


//
// Glavna funkcija za testiranje
//
void runTests() {

    printString("\n");
    printString("====================================\n");
    printString("       MEMORY ALLOCATOR TESTS\n");
    printString("====================================\n");

    testBasicAlloc();
    testSplit();
    testBestFit();
    testMergeNext();
    testMergePrevious();
    testReuse();

    printString("\n");
    printString("====================================\n");
    printString("           TESTS FINISHED\n");
    printString("====================================\n");
}

}