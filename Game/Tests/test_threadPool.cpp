#include "Core/ThreadPool.h"

std::mutex mtx;

void Task()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Function invoked in thread " << std::this_thread::get_id() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    ThreadPool pool(4);

    for (int i = 0; i < 10; ++i)
    {
        pool.addTask(Task);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}