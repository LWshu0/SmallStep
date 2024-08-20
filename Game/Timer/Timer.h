#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <chrono>
#include <functional>
#include <cassert>
#include <iostream>

class Timer {
private:
    std::function<void(void)> m_callback;

    uint64_t m_callbackTime;
    uint64_t m_callbackInterval;
    int64_t m_callbackCount;

public:
    Timer();
    Timer(const std::function<void(void)>& callback, uint64_t now_ms, uint64_t timeout, int64_t count = 1);
    
    inline void setCallback(const std::function<void(void)>& callback)
    {
        assert(callback != nullptr);
        m_callback = callback;
    }

    /**
     *@brief 设置 timeout 毫秒后执行一次, 执行 count 次后销毁定时器
     *
     * @param timeout 定时时间(毫秒), timeout 时间后执行回调函数一次
     * @param count 执行回调 count 次后销毁定时器, 如果 count 为负数, 将永远循环下去
     */
    inline void setTimeout(uint64_t now_ms, uint64_t timeout, int64_t count = 1)
    {
        m_callbackTime = now_ms + timeout;
        m_callbackInterval = timeout;
        m_callbackCount = count;
    }

    inline bool isFinish() const
    {
        return m_callbackCount == 0;
    }

    inline bool isEarlyThan(const Timer* other) const
    {
        assert(other != nullptr);
        return this->m_callbackTime < other->m_callbackTime;
    }
    inline bool isEarlyThan(const Timer& other) const
    {
        return this->m_callbackTime < other.m_callbackTime;
    }

    inline bool isLaterThan(const Timer* other) const
    {
        assert(other != nullptr);
        return this->m_callbackTime > other->m_callbackTime;
    }
    inline bool isLaterThan(const Timer& other) const
    {
        return this->m_callbackTime > other.m_callbackTime;
    }

    inline bool isSameTime(const Timer* other) const
    {
        assert(other != nullptr);
        return this->m_callbackTime == other->m_callbackTime;
    }
    inline bool isSameTime(const Timer& other) const
    {
        return this->m_callbackTime == other.m_callbackTime;
    }

    bool update(uint64_t now_ms);


    ~Timer();
};


#endif