//
// Created by os on 8/21/26.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "../h/print.h"

struct BlockHeader {
        size_t totalBlocks;
        bool isFree;
        BlockHeader *next;
};

class MemoryAllocator {
public:
    static MemoryAllocator& getInstance();

    void* mem_alloc(size_t size); // 0x01
    int mem_free(void*);          // 0x02

    void printList();

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;
private:
    MemoryAllocator();

    BlockHeader *head;

    BlockHeader* bestFit(size_t neededBlocks);                  // used in mem_alloc
    void splitBlock(BlockHeader* block, size_t neededBlocks);   // used in mem_alloc

    void mergeWithPrevious(BlockHeader* block);                 // used in mem_free
    void mergeWithNext(BlockHeader* block);                     // used in mem_free
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP