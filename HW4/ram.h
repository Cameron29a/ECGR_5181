#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>

class RAM {
    std::vector<uint8_t> memory;

public:
    RAM(uint32_t size) : memory(size, 0) {}

    // Read data from memory at the specified address (2 CPU cycles)
    uint32_t Read(uint32_t address) {

        // std::cout << "Memory Read from Address 0x" << std::hex << address << std::dec << "\n";

        return memory[address];
    }

    // Write data to memory at the specified address (2 CPU cycles)
    void Write(uint32_t address, uint8_t data) {

        // std::cout << "Memory Write to Address 0x" << std::hex << address << std::dec << "\n";
        
        memory[address] = data;
    }

    // Print the contents of the memory map
    void PrintMemoryContents() {
        std::cout << "***********************Memory Contents:***********************\n";
        uint32_t address = 0;
        uint8_t data;
        while (address <= memory.size()) {
            std::cout << "Address 0x" << std::hex << address << ": 0b";
            data = static_cast<int>(memory[address]);
            // Print the single byte stored in each address
            // Decrementing for loop = little endiness
            // Incrementing for loop = big endiness
            for (int i = 7; i >= 0; i--) {
            // for (int i = 0; i <= 7; i++) {
                std::cout << std::hex << ((data >> i) & 0b1);
            }
            std::cout << "\n";
            address++;
        }
    }

};

#endif //RAM_H