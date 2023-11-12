#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

typedef uint64_t tick_t;

class Event {
    tick_t simTick;
    uint32_t pc;
    uint32_t instructionCnt;
    std::string fetchString;
    std::string decodeString;
    std::string executeString;
    std::string memoryString;
    std::string writeBackString;
    
public:
    Event(tick_t clk, uint32_t pc, uint32_t instructionCnt, const std::string& fetchString, const std::string& decodeString,
    const std::string& executeString, const std::string& memoryString,
    const std::string& writeBackString)
    : simTick(clk), pc(pc), instructionCnt(instructionCnt), fetchString(fetchString), decodeString(decodeString),
      executeString(executeString), memoryString(memoryString),
      writeBackString(writeBackString) {}

    tick_t getTime() { return simTick; }

    // Function to print the details of an event
    void print() const {
        std::cout << "****************************************\n" 
        << "Clock ticks: " << simTick << "\n" 
        << "Current PC: 0x" << std::hex << pc << std::dec << "\n"
        << "**Instruction Count: " << instructionCnt << "\n"    //change to executed instructions later; move from fetch to execute
        << "Fetch Stage: " << fetchString << "\n" 
        << "Decode Stage: " << decodeString << "\n" 
        << "Execute Stage: " << executeString << "\n" 
        << "Memory Stage: " << memoryString << "\n" 
        << "Write Back Stage: " << writeBackString << "\n"
        << "****************************************\n";
    }
};

#endif //EVENT_H