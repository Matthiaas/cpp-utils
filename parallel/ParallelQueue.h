//
// Created by matthias on 05.04.20.
//

#ifndef UTILS_PARALLEL_QUEUE_H
#define UTILS_PARALLEL_QUEUE_H

#include "Guard.h"
#include "../datastructures/Queue.h"

// Actually this class is totally useless since we could directly use Guarded<Queue<T>>.
template <typename T>
class ParallelQueue {
public:
    void push(const T& v) {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        gq->push(v);
    }
    void push(T&& v) {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        gq->push(v);
    }
    const T& front() const {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        return gq->front();
    }

    T& front() {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        return gq->q.front();
    }

    void pop() {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        gq->pop();
    }

    bool empty() const {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        return gq->empty();
    }

    size_t size() const {
        Guarded<Queue<T>> gq = qGuard.getGuarded();
        return->size();
    };
private:
    Guard<Queue<T>> qGuard;
};


#endif //UTILS_PARALLEL_QUEUE_H
