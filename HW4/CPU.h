#ifndef CPU_H
#define CPU_H

#include <queue>
#include <vector>

#include <bitset>

#include "event.h"
#include "instruction.h"
#include "ram.h"


// Define RISC-V registers
struct Registers {
    int32_t int_regs[32];
    float fp_regs[32];
};

// Define the CPU class
class CPU {
    std::queue<Event> events;  // Event queue for CPU
    RAM& ram;
    Registers registers;
    std::queue<Instruction> instructionMemory;
    uint32_t pc;

public:
    CPU (RAM& ram) : ram(ram), pc(0) {};

    uint32_t getPC() { return pc; }

    // Methods for fetch, decode, execute, and store stages
    void Fetch();
    void Decode();
    void Execute();
    void Store();

    // Additional methods for memory operations
    void memLoad(uint32_t address, uint32_t& data);
    void memStore(uint32_t address, uint32_t data);

    // Function to print the event queue
    void printEvents();

    // Function to simulate the CPU for 1 cycle
    void runCPU();
};

#endif //CPU_H