#ifndef RAM_H
#define RAM_H

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <ostream>

class Ram {
    std::unordered_map<uint64_t, uint64_t> memoryData;

public:
    uint64_t read(uint64_t address) {
         // Check if the address is present in the memoryData map
        auto it = memoryData.find(address);

        if (it != memoryData.end()) {
            // Return the data associated with the address
            return it->second;
        } else {
            std::cerr << "Read from uninitialized memory address: " << std::hex << address << std::endl;
            return -1; 
        }
    }

    void write(uint64_t address, uint64_t data) {
        memoryData[address] = data;
    }

};

#endif //RAM_H
