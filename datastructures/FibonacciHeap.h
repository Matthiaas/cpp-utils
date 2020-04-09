//
// Created by matthias on 07.04.20.
//

#ifndef UTILS_FIBONACCIHEAP_H
#define UTILS_FIBONACCIHEAP_H

#include <functional>
#include <list>
#include <cmath>


template <class T, class Compare = std::less<T>>
class FibonacciHeap {
public:
    FibonacciHeap() {};
    FibonacciHeap(const FibonacciHeap&);
    FibonacciHeap(FibonacciHeap&&);
    ~FibonacciHeap();

    const T& top() const;

    bool empty();
    size_t size();

    void push(const T&);
    void push(T&&);
    template<typename ...Args>
    void emplace(Args && ...args);
    void pop();
    void swap(FibonacciHeap<T> other);

private:
    Compare less;

    struct List;
    struct Node {
        T value;
        Node* prev;
        Node* next;
        List children;
        int degree;
    };

    struct List {
        Node* head;
        Node* tail;
    };

    void removeRootNode(Node*);
    void appendNode(List& l, Node*);
    void appendList(List l);
    void deleteList(List l);
    Node* merge(Node*, Node*);

    const double log_golden_ration = std::log((1+sqrt(5))/2);

    List roots = {nullptr, nullptr};
    Node* min;
    int count = 0;
};

template<class T, class Compare>
inline FibonacciHeap<T, Compare>::~FibonacciHeap() {
    deleteList(roots);
}

template<class T, class Compare>
void FibonacciHeap<T, Compare>::deleteList(FibonacciHeap::List l) {
    Node* cur = l.head;
    while (cur != nullptr) {
        deleteList(cur->children);
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
}

template<class T, class Compare>
inline const T& FibonacciHeap<T, Compare>::top() const {
    return min->value;
}

template<class T, class Compare>
inline void FibonacciHeap<T, Compare>::push(const T& v) {
    emplace(v);
}

template<class T, class Compare>
inline void FibonacciHeap<T, Compare>::push(T&& v) {
    emplace(std::forward<T>(v));
}

template<class T, class Compare>
template<typename... Args>
inline void FibonacciHeap<T, Compare>::emplace(Args &&... args) {
    // TODO: Consutuct in place to make this work with complex types!
    T v = T(args ...);
    Node* new_root_node = new Node{v, nullptr, nullptr, List {nullptr, nullptr} , 0};
    appendNode(roots, new_root_node);
    if(count == 0 || less(v,top())) {
        min = new_root_node;
    }
    count++;
}

template<class T, class Compare>
void FibonacciHeap<T, Compare>::pop() {
    count--;
    // Remove min node:
    removeRootNode(min);
    // Add children to main list.
    appendList(min->children);
    delete min;
    if(count == 0) {
        return;
    }

    Node* cur_min = roots.head;

    std::vector<Node*> nodeDegree(std::floor(std::log(count) / log_golden_ration) + 1, nullptr);
    Node* cur = roots.head;
    while (cur != nullptr) {
        int degree = cur->degree;
        Node* next = cur->next;
        while(nodeDegree[degree]){
            cur = merge(cur, nodeDegree[degree]);
            nodeDegree[degree] = nullptr;
            degree++;
        }
        nodeDegree[degree] = cur;
        if (!less(cur_min->value, cur->value)) {
            cur_min = cur;
        }
        cur =  next;
    }
    min = cur_min;
}

template<class T, class Compare>
inline typename FibonacciHeap<T, Compare>::Node* FibonacciHeap<T, Compare>::merge(Node* n1, Node* n2) {
    if(less(n2->value, n1->value)) {
        std::swap(n1,n2);
    }
    removeRootNode(n2);
    appendNode(n1->children, n2);
    n1->degree++;
    return n1;
}

template<class T, class Compare>
inline void FibonacciHeap<T, Compare>::removeRootNode(FibonacciHeap::Node* n) {
    if(n == roots.head && n == roots.tail) {
        roots.head = nullptr;
        roots.tail = nullptr;
    } else if(n == roots.head) {
        roots.head = n->next;
        n->next->prev = nullptr;
    } else  if(n == roots.tail)  {
        n->prev->next = nullptr;
        roots.tail = n->prev;
    } else {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    n->prev = nullptr;
    n->next = nullptr;
}

template<class T, class Compare>
inline void FibonacciHeap<T, Compare>::appendNode(List& l, Node* n) {
    if(l.head != nullptr) {
        n->prev = l.tail;
        l.tail->next = n;
        l.tail = n;
    } else {
        l.head = n;
        l.tail = n;
    }
}

template<class T, class Compare>
inline void FibonacciHeap<T, Compare>::appendList(List l) {
    if (roots.head == nullptr) {
        roots = l;
    } else if (l.head != nullptr) {
        l.head->prev = roots.tail;
        roots.tail->next = l.head;
        roots.tail = l.tail;
    }
}

template<class T, class Compare>
inline bool FibonacciHeap<T, Compare>::empty() {
    return count == 0;
}

template<class T, class Compare>
inline size_t FibonacciHeap<T, Compare>::size() {
    return count;
}

template<class T, class Compare>
void FibonacciHeap<T, Compare>::swap(FibonacciHeap<T> other) {
    std::swap(count, other.count);
    std::swap(roots, other.roots);
    std::swap(min, other.min);
}


#endif //UTILS_FIBONACCIHEAP_H
