#pragma once

/**
 * @file allocator.hpp
 * @brief Thread-safe pool allocator, supports C++17 standard and higher.
 */

#include "stacklinkedlist.hpp"

#include <mutex>
#include <cstddef>

/// @namespace Pool Allocator namespace.
namespace pa {

    /**
     * @class Allocator
     * @brief It reserves memory pool once and allows to allocate blocks as
     * much as 'alloc' function called. After calling Allocator Constructor,
     * should be called 'init' function.
     */
    class Allocator final {
    private:
        using Block = StackLinkedList::Node;

        /// The product of the number of memory blocks and their size.
        std::size_t _capacity = 0;

        /// The number of allocated blocks in the current allocator state.
        std::size_t _used = 0;

        /// The peak usage of the allocator.
        std::size_t _peak = 0;

        /// The size of one memory block.
        std::size_t _blockSize = 0;

        /// The first pointer of reserved memory sequence.
        void* _start = nullptr;

        /// Available memory blocks to be allocated.
        StackLinkedList _freeList{};

        std::mutex allocMx;
        std::mutex freeMx;
        std::mutex initMx;
        std::mutex resetMx;

    public:
        /**
         * @brief Explicitly allocates the specific number of memory blocks
         * and their size. It can throw std::runtime_error if try to allocate
         * zero amount of blocks or zero size of them. If the whole sequence of
         * blocks is too big it may throw overflow exception.
         * @param nBlocks The number of reserved in pool blocks.
         * @param blockSize The size of block in bits.
         */
        explicit Allocator(const std::size_t nBlocks, 
                           const std::size_t blockSize);

        /// @brief Allocator can't be copy constructed.
        Allocator(const Allocator& rhs) = delete;

        /// @brief Allocator can't be move constructed.
        Allocator(Allocator&& rhs) noexcept = delete;

        /// @brief Allocator can't be copied.
        Allocator* operator=(const Allocator& rhs) = delete;

        /// @brief Allocator can't be moved.
        Allocator* operator=(Allocator&& rhs) noexcept = delete;

        /// @brief Calls 'free' function to deallocate the memory pool.
        ~Allocator();

        /**
         * @brief Allocates the memory nBlocks times. Allocation of more then
         * nBlocks make it throw a std::runtime_error.
         * @return Returns the popped free position in reserved memory list.
         */
        void* alloc();

        /**
         * @brief Puts back allocated memory to the free list.
         * @param ptr Takes pointer which points to the result of 'alloc'
         * function.
         */
        void free(void* ptr);

        /**
         * @brief Supposed to be called after constructor if class invariant
         * was made successfully. It call OS to make a memory pool. It may
         * throw a std::runtime_error if OS doesn't succeed in memory
         * allocation.
         */
        void init();

        /**
         * @brief Resets the class invariant. Automatically called after
         * 'init' function.
         */
        void reset();

        /// Returns the maximum capacity of the allocator.
        static std::size_t maxSize();
    };
} // pa - pool allocator namespace