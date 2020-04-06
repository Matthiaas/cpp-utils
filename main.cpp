#include <iostream>

#include "datastructures/Queue.h"

#include <array>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std::chrono;

using data_type = std::array<long,10>;

int main() {



    int N = 10000000;
    // Should be between 1 and 100;
    int push_threshold = 70;
    data_type data;

    std::vector<int>  arr;
    std::srand(std::time(nullptr));
    for(int i = 0; i< N; i++) {
        arr.push_back(std::rand() % 100);
    }

    milliseconds start = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    {
        std::queue<data_type> std_q;
        for(int i = 0; i< arr.size(); i++) {
            int r = arr[i];
            if(r < push_threshold) {
                std_q.push(data);
            } else {
                size_t size = std_q.size();
                if( size > 0) {
                    data_type a = std_q.front();
                    std_q.pop();
                }
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
    {
        Queue<data_type,20> q;
        for(int i = 0; i< arr.size(); i++) {
            int r = arr[i];
            if(r < push_threshold) {
                q.push(data);
            } else {
                size_t size = q.size();
                if( size > 0) {
                    data_type a = q.front();
                    q.pop();
                }
            }
        }
    }
    end = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    std::cout << (end - start).count() << std::endl;
    return 0;
}
