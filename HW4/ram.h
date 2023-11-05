#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>

class RAM {
    std::vector<uint8_t> memory;

public:
    RAM(uint32_t size) : memory(size) {}

    // Read data from memory at the specified address (2 CPU cycles)
    uint32_t Read(uint32_t* address) {

        std::cout << "Memory Read from Address 0x" << std::hex << address << std::dec << std::endl;

        return memory[address];
    }

    // Write data to memory at the specified address (2 CPU cycles)
    void Write(uint32_t* address, uint8_t data) {

        std::cout << "Memory Write to Address 0x" << std::hex << address << std::dec << std::endl;
        
        memory[address] = data;
    }
    
};

#endif //RAM_H