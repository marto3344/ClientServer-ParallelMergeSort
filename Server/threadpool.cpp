#include "threadpool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
    Start(numThreads);
}

ThreadPool::~ThreadPool()
{
}
