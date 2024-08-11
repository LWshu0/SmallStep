#include "Timer/TimerManager.h"

TimerManager::TimerManager()
{}

void TimerManager::update(uint64_t now_ms)
{
    while (!m_timers.empty())
    {
        // 取得最早的定时器
        Timer* timer = m_timers.top();
        // 更新定时器
        if (timer->update(now_ms))
        {
            // 到达时间并执行了回调
            // 如果还未结束, 需要再放回队列中
            // 否则删除定时器
            m_timers.pop();
            if (!timer->isFinish())
            {
                m_timers.push(timer);
            }
            else
            {
                delete timer;
            }
        }
        else
        {
            // 最早的定时器还未到时间
            break;
        }
    }
}

TimerManager::~TimerManager()
{
    while (!m_timers.empty())
    {
        delete m_timers.top();
        m_timers.pop();
    }
}