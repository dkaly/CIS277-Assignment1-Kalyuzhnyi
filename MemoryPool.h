#pragma once

#include <cstddef>
#include <vector>
#include "Stack.h"

class MemoryPool
{
public:
    MemoryPool(size_t blockSize, size_t blockCount);
    ~MemoryPool();

    void* allocate();
    bool deallocate(void* ptr);

    size_t availableBlocks() const;
    size_t allocatedBlocks() const;
    size_t blockSize() const;
    size_t capacity() const;

private:
    size_t blockSize_;
    size_t blockCount_;
    size_t allocated_;

    unsigned char* pool_;
    Stack<void*>   freeList_;
    std::vector<bool> inUse_;  
};