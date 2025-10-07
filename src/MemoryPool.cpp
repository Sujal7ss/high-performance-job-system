#include "MemoryPool.h"

template<typename T>
MemoryPool<T>::MemoryPool(size_t initialCapacity) {
    pool.reserve(initialCapacity);
    for (size_t i = 0; i < initialCapacity; ++i) {
        T* p = new T();
        pool.push_back(p);
        freeList.push(p);
    }
}

template<typename T>
T* MemoryPool<T>::allocate() {
    std::lock_guard<std::mutex> lock(mtx);
    if (freeList.empty()) {
        T* p = new T();
        pool.push_back(p);
        return p;
    } else {
        T* p = freeList.top();
        freeList.pop();
        return p;
    }
}

template<typename T>
void MemoryPool<T>::deallocate(T* obj) {
    std::lock_guard<std::mutex> lock(mtx);
    freeList.push(obj);
}

template<typename T>
MemoryPool<T>::~MemoryPool() {
    for (T* p : pool)
        delete p;
}
