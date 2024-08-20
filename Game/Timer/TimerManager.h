#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <queue>
#include <vector>
#include "Timer.h"

class TimerManager {
private:
    TimerManager();
public:
    TimerManager(TimerManager const&) = delete;
    void operator=(TimerManager const&) = delete;

    static TimerManager& instance()
    {
        static TimerManager m_timerManager;
        return m_timerManager;
    }

    void addTimer(Timer* timer)
    {
        assert(timer != nullptr);
        if (timer->isFinish()) return;
        m_timers.push(timer);
    }

    bool empty()
    {
        return m_timers.empty();
    }

    void update(uint64_t now_ms);

    ~TimerManager();

private:
    
    struct earlyFirst {
        bool operator()(const Timer* l, const Timer* r) const { 
            return l->isLaterThan(r); 
        }
    };

    std::priority_queue<Timer*, std::vector<Timer*>, earlyFirst> m_timers;
};

#endif