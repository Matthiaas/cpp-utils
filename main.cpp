#include <iostream>

#include "datastructures/Queue.h"

#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std::chrono;


int main() {



    int N = 20000000;
    // Should be between 1 and 100;
    int push_threshold = 50;

    std::vector<int>  arr;
    std::srand(std::time(nullptr));
    for(int i = 0; i< N; i++) {
        arr.push_back(std::rand() % 100);
    }

    milliseconds start = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    std::queue<int> std_q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            std_q.push(r);
        } else {
            size_t size = std_q.size();
            if( size > 0) {
                int a = std_q.front();
                std_q.pop();
            }
        }
    }
    milliseconds end = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );



    std::cout << (end - start).count() << std::endl;


    start = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    Queue<int,100,5> q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                int a = q.front();
                q.pop();
            }
        }
    }
    end = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );

    std::cout << (end - start).count() << std::endl;
    return 0;
}
