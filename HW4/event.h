#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

typedef uint64_t tick_t;

class Event {
    tick_t simTick;
    std::string fetchString;
    std::string decodeString;
    std::string executeString;
    std::string memoryString;
    std::string writeBackString;
    
public:
    Event(tick_t clk, const std::string& fetchString, const std::string& decodeString,
    const std::string& executeString, const std::string& memoryString,
    const std::string& writeBackString)
    : simTick(clk), fetchString(fetchString), decodeString(decodeString),
      executeString(executeString), memoryString(memoryString),
      writeBackString(writeBackString) {}

    tick_t getTime() { return simTick; }

    // Function to print the details of an event
    void print() const {
        std::cout << "Clock: " << simTick << "\n" 
        << "Fetch Stage: " << fetchString << "\n" 
        << "Decode Stage: " << decodeString << "\n" 
        << "Execute Stage: " << executeString << "\n" 
        << "Memory Stage: " << memoryString << "\n" 
        << "Write Back Stage: " << writeBackString << "\n\n";

    }

};

#endif //EVENT_H