#include <iostream>
#include <atomic>
#include <mutex>
#include "ThreadPool.h"
#include "MemoryPool.h"

int main() {
    const size_t NUM_THREADS = 4;
    const int NUM_TASKS = 50;

    ThreadPool pool(NUM_THREADS);
    MemoryPool<int> mp(16);

    std::mutex coutMutex;
    std::atomic<int> completed{0};

    for (int i = 0; i < NUM_TASKS; ++i) {
        pool.enqueue([i, &mp, &coutMutex, &completed]() {
            int* val = mp.allocate();
            *val = i * i;

            volatile int x = 0;
            for (int k = 0; k < 10000; ++k)
                x += k % 7;

            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Thread " << std::this_thread::get_id()
                          << " processed task " << i << " result=" << *val << "\n";
            }

            mp.deallocate(val);
            ++completed;
        });
    }

    pool.shutdown();

    std::cout << "All tasks done. Completed = " << completed.load() << "\n";
    return 0;
}