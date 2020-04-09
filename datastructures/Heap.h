//
// Created by matthias on 09.04.20.
//

#ifndef UTILS_HEAP_H
#define UTILS_HEAP_H

#include <functional>
#include "Vector.h"

template <class T, class Compare = std::less<T>>
class Heap {
public:
    Heap() = default;;
    Heap(const Heap&) = default;
    Heap(Heap&&) = default;
    ~Heap() = default;

    const T& top() const;

    bool empty() const;
    size_t size() const;

    void push(const T&);
    void push(T&&);
    template<typename ...Args>
    void emplace(Args && ...args);
    void pop();
    void swap(Heap<T> other);

private:
    Compare less;
    Vector<T> container;
};



template<class T, class Compare>
inline bool Heap<T, Compare>::empty() const {
    return container.empty();
}

template<class T, class Compare>
inline size_t Heap<T, Compare>::size() const {
    return container.size();
}

template<class T, class Compare>
const T &Heap<T, Compare>::top() const {
    return container[0];
}


template<class T, class Compare>
void Heap<T, Compare>::push(const T &v) {
    emplace(v);
}

template<class T, class Compare>
void Heap<T, Compare>::push(T&& v) {
    emplace(std::forward<T>(v));
}

template<class T, class Compare>
template<typename... Args>
void Heap<T, Compare>::emplace(Args &&... args) {
    container.emplace_back(std::forward<Args>(args) ...);
    int index = container.size() -1;
    if(index == 0) {
        return;
    }
    int parent_index = (index-1)/2;
    while(less(container[index], container[parent_index])) {
        std::swap(container[index], container[parent_index]);
        index = parent_index;
        parent_index = (index-1)/2;;
    }
}

template<class T, class Compare>
void Heap<T, Compare>::pop() {
    std::swap(container[0], container[container.size() -1 ]);
    container.pop_back();
    int index = 0;

    while(true) {
        int left_child = index * 2 + 1;
        int right_child = left_child + 1;

        int min_index = index;
        if (left_child < container.size() && less(container[left_child], container[min_index])) {
            min_index = left_child;
        }
        if (right_child < container.size() &&  less(container[right_child], container[min_index])) {
            min_index = right_child;
        }
        if(min_index == index) {
            break;
        }
        std::swap(container[min_index], container[index]);
        index = min_index;
    }


}


#endif //UTILS_HEAP_H
