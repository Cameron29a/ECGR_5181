#ifndef MEMBUS_H
#define MEMBUS_H

#include <queue>
#include <iostream>
#include <functional>
#include <bitset>

#include "ram.h"

enum class MemoryRequestType { READ, WRITE };

struct MemoryRequest {
    MemoryRequestType type;
    uint32_t address;
    uint32_t data;
};

class MemBus {
public:
    // Constructor to initialize memory banks
    MemBus(RAM& ram) : ram(ram) {}

    uint32_t readInstruction(uint32_t, uint32_t);


    void processMemoryRequests();
    void addMemoryRequest(uint32_t, MemoryRequest);
    bool isMemoryRequestComplete();
    uint32_t getMemoryResponse(uint32_t);
    bool isWriteRequestPending(uint32_t);

private:
    // Define Memory Request Queue type
    using MemoryRequestQueue = std::queue<std::pair<uint32_t, MemoryRequest>>;
    std::queue<std::pair<uint32_t, uint32_t>> memoryResponses;

    RAM& ram;

    uint32_t readInstructionPort(uint32_t);

    // Memory Requests Queue
    MemoryRequestQueue memoryRequests;

    void processMemoryBankRequests();
};

#endif //MEMBUS