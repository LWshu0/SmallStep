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

    timer1->setCallback(std::bind(callBack, "callback 1")).setInterval(6000);
    timer2->setCallback(std::bind(callBack, "callback 2")).setInterval(5000).setRepeat(2);
    timer3->setCallback(std::bind(callBack, "callback 3")).setInterval(3000).setRepeat(3);
    TimerManager::instance().addTimer(timer1);
    TimerManager::instance().addTimer(timer2);
    TimerManager::instance().addTimer(timer3);

    while (!TimerManager::instance().empty())
    {
        Clock::instance().updateTime();
        TimerManager::instance().update();
    }

}