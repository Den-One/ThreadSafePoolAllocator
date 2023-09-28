#include "allocator.hpp"

#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <iostream>

pa::Allocator::Allocator(const std::size_t nBlocks, 
                              const std::size_t blockSize)
    : _capacity{ nBlocks * blockSize }, _blockSize{ blockSize }
{
    if (nBlocks <= 0) {
        throw std::runtime_error {"Blocks number must be more then zero."};
    }

    if (blockSize <= 0) {
        throw std::runtime_error {"Block size must be more then zero."};
    }

    if (_capacity / nBlocks != blockSize) {
        throw std::runtime_error {"Too many blocks of this size."};
    }
}

pa::Allocator::~Allocator() {
    free(_start);
}

void* pa::Allocator::alloc() {
    std::scoped_lock allocSL { allocMx, freeMx, initMx, resetMx };

    Block* freePosition = _freeList.pop();

    if (!freePosition) {
        throw std::runtime_error {"Allocator is full"};
    }

    _used += _blockSize;
    _peak = std::max(_peak, _used);

    std::cout << "A" << "\t@S " << _start << "\t@R " << (void*)freePosition
              << "\tM " << _used << std::endl;

    return (void*)freePosition;
}

void pa::Allocator::free(void* ptr) {
    std::scoped_lock allocSL { allocMx, freeMx, initMx, resetMx };

    _used -= _blockSize;
    _freeList.push((Block*)ptr);

    std::cout << "F" << "\t@S " << _start << "\t@F " << ptr
              << "\tM " << _used << std::endl;
}

void pa::Allocator::init() {
    std::scoped_lock allocSL { allocMx, freeMx, initMx, resetMx };

    _start = malloc(_capacity);
    if (_start == NULL) {
        throw std::runtime_error {"Malloc return NULL."};
    }
    allocSL.~scoped_lock();
    reset();
}

void pa::Allocator::reset() {
    std::scoped_lock allocSL { allocMx, freeMx, initMx, resetMx };

    _used = 0;
    _peak = 0;

    const int nBlocks = _capacity / _blockSize;
    for (std::size_t i = 0; i < nBlocks; ++i) {
        auto address = reinterpret_cast<std::size_t>(_start) + i * _blockSize;
        _freeList.push((Block*)address);
    }
}

std::size_t pa::Allocator::maxSize() {
    return std::numeric_limits<std::size_t>::max() / sizeof(Block*);
}