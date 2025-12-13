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
    //TODO: Implement start logic
}

void ThreadPool::Stop() noexcept
{
    //TODO: Implement stop logic
}
