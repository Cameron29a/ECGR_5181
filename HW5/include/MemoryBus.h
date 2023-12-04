#ifndef MEMORYBUS_H
#define MEMORYBUS_H

#include <queue>
#include <iostream>
#include <cstdint>
#include <unordered_map>

#include "ram.h"
#include "BusArbiter.h"

struct MemoryAccessRequest {
    int cpuID;
    bool isRead;
    uint64_t address;
    uint64_t data;  // Additional data for write operation

    MemoryAccessRequest(int id, bool r, uint64_t addr, uint64_t d = 0)
        : cpuID(id), isRead(r), address(addr), data(d) {}
};

class MemoryBus {
    std::queue<MemoryAccessRequest> requestQueue;
    Ram ram;
    BusArbiter& busArbiter;

    bool busInUse = false;
    int currentProcessorID = -1;

public:
    MemoryBus(BusArbiter& busArbiter) : busArbiter(busArbiter) {}

    bool isBusinUse() { return busInUse; };
    int getCurrentProcessorID() { return currentProcessorID; };

    void requestBusAccess(int);
    void releaseBusAccess(int);
    
    uint64_t readFromMemory(uint64_t);
    void writeToMemory(uint64_t, uint64_t);

    void addRequest(int, bool, uint64_t, uint64_t);
    MemoryAccessRequest getNextRequest();
    MemoryAccessRequest getRequestByCPUID(int);

};


#endif //MEMORYBUS_H