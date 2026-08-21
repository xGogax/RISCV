//
// Created by os on 8/21/26.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

MemoryAllocator::MemoryAllocator() : head(nullptr) {
    head = (BlockHeader*) HEAP_START_ADDR;

    size_t totalBlocks = ((char*) HEAP_END_ADDR - (char*) HEAP_START_ADDR) / MEM_BLOCK_SIZE;

    head->totalBlocks = totalBlocks;
    head->isFree = true;
    head->next = nullptr;

    printString("Memory allocator initialized.\n");
}

BlockHeader* MemoryAllocator::bestFit(size_t neededBlocks) {
    BlockHeader* curr = head;
    BlockHeader* best = nullptr;

    while (curr != nullptr) {
        if (curr->isFree && curr->totalBlocks >= neededBlocks) {
            if (best == nullptr || curr->totalBlocks < best->totalBlocks) {
                best = curr;
            }
        }

        curr = curr->next;
    }

    return best;
}

void MemoryAllocator::splitBlock(BlockHeader* block, size_t neededBlocks) {
    auto* newBlock = (BlockHeader*)((char*)block + neededBlocks * MEM_BLOCK_SIZE);

    newBlock->totalBlocks = block->totalBlocks - neededBlocks;
    newBlock->isFree = true;
    newBlock->next = block->next;

    block->totalBlocks = neededBlocks;
    block->next = newBlock;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    if (size <= 0) { return nullptr; }

    size_t neededBlocks = (sizeof(BlockHeader) + size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    BlockHeader* best = bestFit(neededBlocks);
    if (best == nullptr) { return nullptr; } // no space

    if (best->totalBlocks > neededBlocks) splitBlock(best, neededBlocks);
    best->isFree = false;

    return (void*)((char*) best + sizeof(BlockHeader));
}

void MemoryAllocator::mergeWithNext(BlockHeader* block) {
    if (block->next != nullptr && block->next->isFree) {
        block->totalBlocks += block->next->totalBlocks;
        block->next = block->next->next;
    }
}

void MemoryAllocator::mergeWithPrevious(BlockHeader *block) {
    BlockHeader* prev = nullptr;
    BlockHeader* curr = head;

    while (curr != nullptr && curr != block) {
        prev = curr;
        curr = curr->next;
    }

    if (prev != nullptr && prev->isFree) {
        prev->totalBlocks += block->totalBlocks;
        prev->next = block->next;
    }
}

int MemoryAllocator::mem_free(void* ptr) {
    if (ptr == nullptr) { return -1; }

    // getting exact block
    auto* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));

    block->isFree = true;
    mergeWithNext(block);
    mergeWithPrevious(block);

    return 0;
}

void MemoryAllocator::printList() {
    BlockHeader* curr = head;

    printString("\n--- MEMORY LIST ---\n");

    while (curr != nullptr) {
        printString("[ ");

        if (curr->isFree) {
            printString("FREE");
        } else {
            printString("ALOC");
        }

        printString(" | blocks: ");
        printInteger(curr->totalBlocks);

        printString(" ] -> ");

        curr = curr->next;
    }
    printString("NULL\n");
}
