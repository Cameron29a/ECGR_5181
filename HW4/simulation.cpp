#include "simulation.h"

// Function to print the events
inline void Simulation::printEvents() {
    // std::cout << "Header";
    while (!systemEvents.empty()) {
        systemEvents.front().print();
        systemEvents.pop();
    }
}

inline void Simulation::loadInstructionsToMemory(const std::string& filename, RAM &memory, uint32_t startAddress) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file: " << filename << "/n";
        return;
    }

    std::string line;
    int address = startAddress;

    while (std::getline(file, line)) {
        try {
            // Convert the binary string to an integer (assuming the line contains binary machine code)
            uint32_t instruction = std::stoi(line, nullptr, 2);

            // Store the instruction in memory at the specified address
            int byte1 = (instruction >> 24) & 0xFF;
            int byte2 = (instruction >> 16) & 0xFF;
            int byte3 = (instruction >> 8) & 0xFF;
            int byte4 = instruction & 0xFF;
            memory.Write(address++, byte1);
            memory.Write(address++, byte2);
            memory.Write(address++, byte3);
            memory.Write(address++, byte4);

            // Increment the address for the next instruction
            // address++;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid binary format in the file." << std::endl;
            break; // Exit the loop
        }
    }

    file.close();
}

inline void Simulation::runSimulation() {
    std::cout << "Begin System Initialization\n";

    std::cout << "Create Virtual Memory\n";
    RAM memory(256);

    // write instructions to addresses 0x0 – 0x093
    // std::string filename = "instructions.txt";
    // uint32_t startAddress = 0x0;
    // loadInstructionsToMemory(filename, memory, startAddress);
    memory.PrintMemoryContents();
    // addresses 0x200 - 0x2FF will allocated for the stack

    // address ranges 0x400 - 0x7FF (ARRAY_A) and 0x800 - 0xBFF (ARRAY_B) will be initialized as arrays of random FP32 values.


}