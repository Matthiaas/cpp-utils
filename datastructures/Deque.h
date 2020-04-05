//
// Created by matthias on 06.04.20.
//

#ifndef UTILS_DEQUE_H
#define UTILS_DEQUE_H

#include <vector>
#include <array>



template <typename T, size_t buff_size = 20, size_t begin_map_size = 5 >
class Deque {
    Deque() {
        map = (T**)  malloc(sizeof(T*) * map_size);
        map[end.first] = (T*) malloc(sizeof(T) * buff_size);
    }

    ~Deque() {

    }

    void center(T** new_map) {
        int diff = (end.first - begin.first);
        int new_begin = (map_size + 1 - diff) / 2;
        for(int i = new_begin, j = begin.first; j <= end.first; j++ ,i++) {
            new_map[i] = map[j];
        }
    }

    void downsizeAndCenter() {
        // TODO: downsize and actually use it.
    }

    void upsizeAndCenter(bool decrease) {
        int diff = (end.first - begin.first);
        double fillFactor = ((diff + 1) * 1.0 / map_size);
        if (decrease && fillFactor >= 0.3) {
            return;
        }

        bool resize = fillFactor >= 0.8  || fillFactor < 0.3;
        T** old_map = map;
        if (resize) {
            map_size *= 2;
            map = (T**) malloc(sizeof(T*) * map_size);
        }

        if(resize) {
            free(old_map);
        }
    }

    void push_front(T&& v) {
        count++;
        if(begin.second == 0) {
            if(begin.first == 0) {
                upsizeAndCenter(true);
            }
            begin.second = buff_size;
            begin.first--;
            map[begin.first] = (T*) malloc(sizeof(T) * buff_size);
        }
        begin.second--;
        map[begin.first][begin.second] = std::forward<T>(v);
    }

    void push_back(T&& v) {
        count++;
        map[end.first][end.second] = std::forward<T>(v);
        end.second = (end.second + 1) % buff_size;
        if(end.second == 0) {
            end.first++;
            if(end.first == map_size) {
                upsizeAndCenter(false);
            }
            map[end.first] = (T*) malloc(sizeof(T) * buff_size);
        }
    }

    const T& front() const {
        return map[begin.first][begin.second];
    }
    T& front() {
        return map[begin.first][begin.second];
    }
    const T& back() const {
        return map[end.first][end.second];
    }
    T& back(){
        return map[end.first][end.second];
    }

    void pop_front() {
        count--;
        begin.second = (begin.second + 1) % buff_size;
        if(begin.second == 0) {
            free(map[begin.first]);
            begin.first++;
        }

    }
    void pop_back() {
        count--;
        if(end.second == 0) {
            free(map[end.first]);
            end.first--;
            end.second = buff_size;
        }
        end.second--;
    }

    bool empty() const {
        return count == 0;
    }
    bool size() const {
        return count;
    };

private:
    T** map;
    std::pair<size_t ,size_t> begin = {begin_map_size/2,0};
    std::pair<size_t, size_t> end = {begin_map_size/2, 0};
    int count = 0;
    int map_size = begin_map_size;
};


#endif //UTILS_DEQUE_H
