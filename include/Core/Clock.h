#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include "Core/SpinLock.h"

class Clock {
private:
    Clock();
public:
    Clock(Clock const&) = delete;
    void operator=(Clock const&) = delete;

    static Clock& instance()
    {
        static Clock m_clock;
        return m_clock;
    }

    void setFactor(double factor)
    {
        m_configTimeFactor = factor;
        m_factor = factor;
    }

    void updateTime();

    void pause();

    void start();

    void reset();

    inline bool isPause() const
    {
        return m_factor == 0.0;
    }

    inline int64_t getTime() const
    {
        return m_lastUpdateGameTime;
    };

    inline int64_t getDeltaTime() const
    {
        if (isPause()) return 0;
        return m_deltaGameTime;
    }

    ~Clock();

private:
    using Clock_t = std::chrono::steady_clock;
    using TimeUnit_t = std::chrono::milliseconds;

    // config
    double m_configTimeFactor;
    SpinLock m_spinlock;

    // runtime
    double m_factor;                    // 时间系数, 0.0 代表暂停
    int64_t m_lastUpdateRealTime;       // 上一次更新时的实际时间(nano)
    int64_t m_lastPauseRealTime;        // 暂停时的实际时间(nano), 用与恢复到正确的游戏时间

    int64_t m_lastUpdateGameTime;       // 上一次更新时的游戏时间(nano)
    int64_t m_deltaGameTime;            // 距离上一次更新经过的游戏时间(nano)

    inline int64_t now_milliseconds()
    {
        return std::chrono::duration_cast<TimeUnit_t>(Clock_t::now().time_since_epoch()).count();
    }
};

#endif