//
// Created by matthias on 06.04.20.
//

#ifndef UTILS_GUARD_H
#define UTILS_GUARD_H

#include <mutex>


template <typename T>
class Guarded {
public:
    Guarded(const Guarded<T>&) = delete;
    Guarded(Guarded<T>&&);
    Guarded(T* obj, std::mutex* mutex) : obj_(obj), mutex_(mutex) {};
    ~Guarded();

    T& operator*();
    T* operator->();
private:
    T* obj_;
    std::mutex* mutex_;
};

template<typename T>
Guarded<T>::Guarded(Guarded<T> &&g) : obj_(g.obj_), mutex_(g.mutex_){
    g.mutex_ = nullptr;
}

template<typename T>
Guarded<T>::~Guarded() {
    if (mutex_ != nullptr) {
        mutex_->unlock();
    }
}

template <class T>
T& Guarded<T>::operator*() {
    return *(obj_);
}

template <class T>
T* Guarded<T>::operator->() {
    return obj_;
}

template <typename T>
class Guard {
public:
    template<typename ...Args>
    explicit Guard(Args &&... args) : obj_(std::move(T(std::forward<T>(args) ...))) {}
    Guard(const Guard<T>&) = delete;
    Guard(Guard<T>&& g) = delete;
    Guarded<T> getGuarded();

private:
    T obj_;
    std::mutex mutex_;
};

template<typename T>
Guarded<T> Guard<T>::getGuarded() {
    mutex_.lock();
    return Guarded<T>(&obj_, &mutex_);
}


#endif //UTILS_GUARD_H
