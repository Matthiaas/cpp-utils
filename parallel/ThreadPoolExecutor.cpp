//
// Created by matthias on 06.04.20.
//

#include "ThreadPoolExecutor.h"

ThreadPoolExecutor::ThreadPoolExecutor(size_t size) : online_threads_(size) {
    threads_.reserve(size);
    for(size_t i = 0; i < size; i++) {
        threads_.push_back(std::thread(&ThreadPoolExecutor::threadRoutine, this));
    };
}

void ThreadPoolExecutor::execute(Task&& f) {
    std::unique_lock<std::mutex> lck(mutex_);
    task_queue_.push(std::move(f));
    cv_.notify_one();
}

void ThreadPoolExecutor::execute(const ThreadPoolExecutor::Task& f) {
    std::unique_lock<std::mutex> lck(mutex_);
    task_queue_.push(f);
    cv_.notify_one();
}

void ThreadPoolExecutor::threadRoutine() {
    while(running_) {
        Task task;
        {
            std::unique_lock<std::mutex> lck(mutex_);
            while (task_queue_.empty() && running_) cv_.wait(lck);
            if (running_) {
                task = std::move(task_queue_.front());
                task_queue_.pop();
            } else {
                online_threads_--;
                return;
            }
        }
        task();
    }
}

void ThreadPoolExecutor::shutDown() {
    std::unique_lock<std::mutex> lck(mutex_);
    running_ = false;
    cv_.notify_all();
}

bool ThreadPoolExecutor::isShutDown() {
    std::unique_lock<std::mutex> lck(mutex_);
    return online_threads_ == 0;
}


