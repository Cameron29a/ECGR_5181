#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>
// #include <iomanip> // Include the <iomanip> header for setw and setfill

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

    // Print the contents of the memory map
    void PrintMemoryContents() {
        std::cout << "MMMMMMMMMMMMMMMMMMMMM-Memory Contents:-MMMMMMMMMMMMMMMMMMMMM\n";
        uint32_t address = 0;
        uint8_t data;
        while (address <= memory.size()) {
            std::cout << "Address 0x" << std::hex << address << ": 0b";
            data = static_cast<int>(memory[address]);
            // Print the single byte stored in each address
            // Decrementing for loop = little endiness
            for (int i = 7; i >= 0; i--) {
                std::cout << std::hex << ((data >> i) & 0b1);
            }
            std::cout << "\n";
            address++;
        }
    }
    // void PrintMemoryContents() {
    //     std::cout << "***********************Memory Contents:***********************\n";
    //     for (uint32_t address = 0; address < memory.size(); ++address) {
    //         std::cout << "Address 0x" << std::hex << address << ": 0x" << std::setw(2) << std::setfill('0') << static_cast<int>(memory[address]) << "\n";
    //     }
    // }

};

#endif //RAM_H