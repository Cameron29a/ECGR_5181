#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    // Increment the PC

}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values

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
    while (pc < instruction_memory.size()) {
        Fetch();
        Decode();
        Execute();
        Store();
    }
}