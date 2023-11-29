#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

typedef uint64_t tick_t;

class Event {
    tick_t simTick;
    uint32_t prevPC;
    uint32_t pc;
    std::string fetchString;
    std::string decodeString;
    std::string executeString;
    std::string memoryString;
    std::string writeBackString;
    
public:
    Event(tick_t clk, uint32_t prevPC, uint32_t pc, const std::string& fetchString, const std::string& decodeString,
    const std::string& executeString, const std::string& memoryString,
    const std::string& writeBackString)
    : simTick(clk), prevPC(prevPC), pc(pc), fetchString(fetchString), decodeString(decodeString),
      executeString(executeString), memoryString(memoryString),
      writeBackString(writeBackString) {}

    tick_t getTime() { return simTick; }
    void print();
};

#endif //EVENT_H