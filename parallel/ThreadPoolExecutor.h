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
#include <future>


// join() always needs to be called before deleting this Object.
template <typename T>
class ThreadPoolExecutor {
public:
    using Task = std::function<T()>;

    explicit ThreadPoolExecutor(size_t size);
    ThreadPoolExecutor(const ThreadPoolExecutor&) = delete;
    ThreadPoolExecutor(ThreadPoolExecutor&&) = delete;

    std::future<T> execute(Task);
    // Stop threads from taking tasks from the queue. Adding tasks after this call is not allowed.
    void shutDownNow();
    bool shutDownCompleted();
    // Waits for all tasks to be executed. Adding tasks after this call is not allowed.
    // If shutDownNow was called before it only waits for currently running tasks to finish.
    void join();


private:
    std::vector<std::thread> threads_;
    std::mutex mutex_;
    std::condition_variable cv_;

    Queue<std::tuple<Task, std::promise<T>>> task_promise_queue_;
    bool shutdown_ = false;
    bool shutdownNow_ = false;
    size_t online_threads_;
    void threadRoutine();
};



template <typename T>
ThreadPoolExecutor<T>::ThreadPoolExecutor(size_t size) : online_threads_(size) {
    threads_.reserve(size);
    for(size_t i = 0; i < size; i++) {
        threads_.push_back(std::thread(&ThreadPoolExecutor::threadRoutine, this));
    };
}

template <typename T>
std::future<T> ThreadPoolExecutor<T>::execute(ThreadPoolExecutor::Task f) {
    std::promise<T> promise;
    std::future<T> result_future = promise.get_future();
    {
        std::lock_guard<std::mutex> lck(mutex_);
        task_promise_queue_.push(make_tuple(f, std::move(promise)));
    }
    cv_.notify_one();
    return result_future;
}

template <typename T>
void ThreadPoolExecutor<T>::threadRoutine() {
    std::unique_lock<std::mutex> lck(mutex_,  std::defer_lock);
    while(true) {
        lck.lock();
        while (task_promise_queue_.empty() && !shutdownNow_) {
            if(!shutdown_) {
                cv_.wait(lck);
            } else {
                online_threads_--;
                lck.unlock();
                return;
            }
        }
        if (!shutdownNow_) {
            auto [task, promise] = std::move(task_promise_queue_.front());
            task_promise_queue_.pop();
            lck.unlock();
            promise.set_value(task());
        } else {
            online_threads_--;
            lck.unlock();
            return;
        }
    }
}

template <typename T>
void ThreadPoolExecutor<T>::shutDownNow() {
    std::lock_guard<std::mutex> lck(mutex_);
    shutdownNow_ = true;
    cv_.notify_all();
}

template <typename T>
bool ThreadPoolExecutor<T>::shutDownCompleted() {
    std::lock_guard<std::mutex> lck(mutex_);
    return online_threads_ == 0;
}

template<typename T>
void ThreadPoolExecutor<T>::join() {
    {
        std::lock_guard<std::mutex> lck(mutex_);
        shutdown_ = true;
    }
    for(auto& t : threads_) {
       t.join();
    }
}


#endif //UTILS_THREADPOOLEXECUTOR_H
