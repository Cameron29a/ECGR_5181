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
    bool reset;

public:
    CPU (RAM& ram) : ram(ram), pc(0), reset(false) { };

    bool checkReset() { return reset; }

    // Functions for fetch, decode, execute, and store stages
    void Fetch();
    void Decode();
    void Execute();
    void Store();

    // Function to print the event queue
    void printEvents();

    // Function to simulate the CPU for 1 cycle
    void runCPU();
};

#endif //CPU_H