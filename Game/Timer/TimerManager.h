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

    /**
     *@brief 将 Timer 交由 TimerManager 管理, 添加后 TimerManager 将调用 Timer 的 start 函数启动定时器
     *       Timer 内存的释放也将由 TimerManager 进行
     * @param timer 定时器
     */
    void addTimer(Timer* timer);

    bool empty()
    {
        return m_timers.empty();
    }

    void update();

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