#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

typedef uint64_t tick_t;

class Event {
    tick_t simTick;
    bool scheduled;
    
public:
    Event(void) : simTick(0), scheduled(false) { }
    void schedule(tick_t newTime) {
		simTick = newTime;
		scheduled = true;
	}
	void deschedule() {
		simTick = -1;
		scheduled = false;
	}
    tick_t getTime() { return simTick; }
    bool isScheduled() { return scheduled; }

    // Function to print the details of an event
    void print() const {
        std::cout << "Clock: " << simTick << "\n";
    }

};

#endif //EVENT_H