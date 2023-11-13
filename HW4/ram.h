#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>
#include <cstring>

class RAM {
    std::vector<uint8_t> memory;

public:
    RAM(uint32_t size) : memory(size, 0) {}

    // Read data from memory at the specified address (2 CPU cycles)
    uint32_t Read(uint32_t address) {
        // std::cout << "Memory Read from Address 0x" << std::hex << address << std::dec << "\n";
        uint32_t result = 0;
        for (int i = 0; i <= 3; i++) {
            uint32_t byte = static_cast<uint32_t>(memory[address + i]);
            result |= (byte << (i * 8));
        }
        return result;
    }

    // Write data to memory at the specified address (2 CPU cycles)
    void Write(uint32_t address, uint32_t data) {
        // std::cout << "Memory Write to Address 0x" << std::hex << address << std::dec << "\n";
        for (int i = 0; i <= 3; i++) {
            uint32_t byte = (data >> (i * 8)) & 0xFF;
            memory[address + i] = byte;
        }
    }

    // Read float from memory at the specified address
    float ReadFloat(uint32_t address) {
        uint32_t floatData = Read(address);
        float result;
        std::memcpy(&result, &floatData, sizeof(float));
        return result;
    }

    // Write float to memory at the specified address
    void WriteFloat(uint32_t address, float data) {
        uint32_t floatData;
        std::memcpy(&floatData, &data, sizeof(uint32_t));
        Write(address, floatData);
    }

    // Print the contents of the memory map
    void PrintMemoryContents() {
        std::cout << "MMMMMMMMMMMMMMMMMMMMMM-Memory Contents:-MMMMMMMMMMMMMMMMMMMMMM\n";
        for (uint32_t address = 0; address <= memory.size(); address++) {
            std::cout << "Address 0x" << std::hex << address << ": 0b";
            uint8_t data = static_cast<int>(memory[address]);
            // Print the single byte stored in each address
            // Decrementing for loop = little endiness
            for (int i = 7; i >= 0; i--) {
                std::cout << std::hex << ((data >> i) & 0b1);
            }
            std::cout << "\n";
        }
    }

};

#endif //RAM_H