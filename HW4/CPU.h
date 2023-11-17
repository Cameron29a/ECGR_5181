#ifndef CPU_H
#define CPU_H

#include <queue>
#include <vector>

#include <bitset>

#include "event.h"
#include "instruction.h"
#include "ALU.h"
#include "branchCtl.h"
#include "memBus.h"

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
    uint32_t instructionStart;
    uint32_t sp;            // Stack pointer
    uint32_t stackAddress;    // Start of the stack
    MemBus& memBus;
    uint32_t cpuId;

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

    bool controlHazard;
    bool dataHazard;

    // uint32_t pcEnd;         // Address of last instruction
    uint32_t prevPC;

    bool memRequestPending;

    // Functions for flow control, pipelined and single instruction
    void updateDataPath();
    void updatePipeline();
    void flushPipeline();
    void controlHazardCheck();
    void dataHazardCheck();

    // Functions for fetch, decode, execute, memory, and write back stages of the data path
    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void WriteBack();

public:
    CPU(MemBus& bus, uint32_t id, uint32_t instructionStart, uint32_t stackAddress, bool pipeline) 
    : reset(false), pipeline(pipeline), currentTick(0), pc(0), instructionStart(instructionStart), 
      sp(stackAddress), stackAddress(stackAddress), memBus(bus), cpuId(id) { 
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
        controlHazard = false;
        dataHazard = false;
        memRequestPending = false;

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

    // Register Access Functions
    void writeIntRegister(uint32_t address, uint32_t data);
    void writeFPRegister(uint32_t address, float data);
    int32_t readIntRegister(int regIndex);
    float readFloatRegister(int regIndex);

    // Memory Access Functions
    void Read(uint32_t);
    void Write(uint32_t, uint32_t);
    void ReadFloat(uint32_t);
    void WriteFloat(uint32_t, float);
 
};

#endif //CPU_H