#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<functional>
#include <thread>
#include<mutex>
#include<condition_variable>

class ThreadPool
{
    public:
    using Task = std::function<void()>;

    template<class T>
	auto Enqueue(T task);
    
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    
    private:
    std::vector<std::thread> nThreads;
    std::condition_variable nEventVar;
	std::mutex nEventMutex;
    std::queue<Task> tasks;
    bool isStopped = false;
    
    void Start(size_t numThreads);
    void Stop();
};

template <class T>
inline auto ThreadPool::Enqueue(T task)
{
}
