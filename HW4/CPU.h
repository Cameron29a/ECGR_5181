#ifndef CPU_H
#define CPU_H

#include <queue>
#include <vector>

#include <bitset>

#include "event.h"
#include "instruction.h"
#include "ALU.h"
#include "branchCtl.h"
#include "ram.h"

// Define RISC-V registers
struct Registers {
    int32_t int_regs[32];
    float fp_regs[32];

    // uint32_t readIntRegister(uint32_t address) {
    //     if (address >= 0 && address < 32)
    //         return int_regs[address];
    //     else
    //         return 0;
    // }

    void writeIntRegister(uint32_t address, uint32_t data) {
        if (address >= 0 && address < 32)
            int_regs[address] = data;
    }

    // float readFPRegister(uint32_t address) {
    //     if (address >= 0 && address < 32)
    //         return fp_regs[address];
    //     else 
    //         return 0;
    // }

    void writeFPRegister(uint32_t address, float data) {
        if (address >= 0 && address < 32)
            fp_regs[address] = data;
    }

};

// Define the CPU class
class CPU {
    bool reset;
    tick_t currentTick;
    uint32_t pc;            // Program Counter
    uint32_t sp;            // Stack pointer
    uint32_t stackStart;    // Start of the stack
    RAM& ram;

    ALU alu;
    BranchCtl branchCtl;
    Registers registers;
    std::queue<Event> events;
    std::queue<Instruction> fetchStage;
    std::queue<Instruction> decodeStage;
    std::queue<Instruction> executeStage;
    std::queue<Instruction> memoryStage;
    std::queue<Instruction> writeBackStage;
    uint32_t prevPC;

public:
    CPU(RAM& ram, uint32_t stackStart) : reset(false), currentTick(0), pc(0), sp(stackStart), stackStart(stackStart), ram(ram) { 
        // Initialize registers to 0
        for (int i = 0; i < 32; ++i) {
            registers.int_regs[i] = 0;
            registers.fp_regs[i] = 0.0f;
        }
    };

    bool checkReset() { return reset; }

    void updatePipeline();

    // Functions for fetch, decode, execute, memory, and write back stages
    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void WriteBack();

    // Function to print the event queue
    void printEvents();
    void updateEventQueue();


    void printRegisters();

    // Function to simulate the CPU for 1 cycle
    void runCPU();
};

#endif //CPU_H