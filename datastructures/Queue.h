//
// Created by matthias on 05.04.20.
//

#ifndef UTILS_QUEUE_H
#define UTILS_QUEUE_H


#include <vector>
#include <array>

template <typename T, size_t buff_size = 20, size_t begin_map_size = 5 >
class Queue {
public:
    Queue() {
        map = (T**)  malloc(sizeof(T*) * map_size);
        map[end.first] = (T*) malloc(sizeof(T) * buff_size);
    }

    ~Queue() {
        if(begin.first > end.first){
            // The content was moved, no need to free up here.
            return;
        }
        for(int i = begin.first; i <= end.first; i++) {
            free(map[i]);
        }
        free(map);
    }

    Queue(const Queue& q) {
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

    Queue(Queue&& q) {
        begin = q.begin;
        end = q.end;
        map = q.map;
        count = q.count;
        map_size = q.map_size;

        // Invalidate old Queue so no free is called again.
        q.begin.first = 1;
        q.end.first = 0;
    }


    void push(const T& v) {
        map[end.first][end.second] = v;
        count++;
        increaseEndCounter();
    }
    void push(T&& v) {
        map[end.first][end.second] = std::forward<T>(v);
        count++;
        increaseEndCounter();
    }

    const T& front() const {
        return map[begin.first][begin.second];
    }

    T& front() {
        return map[begin.first][begin.second];
    }

    void pop() {
        count--;
        begin.second = (begin.second + 1) % buff_size;
        if(begin.second == 0) {
            free(map[begin.first]);
            begin.first++;
        }

    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    };

private:
    T** map;
    std::pair<size_t ,size_t> begin = {begin_map_size/2,0};
    std::pair<size_t, size_t> end = {begin_map_size/2, 0};
    int count = 0;
    int map_size = begin_map_size;


    void shiftToZero(T** old_map) {
        int diff = (end.first - begin.first);
        for(int i = 0, j = begin.first; j <= end.first; j++ ,i++) {
            map[i] = old_map[j];
        }
        begin.first = 0;
        end.first = diff;
    }

    void increaseEndCounter(){
        end.second = (end.second + 1) % buff_size;
        if(end.second == 0) {
            end.first++;
            if(end.first == map_size) {
                addPushBackSpace();
            }
            map[end.first] = (T*) malloc(sizeof(T) * buff_size);
        }
    }

    void resize(int newSize) {
        T** old_map = map;
        map_size = newSize;
        map = (T**) malloc(sizeof(T*) * map_size);
        shiftToZero(old_map);
        free(old_map);
    }

    void addPushBackSpace() {
        if (begin.first < map_size / 2) {
            resize(map_size*2);
        } else {
            shiftToZero(map);
        }
    }

    void maybeFreeUpSpace() {
        if (begin.first > map_size * 4 / 5) {
            resize(map_size/2);
        }
    }
};


#endif //UTILS_QUEUE_H
