#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this]() { return stop.load() || !tasks.empty(); });
                    if (stop.load() && tasks.empty())
                        return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (stop.load())
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.push(std::move(job));
    }
    condition.notify_one();
}

void ThreadPool::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop.store(true);
    }
    condition.notify_all();
    for (auto& t : workers)
        if (t.joinable())
            t.join();
}

ThreadPool::~ThreadPool() {
    if (!stop.load())
        shutdown();
}
