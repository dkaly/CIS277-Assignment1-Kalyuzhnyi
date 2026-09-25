#include "MemoryPool.h"
#include <cstdint>

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount)
    : blockSize_(blockSize)
    , blockCount_(blockCount)
    , allocated_(0)
    , pool_(nullptr)
    , inUse_(blockCount, false)          //blocks start free
{
    pool_ = new unsigned char[blockSize_ * blockCount_];

    for (size_t i = 0; i < blockCount_; ++i) {
        freeList_.push(pool_ + i * blockSize_);
    }
}

MemoryPool::~MemoryPool() {
    delete[] pool_;
    pool_ = nullptr;
}

void* MemoryPool::allocate() {
    if (freeList_.empty()) {
        return nullptr;
    }
    void* block = freeList_.top();
    freeList_.pop();

    size_t index = (static_cast<unsigned char*>(block) - pool_) / blockSize_;
    inUse_[index] = true;
    ++allocated_;

    return block;
}

bool MemoryPool::deallocate(void* ptr) {
    if (ptr == nullptr) {
        return false;
    }

    unsigned char* p = static_cast<unsigned char*>(ptr);

    //must point inside the pool
    if (p < pool_ || p >= pool_ + blockSize_ * blockCount_) {
        return false;
    }

    //must be aligned exactly to a block boundary
    if ((p - pool_) % blockSize_ != 0) {
        return false;
    }

    size_t index = (p - pool_) / blockSize_;

    //block must currently be in use
    if (!inUse_[index]) {
        return false; 
    }

    inUse_[index] = false;
    freeList_.push(ptr);
    --allocated_;
    return true;
}

size_t MemoryPool::availableBlocks() const { return blockCount_ - allocated_; }
size_t MemoryPool::allocatedBlocks() const { return allocated_; }
size_t MemoryPool::blockSize()       const { return blockSize_; }
size_t MemoryPool::capacity()        const { return blockCount_; }