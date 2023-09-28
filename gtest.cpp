#include "allocator.hpp"

#include "gtest/gtest.h"

#include <thread>

void threadAlloc1(pa::Allocator& allocator) {
    for (std::size_t i = 0; i < 10; ++i) {
        allocator.alloc();
    }
}

void threadAlloc2(pa::Allocator& allocator) {
    for (std::size_t i = 0; i < 10; ++i) {
        allocator.alloc();
    }
}

TEST(ThreadTest, SingleAllocation) {
    const std::size_t N_BLOCKS{ 20 }, BLOCK_SIZE{ 128 };

    pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
    alloc->init();

    std::thread th1(threadAlloc1, std::ref(*alloc));
    std::thread th2(threadAlloc2, std::ref(*alloc));

    th1.join();
    th2.join();
}

void thAllocFree1(pa::Allocator& allocator) {
    const size_t N_OPERATIONS = 10;
    void* addresses[N_OPERATIONS]{};
    size_t i = 0;
    while (i < N_OPERATIONS) {
        addresses[i] = allocator.alloc();
        ++i;
    }

    while (i) {
        allocator.free(addresses[--i]);
    }
}

void thAllocFree2(pa::Allocator& allocator) {
    const size_t N_OPERATIONS = 10;
    void* addresses[N_OPERATIONS]{};
    size_t i = 0;
    while (i < N_OPERATIONS) {
        addresses[i] = allocator.alloc();
        ++i;
    }

    while (i) {
        allocator.free(addresses[--i]);
    }
}

TEST(ThreadTest, SingleFree) {
    const std::size_t N_BLOCKS{ 20 }, BLOCK_SIZE{ 128 };

    pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
    alloc->init();

    std::thread th1(thAllocFree1, std::ref(*alloc));
    std::thread th2(thAllocFree2, std::ref(*alloc));

    th1.join();
    th2.join();
}

TEST(MemoryAlloc, CheckZeroBlocks) {
    const std::size_t N_BLOCKS{ 0 }, BLOCK_SIZE{ 128 };

    try {
        pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
    }
    catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << "Error has been caught." << std::endl;
    }
}

TEST(MemoryAlloc, CheckZeroBlockSize) {
    const std::size_t N_BLOCKS{ 20 }, BLOCK_SIZE{ 0 };

    try {
        pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
    }
    catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << "Error has been caught." << std::endl;
    }
}

TEST(MemoryAlloc, CheckCapacityOverflow) {
    const std::size_t N_BLOCKS{ pa::Allocator::maxSize() }, BLOCK_SIZE{500};

    try {
        pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
    }
    catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        std::cout << "Error has been caught." << std::endl;
    }
}

//TEST(MemoryAlloc, FullAllocator) {
//    const std::size_t N_BLOCKS{ 10 }, BLOCK_SIZE{ 128 };
//
//    pa::Allocator* alloc = new pa::Allocator(N_BLOCKS, BLOCK_SIZE);
//    alloc->init();
//    try {
//        for (std::size_t i = 0; i < 10; ++i) {
//            alloc->alloc();
//        }
//    }
//    catch (std::runtime_error& err) {
//        std::cout << err.what() << std::endl;
//        std::cout << "Error has been caught." << std::endl;
//    }
//}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
