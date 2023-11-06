#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    uint32_t nextInstruction = 0;
    for(int i = 0; i <= 3; i++) {
        uint32_t byte = ram.Read(pc++);
        nextInstruction = nextInstruction | (byte << (i * 8));

        std::bitset<8> byteBinary(byte);
        std::cout << "Byte " << i << ": 0b" << byteBinary << "\n";
    }

    // uint32_t byte0 = ram.Read(pc);
    // uint32_t byte1 = ram.Read(pc+1) << 8;
    // uint32_t byte2 = ram.Read(pc+2) << 16;
    // uint32_t byte3 = ram.Read(pc+3) << 24;
    // nextInstruction = byte0 | byte1 | byte2 | byte3;


    instructionMemory.push(nextInstruction);

    // // Adjust PC if there is a branch or jump
    // if (checkBranch() == 1 || checkJump() == 1) {
    //     pc = getImm();
    // }
}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    // Instruction currentInstruction = instructionMemory.front();
    // currentInstruction.printAssembly();
    // currentInstruction.printInstruction();
    instructionMemory.front().printAssembly();
    instructionMemory.front().printInstruction();
}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands

}

inline void CPU::Store() {
    // Perform memory store operation if required

}

inline void CPU::memLoad(uint32_t address, uint32_t& data) {
    // Read data from memory at the specified address

}

inline void CPU::memStore(uint32_t address, uint32_t data) {
    // Store data into memory at the specified address

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