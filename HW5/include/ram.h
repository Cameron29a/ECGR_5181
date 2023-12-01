#ifndef RAM_H
#define RAM_H

#include <cstdint>
#include <unordered_map>

class Ram {
    std::unordered_map<uint64_t, uint64_t> memoryData;

public:
    uint64_t read(uint64_t address) {
        return 0;
    }

    void write(uint64_t address, uint64_t data) {

    }

};

#endif //RAM_H