#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : m_stopFlag(false)
{
    for (size_t i = 0; i < numThreads; ++i)
    {
        m_workerThreads.emplace_back([this] { this->workerThread(); });
    }
}

void ThreadPool::addTask(std::function<void()> func)
{
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_tasks.push(func);
    }
    m_cv.notify_one();
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_stopFlag = true;
    }
    m_cv.notify_all();
    for (std::thread& worker : m_workerThreads)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_cv.wait(lock, [this] { return m_stopFlag || !m_tasks.empty(); });
            if (m_stopFlag && m_tasks.empty()) return;
            task = m_tasks.front();
            m_tasks.pop();
        }
        task();
    }
}