#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>

class RAM {
    std::vector<uint8_t> memory;

public:
    RAM(uint32_t size) : memory(size) {}

    // Read data from memory at the specified address (2 CPU cycles)
    uint32_t Read(uint32_t address) {

        std::cout << "Memory Read from Address 0x" << std::hex << address << std::dec << "\n";

        return memory[address];
    }

    // Write data to memory at the specified address (2 CPU cycles)
    void Write(uint32_t address, uint8_t data) {

        std::cout << "Memory Write to Address 0x" << std::hex << address << std::dec << "\n";
        
        memory[address] = data;
    }

    // Print the contents of the memory
    void PrintMemoryContents() {
        std::cout << "Memory Contents:" << std::endl;
        
        uint32_t address = 0;
        while (address <= memory.size()) {
            std::cout << "Address 0x" << std::hex << address << ": 0x";

            // Print 1 byte per line
            for (int i = 0; i < 8; ++i) {
                std::cout << memory[address];
                address++;
            }
            std::cout << "\n";
        }
    }


};

#endif //RAM_H