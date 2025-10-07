#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <vector>
#include <stack>
#include <mutex>

template<typename T>
class MemoryPool {
public:
    explicit MemoryPool(size_t initialCapacity = 0);
    T* allocate();
    void deallocate(T* obj);
    ~MemoryPool();

private:
    std::vector<T*> pool;
    std::stack<T*> freeList;
    std::mutex mtx;
};

#include "MemoryPool.cpp" // include template implementation

#endif // MEMORYPOOL_H
