//
// Created by matthias on 09.04.20.
//

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <cstddef>
#include <cstdlib>
#include <utility>

template <typename T>
class Vector {
public:
    Vector();
    explicit Vector(int size,const T& d=T{});
    Vector(const Vector&);
    Vector(Vector&&);
    ~Vector();


    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;

    void push_back(const T&);
    void push_back(T&&);
    template<typename ...Args>
    void emplace_back(Args && ...args);

    void push_front(const T&);
    void push_front(T&&);
    template<typename ...Args>
    void emplace_front(Args && ...args);

    void pop_back();
    void pop_front();


    T& operator[](int i) const;

    void reserve(int);
    void swap(Vector<T> other);

private:
    int count_;
    int reserved_size_;
    T* mem_;

    void resize();
};

template<typename T>
Vector<T>::Vector(): count_(0), reserved_size_(10) {
    mem_ = (T*) malloc(reserved_size_ * sizeof(T));
}

template<typename T>
Vector<T>::Vector(int size, const T &d) : count_(0), reserved_size_(size*2) {
    mem_ = (T*) malloc(reserved_size_ * sizeof(T));
    for(int i = 0; i < size; i++) {
        emplace_back(d);
    }
}

template<typename T>
Vector<T>::Vector(const Vector& v) {
    count_ = v.count_;
    reserved_size_ = v.count_;
    mem_ = (T*) malloc(reserved_size_ * sizeof(T));
    for (int i = 0; i < count_; i++) {
        mem_[i] = v[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector&& old) {
    count_ = old.count_;
    reserved_size_ = old.reserved_size_;
    mem_ = old.mem_;
    old.count_ = 0;
    old.reserved_size_ = 0;
    old.mem_ = nullptr;
}

template<typename T>
Vector<T>::~Vector() {
    for (int i = 0; i < count_; i++) {
        mem_[i].T::~T();
    }
    free(mem_);
}

template<typename T>
void Vector<T>::push_back(const T& v) {
    emplace_back(v);
}

template<typename T>
void Vector<T>::push_back(T&& v) {
    emplace_back(std::forward<T>(v));
}


template<typename T>
template<typename... Args>
void Vector<T>::emplace_back(Args &&... args) {
    if(count_ == reserved_size_) {
        resize();
    }
    ::new((void *) &mem_[count_]) T(std::forward<Args>(args)...);
    count_++;
}

template<typename T>
void Vector<T>::push_front(const T& v) {
    emplace_front(v);
}

template<typename T>
void Vector<T>::push_front(T&& v) {
    emplace_front(std::forward<T>(v));
}

template<typename T>
template<typename... Args>
void Vector<T>::emplace_front(Args &&... args) {
    emplace_back(std::forward<Args>(args)...);
    for(int i = count_ - 1; i > 0; i--) {
        std::swap(mem_[i], mem_[i-1]);
    }
};

template<typename T>
void Vector<T>::pop_back() {
    count_--;
    mem_[count_].T::~T();
    resize();
}

template<typename T>
void Vector<T>::pop_front() {
    for(int i = 0; i < count_ - 1; i++) {
        std::swap(mem_[i], mem_[i + 1]);
    }
    pop_back();
}

template<typename T>
T& Vector<T>::operator[](int i) const {
    return mem_[i];
}

template<typename T>
inline void Vector<T>::reserve(int new_size) {
    reserved_size_ = new_size;
    mem_ = (T*) realloc(mem_, new_size * sizeof(T));
}


template<typename T>
inline void Vector<T>::resize() {
    if(count_ == reserved_size_) {
        reserve(reserved_size_ * 2);
    } else  if (count_ < reserved_size_ / 4){
        reserve(reserved_size_ / 2);
    }
}


template <typename T>
inline bool Vector<T>::empty() const {
    return count_ == 0;
}

template <typename T>
inline size_t Vector<T>::size() const {
    return count_;
}


#endif //UTILS_VECTOR_H
