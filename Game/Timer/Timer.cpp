#include "Timer.h"

Timer::Timer() :
    m_callback(nullptr),
    m_callbackTime(0),
    m_callbackInterval(-1),
    m_callbackRepeatCount(1),
    m_isRunning(false),
    m_isTriggerOnStart(false)
{}

Timer& Timer::setCallback(const std::function<void(void)>& callback)
{
    assert(callback != nullptr);
    m_callback = callback;
    return *this;
}

Timer& Timer::setInterval(uint64_t interval)
{
    m_callbackInterval = interval;
    return *this;
}

Timer& Timer::setRepeat(uint64_t repeat_count)
{
    m_callbackRepeatCount = repeat_count;
    return *this;
}

Timer& Timer::setTriggerOnStart(bool trigger_on_start)
{
    m_isTriggerOnStart = trigger_on_start;
    return *this;
}

void Timer::start(int64_t now_ms)
{
    if (nullptr == m_callback || m_callbackInterval <= 0)
    {
        stop();
        return;
    }
    if (m_isTriggerOnStart)
    {
        m_callback();
        m_isTriggerOnStart = false;
    }
    m_isRunning = true;
    m_callbackTime = now_ms + m_callbackInterval;
}

void Timer::stop()
{
    m_isRunning = false;
    m_callbackRepeatCount = 0;
}

bool Timer::update(int64_t now_ms)
{    
    if (!running() || dead() || !timeout(now_ms)) return false;
    // 执行一次回调
    m_callback();
    if (m_callbackRepeatCount > 0) m_callbackRepeatCount--;
    m_callbackTime = now_ms + m_callbackInterval;
    return true;
}

Timer::~Timer()
{}
