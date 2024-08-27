#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <chrono>
#include <functional>
#include <cassert>
#include <iostream>

#include "Core/Clock.h"

class Timer {
private:
    std::function<void(void)> m_callback;

    int64_t m_callbackTime;         // 下一次调用回调的时间
    int64_t m_callbackInterval;     // 回调时间间隔
    int64_t m_callbackRepeatCount;  // 回调剩余次数

    bool m_isRunning;               // 计时中
    bool m_isTriggerOnStart;        // 开始时触发回调一次

public:
    Timer();

    /**
     *@brief 设置计时器回调函数
     * 
     * @param callback 回调函数
     * @return Timer& 计时器对象本身
     */
    Timer& setCallback(const std::function<void(void)>& callback);

    /**
    * @brief 设置每 interval 毫秒执行一次
    *
    * @param interval 定时时间(毫秒), timeout 时间后执行回调函数一次
    *     * @return Timer& 计时器对象本身
    */
    Timer& setInterval(uint64_t interval);

    /**
     * @brief 设置回调执行的次数
     *
     * @param repeat_count 执行回调 repeat_count 次后销毁定时器, 如果 repeat_count 为负数, 将永远循环下去
     * @return Timer& 计时器对象本身
     */
    Timer& setRepeat(uint64_t repeat_count);

    /**
     *@brief 启用后, 在计时器开始时调用一次回调
     * 
     * @param trigger_on_start true 则启用 
     * @return Timer& 计时器对象本身
     */
    Timer& setTriggerOnStart(bool trigger_on_start);

    void start(int64_t now_ms);
    void stop();

    /**
     *@brief 更新定时器, 如果到时间则执行回调函数
     *
     * @param now_ms 当前时间(毫秒)
     * @return true 定时器到时间, 执行了一次回调
     * @return false 定时器未到时间
     */
    bool update(int64_t now_ms);

    bool running()
    {
        return m_isRunning;
    }
    bool timeout(int64_t now_ms)
    {
        return m_callbackTime <= now_ms;
    }
    bool dead()
    {
        return 0 == m_callbackRepeatCount;
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

    ~Timer();
};


#endif