#include <iostream>

#include "datastructures/Queue.h"
#include "parallel/ThreadPoolExecutor.h"
#include "datastructures/FibonacciHeap.h"

#include <array>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <vector>


using namespace std::chrono;

using data_type = std::array<long,10>;

void testStdQueue(std::vector<int>& arr, data_type& data, int push_threshold) {
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

void testMyQueue(std::vector<int>& arr, data_type& data, int push_threshold) {

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

int timeIt(std::function<void()> f){
    milliseconds start = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    f();
    milliseconds end = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    return (end - start).count();
}

void threadPoolQueusTest() {
    int N = 10000000;
    // Should be between 1 and 100;
    int push_threshold = 70;
    data_type data;

    std::vector<int>  arr;
    std::srand(std::time(nullptr));
    for(int i = 0; i< N; i++) {
        arr.push_back(std::rand() % 100);
    }

    timeIt(std::bind(testStdQueue, arr, data, push_threshold));
    std::function<void()> test_std_queue = std::bind(testStdQueue, arr, data, push_threshold);
    std::function<void()> test_my_queue = std::bind(testMyQueue, arr, data, push_threshold);

    ThreadPoolExecutor<int> tpe(2);

    std::future<int> f1 = tpe.execute(std::bind(timeIt, test_std_queue));
    std::future<int> f2 = tpe.execute(std::bind(timeIt, test_my_queue));
    std::future<int> f3 = tpe.execute(std::bind(timeIt, test_std_queue));
    std::future<int> f4 = tpe.execute(std::bind(timeIt, test_my_queue));
    tpe.join();
    std::cout << f1.get() << std::endl;
    std::cout << f2.get() << std::endl;
    std::cout << (f3.wait_for(std::chrono::seconds(0)) == std::future_status::ready ) << std::endl;
    std::cout << (f4.wait_for(std::chrono::seconds(0)) == std::future_status::ready)  << std::endl;
}

int main() {
    threadPoolQueusTest();
    FibonacciHeap<int> fb;

    fb.push(1);
    fb.push(3);
    fb.push(2);
    fb.push(5);
    fb.push(6);
    fb.push(4);

    std::cout << fb.top() << std::endl;
    fb.pop();
    std::cout << fb.top() << std::endl;
    fb.pop();
    std::cout << fb.top() << std::endl;
    fb.pop();
    std::cout << fb.top() << std::endl;
    fb.pop();
    std::cout << fb.top() << std::endl;
    fb.pop();
    std::cout << fb.top() << std::endl;
    fb.pop();
    fb.push(7);
    std::cout << fb.top() << std::endl;


    return 0;
}
