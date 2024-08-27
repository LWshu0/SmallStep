#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <queue>
#include <functional>
#include <atomic>

class ThreadPool
{
private:
    std::list<std::thread> m_workerThreads;

    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mtx;
    std::condition_variable m_cv;

    std::atomic<bool> m_stopFlag;

public:
    ThreadPool(size_t numThreads);

    void addTask(std::function<void()> func);

    ~ThreadPool();

private:
    void workerThread();
};

#endif