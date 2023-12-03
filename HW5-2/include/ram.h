#ifndef RAM_H
#define RAM_H

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <ostream>

class Ram {
    std::unordered_map<uint64_t, uint64_t> memoryData;

public:
    Ram() {
        for (uint64_t address = 0; address < 1024; ++address) {
            memoryData[address] = 0; // Prefill with 0
        }
    }
    
    uint64_t read(uint64_t address) {
         // Check if the address is present in the memoryData map
        auto it = memoryData.find(address);
    std::cout << "RAM Read at Address: " << address << ", Data: " << memoryData[address] << std::endl;

        if (it != memoryData.end()) {
            // Return the data associated with the address
            return it->second;
        } else {
            std::cerr << "Read from uninitialized memory address: " << std::hex << address << std::endl;
            return -1; 
        }
    }

    void write(uint64_t address, uint64_t data) {
        std::cout << "RAM Write at Address: " << address << ", Data: " << data << std::endl;

        memoryData[address] = data;
    }

};

#endif //RAM_H
