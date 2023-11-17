#ifndef MEMBUS_H
#define MEMBUS_H

#include <queue>
#include <iostream>
#include <functional>

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

    uint32_t readInstruction(uint32_t pc, uint32_t instructionStart) {
        return readInstructionPort(pc + instructionStart);
    }

    // Method to process memory requests
    void processMemoryRequests() {
        // Process requests for RAM
        processMemoryBankRequests();
    }

    // Method to add memory request to the bus
    void addMemoryRequest(uint32_t cpuId, MemoryRequest request) {
        memoryRequests.push({cpuId, request});
    }

    // Method to check if a memory request is complete
    bool isMemoryRequestComplete() const {
        return !memoryResponses.empty();
    }

    // Method to get the response from the completed memory request
    uint32_t getMemoryResponse() {
        if (!memoryResponses.empty()) {
            auto response = memoryResponses.front();
            memoryResponses.pop();
            return response;
        } else {
            return 0; // Placeholder value, adjust as needed
        }
    }

    bool isWriteRequestPending(uint32_t cpuId) const {
        auto requestsCopy = memoryRequests;  // Make a copy to avoid modifying the original queue
        while (!requestsCopy.empty()) {
            const auto& request = requestsCopy.front();
            if (request.first == cpuId && request.second.type == MemoryRequestType::WRITE) {
                return true;
            }
            requestsCopy.pop();
        }
        return false;
    }

private:
    // Define Memory Request Queue type
    using MemoryRequestQueue = std::queue<std::pair<uint32_t, MemoryRequest>>;
    std::queue<uint32_t> memoryResponses;

    RAM& ram;

    uint32_t readInstructionPort(uint32_t address) {
        return ram.Read(address);
    }

    // Memory Requests Queue
    MemoryRequestQueue memoryRequests;


    // Method to process memory requests for RAM
    void processMemoryBankRequests() {
        if (!memoryRequests.empty()) {
            auto [cpuId, request] = memoryRequests.front();
            memoryRequests.pop();

            // Check if the address is already being accessed
            if (request.type == MemoryRequestType::READ) {
                // Read from memory
                uint32_t data = ram.Read(request.address);
                std::cout << "CPU " << cpuId << " reads from address " << request.address << ": " << data << std::endl;
                memoryResponses.push(data);
            } else {
                // Write to memory
                ram.Write(request.address, request.data);
                std::cout << "CPU " << cpuId << " writes to address " << request.address << ": " << request.data << std::endl;
            }
        }
    }
};

#endif //MEMBUS