#include "MemoryPool.h"

#include <cstring>
#include <iostream>

static void printHeader(const MemoryPool& pool) {
    std::cout << "Network Packet Buffer Pool\n\n";
    std::cout << "Block Size:       " << pool.blockSize()  << " bytes\n";
    std::cout << "Blocks:           " << pool.capacity()   << '\n';
    std::cout << "Total Capacity:   "
              << pool.blockSize() * pool.capacity()       << " bytes\n\n";
}

static void printStats(const MemoryPool& pool) {
    std::cout << "Available blocks: " << pool.availableBlocks() << '\n';
    std::cout << "Allocated blocks: " << pool.allocatedBlocks() << "\n\n";
}

int main() {
    MemoryPool pool(512, 8);            //8 blocks × 512 bytes = 4096

    printHeader(pool);

    //allocate three packets
    void* p1 = pool.allocate();
    void* p2 = pool.allocate();
    void* p3 = pool.allocate();

    std::cout << "Packet 1 allocated: " << p1 << '\n';
    std::cout << "Packet 2 allocated: " << p2 << '\n';
    std::cout << "Packet 3 allocated: " << p3 << "\n\n";

    printStats(pool);

    //write binary data into packet 1
    unsigned char packet[] = {
        0x45, 0x00, 0x00, 0x3C,
        0xAB, 0xCD, 0x12, 0x34
    };
    std::memcpy(p1, packet, sizeof(packet));        //8 bytes ≤ 512
    std::cout << "Binary packet written to Packet 1.\n\n";

    // verify the bytes were stored correctly
    unsigned char readback[8];
    std::memcpy(readback, p1, sizeof(readback));      // READ back from the block
    if (std::memcmp(readback, packet, sizeof(packet)) == 0) {
        std::cout << "Binary packet verified in Packet 1.\n\n";
    } else {
        std::cout << "Binary packet verification FAILED.\n\n";
    }

    //release packet 2
    pool.deallocate(p2);
    std::cout << "Packet 2 released.\n\n";

    printStats(pool);

    //allocate packet 4, should reuse the block packet 2 had
    void* p4 = pool.allocate();
    std::cout << "Packet 4 allocated: " << p4 << "\n\n";

    if (p4 == p2) {
        std::cout << "Packet 4 reused the previously released block.\n\n";
    } else {
        std::cout << "Packet 4 did NOT reuse the block (unexpected).\n\n";
    }

    //exhaust the pool
    std::cout << "Attempting to exhaust pool...\n\n";
    while (pool.allocate() != nullptr) {
        //drain
    }

    std::cout << "No blocks available.\n";
    std::cout << "allocate() returned " << pool.allocate() << ".\n\n";

    //attempt double deallocation
    std::cout << "Attempting double deallocation...\n\n";
    pool.deallocate(p1);                 //first free — succeeds
    bool second = pool.deallocate(p1);   //second free — should fail

    if (!second) {
        std::cout << "Double deallocation rejected.\n";
    } else {
        std::cout << "Double deallocation was NOT rejected (unexpected).\n";
    }

    (void)p3;   //silence unused warning
    return 0;
}