#include "threadpool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
    Start(numThreads);
}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::Start(size_t numThreads)
{
    for(int i = 0 ; i < numThreads; ++i)
    {
        workers.emplace_back(
            [=]{
                while (1)
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock (eventMutex);
                        eventVar.wait(lock,[=] { return isStopped || !tasks.empty();});
                        if(isStopped && tasks.empty())
                            break;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}

void ThreadPool::Stop() noexcept
{
    {
        std::unique_lock<std::mutex> lock (eventMutex);
        isStopped = true;
    }
    eventVar.notify_all();
    for(auto& thread : workers)
    {
        thread.join();
    }
}
