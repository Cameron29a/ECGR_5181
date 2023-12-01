#ifndef MEMORYBUS_H
#define MEMORYBUS_H

#include <vector>

#include "ram.h"

class MemoryBus {
    std::vector<int> requestQueue;
    Ram ram;

public:
    MemoryBus() {}

    void requestBusAccess(int processorID);
    void releaseBusAccess(int processorID);
    uint64_t readFromMemory(uint64_t address);
    void writeToMemory(uint64_t address, uint64_t data);

};


#endif //MEMORYBUS_H