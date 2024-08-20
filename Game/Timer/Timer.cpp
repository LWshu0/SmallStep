#include "Timer.h"

Timer::Timer() :
    m_callback(nullptr),
    m_callbackTime(0),
    m_callbackInterval(0),
    m_callbackCount(0)
{}

Timer::Timer(const std::function<void(void)>& callback, uint64_t now_ms, uint64_t timeout, int64_t count)
{
    setCallback(callback);
    setTimeout(now_ms, timeout, count);
}

bool Timer::update(uint64_t now_ms)
{
    if (now_ms < m_callbackTime) return false;
    // 执行一次回调
    m_callback();
    if (m_callbackCount > 0) m_callbackCount--;
    m_callbackTime = now_ms + m_callbackInterval;
    return true;
}

Timer::~Timer()
{}
