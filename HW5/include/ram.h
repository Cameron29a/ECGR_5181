#ifndef RAM_H
#define RAM_H

#include <iostream>
#include <vector>
#include <cstring>

class RAM {
    std::vector<uint8_t> memory;

public:
    RAM(uint32_t size) : memory(size, 0) {}

    uint32_t Read(uint32_t);
    void Write(uint32_t, uint32_t);
    void PrintMemoryContents();
};

#endif //RAM_H