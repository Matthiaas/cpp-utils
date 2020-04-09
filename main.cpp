#include <iostream>

#include "datastructures/Queue.h"
#include "parallel/ThreadPoolExecutor.h"
#include "datastructures/FibonacciHeap.h"
#include "datastructures/Heap.h"

#include <array>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <vector>

#include <boost/heap/fibonacci_heap.hpp>


using namespace std::chrono;

using data_type = int;

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


void testMyFibonacci(std::vector<int>& arr, data_type& data, int push_threshold) {

    FibonacciHeap<int> q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                data_type a = q.top();
                q.pop();
            }
        }
    }
}

void testStdPrioQueu(std::vector<int>& arr, data_type& data, int push_threshold) {

    std::priority_queue<int, std::vector<int>, std::greater<int>> q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                data_type a = q.top();
                q.pop();
            }
        }
    }
}


void testMyPrioQueu(std::vector<int>& arr, data_type& data, int push_threshold) {

    Heap<int> q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                data_type a = q.top();
                q.pop();
            }
        }
    }
}

void testBoostFibonacciHeap(std::vector<int>& arr, data_type& data, int push_threshold) {
    boost::heap::fibonacci_heap<int> q;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                data_type a = q.top();
                q.pop();
            }
        }
    }
}

void ValidateFibonacciHeap(std::vector<int>& arr, data_type& data, int push_threshold) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> q;
    Heap<int> q2;
    for(int i = 0; i< arr.size(); i++) {
        int r = arr[i];
        if(r < push_threshold) {
            q.push(r);
            q2.push(r);
        } else {
            size_t size = q.size();
            if( size > 0) {
                data_type a = q.top();
                if(a != q2.top()) {
                    std::cout << "WRONG" << std::endl;
                }
                q.pop();
                q2.pop();

            }
        }
    }
    std::cout << "Validated" << std::endl;
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
    int N = 5000000;
    // Should be between 1 and 100;
    int push_threshold = 80;
    data_type data;

    std::vector<int>  arr;
    std::srand(std::time(nullptr));
    for(int i = 0; i< N; i++) {
        arr.push_back(std::rand() % 100);
    }

    std::function<void()> test_std_queue = std::bind(testStdQueue, arr, data, push_threshold);
    std::function<void()> test_my_queue = std::bind(testMyQueue, arr, data, push_threshold);
    std::function<void()> test_std_prioqueue = std::bind(testStdPrioQueu, arr, data, push_threshold);
    std::function<void()> test_my_prioqueue = std::bind(testStdPrioQueu, arr, data, push_threshold);
    std::function<void()> test_my_fibonacciHeap = std::bind(testMyFibonacci, arr, data, push_threshold);
    std::function<void()> test_boost_fibonacciHeap = std::bind(testBoostFibonacciHeap, arr, data, push_threshold);

    ThreadPoolExecutor<int> tpe(1);

    //std::future<int> f1 = tpe.execute(std::bind(timeIt, test_std_queue));
    //std::future<int> f2 = tpe.execute(std::bind(timeIt, test_my_queue));
    std::future<int> f3 = tpe.execute(std::bind(timeIt, test_std_prioqueue));
    std::future<int> f4 = tpe.execute(std::bind(timeIt, test_my_prioqueue));
    std::future<int> f5 = tpe.execute(std::bind(timeIt, test_my_fibonacciHeap));
    std::future<int> f6 = tpe.execute(std::bind(timeIt, test_boost_fibonacciHeap));
    tpe.join();
    //std::cout << "STD QUEUE: " << f1.get() << std::endl;
    //std::cout << "MY QUEUE: " << f2.get() << std::endl;
    std::cout << "STD PRIO QUEUE: " << f3.get() << std::endl;
    std::cout << "MY HEAP: " << f4.get() << std::endl;
    std::cout << "FIBONACCI HEAP: " << f5.get()  << std::endl;
    std::cout << "BOOST FIBONACCI HEAP: " << f6.get()  << std::endl;
}

int main() {
  threadPoolQueusTest();




    return 0;
}
