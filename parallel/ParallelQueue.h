//
// Created by matthias on 05.04.20.
//

#ifndef UTILS_PARALLEL_QUEUE_H
#define UTILS_PARALLEL_QUEUE_H

#include <mutex>
#include "../datastructures/Queue.h"

template <typename T>
class ParallelQueue {
public:
    void push(const T& v) {
        std::lock_guard<std::mutex> g(m);
        q.push(v);
    }
    void push(T&& v) {
        std::lock_guard<std::mutex> g(m);
        q.push(v);
    }
    const T& front() const {
        std::lock_guard<std::mutex> g(m);
        return q.front();
    }

    T& front() {
        std::lock_guard<std::mutex> g(m);
        return q.front();
    }

    void pop() {
        std::lock_guard<std::mutex> g(m);
        q.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> g(m);
        return q.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> g(m);
        return q.size();
    };
private:

    Queue<T> q;
    std::mutex m;
};


#endif //UTILS_PARALLEL_QUEUE_H
