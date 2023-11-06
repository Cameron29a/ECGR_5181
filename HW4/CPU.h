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
    bool reset;
    tick_t currentTick;
    uint32_t pc;            // Program Counter
    uint32_t sp;            // Stack pointer
    uint32_t stackStart;    // Start of the stack
    RAM& ram;

    Registers registers;
    std::queue<Event> events;
    std::queue<Instruction> fetchStage;
    std::queue<Instruction> decodeStage;
    std::queue<Instruction> executeStage;
    std::queue<Instruction> writeBackStage;

public:
    CPU(RAM& ram, uint32_t stackStart) : reset(false), currentTick(0), pc(0), sp(stackStart), stackStart(stackStart), ram(ram) { };

    bool checkReset() { return reset; }

    // Functions for fetch, decode, execute, and write back stages
    void Fetch();
    void Decode();
    void Execute();
    void WriteBack();

    // Function to print the event queue
    void printEvents();

    // Function to simulate the CPU for 1 cycle
    void runCPU();
};

#endif //CPU_H