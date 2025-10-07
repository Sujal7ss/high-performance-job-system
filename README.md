# High-Performance Multithreaded Job System (C++17)

A lightweight job system built using modern C++17 features — showcasing multithreading, concurrency control, and custom memory management.

### 🚀 Features
- Thread pool with worker threads
- Task queue protected by mutex + condition variable
- Custom memory pool for efficient object allocation
- Atomic counters for synchronization

### 🧠 Concepts Demonstrated
- std::thread, std::mutex, std::condition_variable
- Concurrency, race condition avoidance
- Memory management and RAII principles
- Linux compilation and execution (`-pthread`)

### 🛠️ Build & Run
```bash
g++ -std=c++17 -pthread src/main.cpp src/ThreadPool.cpp -O2 -o job_system
./job_system
