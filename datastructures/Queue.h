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
    // Constructors
    Queue();
    ~Queue();
    Queue(const Queue<T,buff_size>& q);
    Queue(Queue<T,buff_size>&& q) noexcept;

    // Member Fuctions
    template<typename ...Args>
    void emplace(Args && ...args);
    void push(const T& v);
    void push(T&& v);
    void pop();
    void swap(Queue<T,buff_size>& other);

    [[nodiscard]] const T& front() const;
    [[nodiscard]] T& front();

    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;

    //Non-Member Functions
    template<class U, size_t buff_size_l, size_t buff_size_r>
    friend bool operator==(const Queue<U, buff_size_l>& lhs, const Queue<U, buff_size_r>& rhs);

private:
    // Member Variables
    T** map;
    int map_size = 5;
    std::pair<size_t ,size_t> begin = {0,0};
    std::pair<size_t, size_t> end = {0, 0};
    int count = 0;

    //
    void shiftToZero(T** old_map);
    void increaseEndCounter();
    void resize(int newSize);
    void addPushBackSpace();
    void maybeFreeUpSpace();
};

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::Queue() {
    map = (T**) malloc(sizeof(T*) * map_size);
    map[end.first] = (T*) malloc(sizeof(T) * buff_size);
}

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::~Queue() {
    for(int i = begin.first; i <= end.first; i++) {
        for(int j = 0; j < buff_size; j ++) {
            if((i != begin.first || j >= begin.second) && (i != end.first || j < end.second)){
                map[i][j].T::~T();
            }
        }
        free(map[i]);
    }
    free(map);
}

template <typename T, size_t buff_size>
inline Queue<T,buff_size>::Queue(const Queue<T,buff_size>& q) {
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
inline Queue<T,buff_size>::Queue(Queue<T,buff_size>&& q) noexcept {
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
    ::new((void *) &map[end.first][end.second]) T(std::forward<Args>(args)...);
    count++;
    increaseEndCounter();
}

template <typename T, size_t buff_size>
void inline Queue<T,buff_size>::push(const T& v) {
    emplace(v);
}

template <typename T, size_t buff_size>
void inline Queue<T,buff_size>::push(T&& v) {
    emplace(std::forward<T>(v));
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
    map[begin.first][begin.second].T::~T();
    begin.second = (begin.second + 1) % buff_size;
    if(begin.second == 0) {
        free(map[begin.first]);
        begin.first++;
    }
}

template <typename T, size_t buff_size>
void Queue<T,buff_size>::swap(Queue<T,buff_size>& other) {
    begin.swap(other.begin);
    end.swap(other.end);
    std::swap(count, other.count);
    std::swap(map_size, other.map_size);
    std::swap(map, other.map);
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


template <typename U, size_t buff_size_l, size_t buff_size_r>
bool operator==(const Queue<U, buff_size_l>& lhs, const Queue<U, buff_size_r>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    int lf = lhs.begin.first;
    int ls = lhs.begin.second;
    int rf = lhs.begin.first;
    int rs = lhs.begin.second;

    while (lf != lhs.end.first && ls != lhs.end.second) {
        if (lhs.map[lf][ls] != rhs.map[rf][rs]) {
            return false;
        }
        ls = (ls + 1) % buff_size_l;
        rs = (rs + 1) % buff_size_r;
        if(ls == 0) {
            lf++;
        }
        if(rs == 0) {
            rf++;
        }
    }
    return true;
}

#endif //UTILS_QUEUE_H