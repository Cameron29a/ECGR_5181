#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <queue>

typedef uint64_t tick_t;

class Event {
    tick_t simTick;
    std::queue<int> requestQueue;
    
public:
    Event(tick_t clk, std::queue<int> requestQueue) : simTick(clk), requestQueue(requestQueue) {}

    tick_t getTime() { return simTick; }

    void print() {
        std::cout << "****************************************\n" 
        << "Current Clock Tick: " << simTick << "\n" 
        << "Current BusArbiter Queue: "; printQueue(requestQueue);
        std::cout << "Next item here: " << 0 << "\n";


        std::cout << "****************************************\n";
    }


    void printQueue(std::queue<int> queue) {
        bool firstItem = true;
        std::cout << "{ ";
        while (!queue.empty()) {
            if(firstItem) {
                firstItem = false;
            } else {
                std::cout << ", ";
            }
            std::cout << queue.front();
            queue.pop();
        }
        std::cout << " }\n";
    }



};

#endif //EVENT_H