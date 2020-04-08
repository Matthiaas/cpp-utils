//
// Created by matthias on 07.04.20.
//

#ifndef UTILS_FIBONACCIHEAP_H
#define UTILS_FIBONACCIHEAP_H

#include <functional>
#include <list>

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


    List roots = {nullptr, nullptr};
    Node* min;
    int count = 0;
};

template<class T, class Compare>
FibonacciHeap<T, Compare>::~FibonacciHeap() {
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
void FibonacciHeap<T, Compare>::push(const T& v) {
    emplace(v);
}

template<class T, class Compare>
void FibonacciHeap<T, Compare>::push(T&& v) {
    emplace(std::forward<T>(v));
}

template<class T, class Compare>
template<typename... Args>
void FibonacciHeap<T, Compare>::emplace(Args &&... args) {
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
    delete min;
    if(count == 0) {
        return;
    }
    // Add children to main list.
    appendList(min->children);
    Node* cur_min = roots.head;

    std::vector<Node*> nodeDegree(100, nullptr);
    for (Node* cur = roots.head; cur != nullptr; cur =  cur->next) {
        int degree = cur->degree;
        Node* n = cur;
        while(nodeDegree[degree]){
            n = merge(n, nodeDegree[degree]);
            nodeDegree[degree] = nullptr;
            degree++;
        }
        nodeDegree[degree] = n;
        if (less(n->value, cur_min->value)) {
            cur_min = n;
        }
    }
    min = cur_min;
}

template<class T, class Compare>
typename FibonacciHeap<T, Compare>::Node* FibonacciHeap<T, Compare>::merge(Node* n1, Node* n2) {
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
void FibonacciHeap<T, Compare>::appendNode(List& l, Node* n) {
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
void FibonacciHeap<T, Compare>::appendList(List l) {
    if (roots.head == nullptr) {
        roots = l;
    } else if (l.head != nullptr) {
        roots.tail->next = min->children.head;
        roots.tail = min->children.tail;
    }
}




#endif //UTILS_FIBONACCIHEAP_H
