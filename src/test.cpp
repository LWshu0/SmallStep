#include <iostream>
#include <chrono>
#include "Core/Clock.h"
#include "Timer/Timer.h"
#include "Timer/TimerManager.h"

void callBack(const std::string& str)
{
    std::cout << str << std::endl;
}

int main()
{
    Timer* timer1 = new Timer();
    Timer* timer2 = new Timer();
    Timer* timer3 = new Timer();

    timer1->setCallback(std::bind(callBack, "callback 1"));
    timer2->setCallback(std::bind(callBack, "callback 2"));
    timer3->setCallback(std::bind(callBack, "callback 3"));
    timer1->setTimeout(Clock::instance().getTime(), 6000);
    timer2->setTimeout(Clock::instance().getTime(), 5000, 2);
    timer3->setTimeout(Clock::instance().getTime(), 3000, 3);
    TimerManager::instance().addTimer(timer1);
    TimerManager::instance().addTimer(timer2);
    TimerManager::instance().addTimer(timer3);

    while (!TimerManager::instance().empty())
    {
        Clock::instance().updateTime();
        TimerManager::instance().update(Clock::instance().getTime());
    }

}