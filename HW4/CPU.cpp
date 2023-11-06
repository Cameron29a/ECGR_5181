#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    uint32_t nextInstruction = 0;
    std::cout << "***************Instruction Fetch***************\n";
    for(int i = 0; i <= 3; i++) {
        uint32_t byte = ram.Read(pc++);
        nextInstruction = nextInstruction | (byte << (i * 8));
        // Print out to check instructions are compiled correctly
        std::bitset<8> byteBinary(byte);
        std::cout << "Byte " << pc-1 << ": 0b" << byteBinary << "\n";
    }
    instructionMemory.push(nextInstruction);

    // // Adjust PC if there is a branch or jump
    // if (checkBranch() == 1 || checkJump() == 1) {
    //     pc = getImm();
    // }
}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    instructionMemory.front().printAssembly();
    instructionMemory.front().printInstruction();
    instructionMemory.pop();
    if (instructionMemory.empty()) reset = true;

}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands

}

inline void CPU::Store() {
    // Perform memory store operation if required

}

// Function to print the events
inline void CPU::printEvents() {
    // std::cout << "Header";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

inline void CPU::runCPU() {
    // Main simulation loop
    while (pc < instructionMemory.size()) {
        Fetch();
        Decode();
        Execute();
        Store();
    }
}