#include "event.h"

// Function to print the details of an event
    void Event::print() {
        std::cout << "****************************************\n" 
        << "Clock ticks: " << simTick << "\n" 
        << "PC at start of cycle: 0x" << std::hex << prevPC << std::dec << "\n"
        << "PC at end of cycle: 0x" << std::hex << pc << std::dec << "\n"
        << "Fetch Stage: " << fetchString << "\n" 
        << "Decode Stage: " << decodeString << "\n" 
        << "Execute Stage: " << executeString << "\n" 
        << "Memory Stage: " << memoryString << "\n" 
        << "Write Back Stage: " << writeBackString << "\n"
        << "****************************************\n";
    }