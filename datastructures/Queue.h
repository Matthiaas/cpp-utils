//
// Created by matthias on 05.04.20.
//

#ifndef UTILS_QUEUE_H
#define UTILS_QUEUE_H

#include <cstdlib>
#include <utility>

// buff_size should be large (20-30) for small types like ints
// buff_size should be small (5) for large types >= 5KB

template <typename T, size_t buff_size = 20>
class Queue {
public:
    Queue();
    ~Queue();
    Queue(const Queue& q);
    Queue(Queue&& q);

    template<typename ...Args>
    void emplace(Args && ...args);
    void push(const T& v);
    void push(T&& v);
    const T& front() const;
    T& front();
    void pop();
    bool empty() const;
    size_t size() const;

private:
    T** map;
    int map_size = 5;
    std::pair<size_t ,size_t> begin = {0,0};
    std::pair<size_t, size_t> end = {0, 0};
    int count = 0;

    void shiftToZero(T** old_map);
    void increaseEndCounter();
    void resize(int newSize);
    void addPushBackSpace();
    void maybeFreeUpSpace();
};

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::Queue() {
    map = (T**)  malloc(sizeof(T*) * map_size);
    map[end.first] = (T*) malloc(sizeof(T) * buff_size);
}

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::~Queue() {
    for(int i = begin.first; i <= end.first; i++) {
        free(map[i]);
    }
    free(map);
}

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::Queue(const Queue& q) {
    begin = q.begin;
    end = q.end;
    count = q.count;
    map_size = q.map_size;

    map = (T**)  malloc(sizeof(T*) * map_size);
    for(int i = begin.first; i <= end.first; i++) {
        map[i] = (T*) malloc(sizeof(T) * buff_size);
        T* newData = map[i];
        T* oldData = q.map[i];
        for (int j = 0; j < buff_size; j++) {
            newData[j] = oldData[j];
        }
    }
}

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::Queue(Queue&& q) {
    begin = q.begin;
    end = q.end;
    map = q.map;
    count = q.count;
    map_size = q.map_size;

    // Invalidate old Queue so no free is called again.
    q.map = nullptr;
    q.begin.first = 1;
    q.end.first = 0;
}


template <typename T, size_t buff_size>
template<typename ...Args>
void inline Queue<T,buff_size>::emplace(Args && ...args) {
    map[end.first][end.second] = std::move(T(std::forward<T>(args) ...));
    count++;
    increaseEndCounter();
}

template <typename T, size_t buff_size>
void inline Queue<T,buff_size>::push(const T& v) {
    map[end.first][end.second] = v;
    count++;
    increaseEndCounter();
}

template <typename T, size_t buff_size>
void inline Queue<T,buff_size>::push(T&& v) {
    map[end.first][end.second] = std::forward<T>(v);
    count++;
    increaseEndCounter();
}

template <typename T, size_t buff_size>
inline const T& Queue<T,buff_size>::front() const {
    return map[begin.first][begin.second];
}

template <typename T, size_t buff_size>
inline T& Queue<T,buff_size>::front() {
    return map[begin.first][begin.second];
}

template <typename T, size_t buff_size>
inline void Queue<T,buff_size>::pop() {
    count--;
    begin.second = (begin.second + 1) % buff_size;
    if(begin.second == 0) {
        free(map[begin.first]);
        begin.first++;
    }

}

template <typename T, size_t buff_size>
inline bool Queue<T,buff_size>::empty() const {
    return count == 0;
}

template <typename T, size_t buff_size>
inline size_t Queue<T,buff_size>::size() const {
    return count;
};







template <typename T, size_t buff_size>
void Queue<T,buff_size>::shiftToZero(T** old_map) {
    int diff = (end.first - begin.first);
    for(int i = 0, j = begin.first; j <= end.first; j++ ,i++) {
        map[i] = old_map[j];
    }
    begin.first = 0;
    end.first = diff;
}

template <typename T, size_t buff_size>
void Queue<T,buff_size>::increaseEndCounter(){
    end.second = (end.second + 1) % buff_size;
    if(end.second == 0) {
        end.first++;
        if(end.first == map_size) {
            addPushBackSpace();
        }
        map[end.first] = (T*) malloc(sizeof(T) * buff_size);
    }
}

template <typename T, size_t buff_size>
void Queue<T,buff_size>::resize(int newSize) {
    T** old_map = map;
    map_size = newSize;
    map = (T**) malloc(sizeof(T*) * map_size);
    shiftToZero(old_map);
    free(old_map);
}

template <typename T, size_t buff_size>
inline void Queue<T,buff_size>::addPushBackSpace() {
    if (begin.first < map_size / 2) {
        resize(map_size*2);
    } else {
        shiftToZero(map);
    }
}

template <typename T, size_t buff_size>
inline void Queue<T,buff_size>::maybeFreeUpSpace() {
    if (begin.first > map_size * 4 / 5) {
        resize(map_size/2);
    }
}



#endif //UTILS_QUEUE_H
