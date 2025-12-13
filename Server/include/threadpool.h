#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<functional>
#include <thread>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<future>

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
    std::atomic<bool> isStopped = false;
    
    void Start(size_t numThreads);
    void Stop() noexcept;
};

template <class T>
inline auto ThreadPool::Enqueue(T task)
{
    auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
    {
        std::unique_lock<std::mutex> lock{nEventMutex};
        tasks.emplace([=]{ (*wrapper)(); });
    }
    nEventVar.notify_one();
    return wrapper->get_future();
}
