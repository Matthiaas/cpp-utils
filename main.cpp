#include <iostream>

#include "datastructures/Queue.h"
#include "parallel/ThreadPoolExecutor.h"

#include <array>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <vector>


using namespace std::chrono;

using data_type = std::array<long,10>;

int testStdQueue(std::vector<int>& arr, data_type& data, int push_threshold) {
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
    return (end - start).count();
}

int testMyQueue(std::vector<int>& arr, data_type& data, int push_threshold) {
    milliseconds start = duration_cast< milliseconds >(
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
    milliseconds end = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    return (end - start).count();
}

void foo() {

}

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


    ThreadPoolExecutor<int> tpe(2);
    std::future<int> f1 = tpe.execute(std::bind(testStdQueue, arr, data, push_threshold));
    std::future<int> f2 = tpe.execute(std::bind(testMyQueue, arr, data, push_threshold));
    std::future<int> f3 = tpe.execute(std::bind(testStdQueue, arr, data, push_threshold));
    std::future<int> f4 = tpe.execute(std::bind(testMyQueue, arr, data, push_threshold));
    tpe.shutDownNow();
    tpe.join();
    std::cout << f1.get() << std::endl;
    std::cout << f2.get() << std::endl;
    std::cout << (f3.wait_for(std::chrono::seconds(0)) == std::future_status::ready ) << std::endl;
    std::cout << (f4.wait_for(std::chrono::seconds(0)) == std::future_status::ready)  << std::endl;


    return 0;
}
