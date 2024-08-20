#include "Clock.h"

Clock::Clock() :
    m_configTimeFactor(1.0),
    m_factor(1.0),
    m_lastUpdateRealTime(now_milliseconds()),
    m_lastPauseRealTime(0),
    m_lastUpdateGameTime(0),
    m_deltaGameTime(0)
{}

void Clock::updateTime()
{
    m_spinlock.lock();
    if (isPause()) return;
    int64_t now_ms = now_milliseconds();
    m_deltaGameTime = (now_ms - m_lastUpdateRealTime) * m_factor;
    m_lastUpdateGameTime += m_deltaGameTime;
    m_lastUpdateRealTime = now_ms;
    m_spinlock.unlock();
}

void Clock::pause()
{
    m_spinlock.lock();
    if (isPause()) return;
    m_factor = 0.0;
    m_lastPauseRealTime = now_milliseconds();
    m_spinlock.unlock();
}

void Clock::start()
{
    m_spinlock.lock();
    if (!isPause()) return;
    m_factor = m_configTimeFactor;
    int64_t offset_ms = m_lastPauseRealTime - m_lastUpdateRealTime;
    m_lastUpdateRealTime = now_milliseconds() - offset_ms;
    m_spinlock.unlock();
}

void Clock::reset()
{
    m_spinlock.lock();
    m_lastUpdateRealTime = now_milliseconds();
    m_lastUpdateGameTime = 0;
    m_deltaGameTime = 0;
    setFactor(1.0);
    m_spinlock.unlock();
}

Clock::~Clock()
{}
