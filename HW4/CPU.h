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
};

// Define the CPU class
class CPU {
    bool reset;
    bool pipeline;
    tick_t currentTick;
    uint32_t pc;            // Program Counter
    uint32_t sp;            // Stack pointer
    uint32_t stackAddress;    // Start of the stack
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
    std::queue<Instruction> executedInstructions;
    std::queue<Instruction> discardedInstructions;
    
    uint32_t fpDelay;
    bool memDelay;

    // uint32_t pcEnd;         // Address of last instruction
    uint32_t prevPC;

    // Functions for flow control, pipelined and single instruction
    void updateDataPath();
    void updatePipeline();
    void flushPipeline();
    bool controlHazardCheck();
    bool dataHazardCheck();

    // Functions for fetch, decode, execute, memory, and write back stages of the data path
    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void WriteBack();

public:
    CPU(RAM& ram, uint32_t pc, uint32_t stackAddress, bool pipeline) 
    : reset(false), pipeline(pipeline), currentTick(0), pc(pc), sp(stackAddress), stackAddress(stackAddress), ram(ram){ 
        // Initialize registers to 0
        for (int i = 0; i < 32; ++i) {
            registers.int_regs[i] = 0;
            registers.fp_regs[i] = 0.0f;
        }
        // Updates value of last address
        // pcEnd = pc;
        // while (ram.Read(pcEnd) != 0) {
        //     pcEnd += 4;
        // }

        fpDelay = 0;
        memDelay = 0;

    };

    bool checkReset() { return reset; }

    // Function to simulate the CPU for 1 cycle
    void runCPUcycle();

    // Functions to handle the event queue
    void printEventQueue();
    void printCurrentEvent();
    void updateEventQueue();

    void printExecutedInstructions();
    void printRegisters();

    // the register functions are only used outside of the cpu class (i.e. set up x1 for running lab 2)
    void writeIntRegister(uint32_t address, uint32_t data) {
        if (address >= 0 && address < 32)
            registers.int_regs[address] = data;
    }

    void writeFPRegister(uint32_t address, float data) {
        if (address >= 0 && address < 32)
            registers.fp_regs[address] = data;
    }

    int32_t readIntRegister(int regIndex) {
        if (regIndex >= 0 && regIndex < 32) {
            return registers.int_regs[regIndex];
        } else {
            std::cerr << "Error: Integer register index out of bounds." << std::endl;
            return 0;
        }
    }

    float readFloatRegister(int regIndex) {
        if (regIndex >= 0 && regIndex < 32) {
            return registers.fp_regs[regIndex];
        } else {
            std::cerr << "Error: Float register index out of bounds." << std::endl;
            return 0;
        }
    }
 
};

#endif //CPU_H