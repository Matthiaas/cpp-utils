//
// Created by matthias on 06.04.20.
//

#ifndef UTILS_THREADPOOLEXECUTOR_H
#define UTILS_THREADPOOLEXECUTOR_H


#include <functional>
#include <vector>
#include <thread>
#include <condition_variable>


#include "../datastructures/Queue.h"
#include "Guard.h"

class ThreadPoolExecutor {
public:
    using Task = std::function<void()>;

    explicit ThreadPoolExecutor(size_t size);
    ThreadPoolExecutor(const ThreadPoolExecutor&) = delete;
    ThreadPoolExecutor(ThreadPoolExecutor&&) = delete;

    void execute(Task&&);
    void execute(const Task&);
    void shutDown();
    bool isShutDown();

private:
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable cv_;

    Queue<Task> task_queue_;
    bool running_ = true;
    size_t online_threads_;
    void threadRoutine();
};


#endif //UTILS_THREADPOOLEXECUTOR_H
