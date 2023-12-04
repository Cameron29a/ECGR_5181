#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>
#include <cstring>

class Ram {
    std::vector<uint8_t> memory;

public:
    Ram() {}

    // Read data from memory at the specified address (2 CPU cycles)
    uint32_t read(uint32_t address) {
        // std::cout << "Memory Read from Address 0x" << std::hex << address << std::dec << "\n";
        uint32_t result = 0;
        for (int i = 0; i <= 3; i++) {
            uint32_t byte = static_cast<uint32_t>(memory[address + i]);
            result |= (byte << (i * 8));
        }
        return result;
    }

    // Write data to memory at the specified address (2 CPU cycles)
    void write(uint32_t address, uint32_t data) {
        // std::cout << "Memory Write to Address 0x" << std::hex << address << std::dec << "\n";
        for (int i = 0; i <= 3; i++) {
            memory[address + i] = (data >> (i * 8)) & 0xFF;
        }
    }

};

#endif //RAM_H