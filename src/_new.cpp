//
// Created by os on 8/24/26.
//

#include "../h/MemoryAllocator.hpp"

void *operator new(uint64 n) {
    return MemoryAllocator::getInstance().mem_alloc(n);
}

void *operator new[](uint64 n) {
    return MemoryAllocator::getInstance().mem_alloc(n);
}

void operator delete(void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}

void operator delete[](void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}