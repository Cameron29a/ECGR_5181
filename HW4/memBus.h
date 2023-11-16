#include <iostream>
#include <vector>
#include <queue>

#include "ram.h"

enum class MemoryRequestType { READ, WRITE };

struct MemoryRequest {
    MemoryRequestType type;
    uint32_t address;
    uint32_t data;
};

class MemoryBus {
public:
    // Constructor to initialize memory banks
    MemoryBus(RAM& ram) : ram(ram) {}

    // Method to process memory requests
    void processMemoryRequests() {
        // Process requests for RAM
        processMemoryBankRequests();
    }

    // Method to add memory request to the bus
    void addMemoryRequest(uint32_t cpuId, MemoryRequest request) {
        memoryRequests.push({cpuId, request});
    }

private:
    // Define Memory Request Queue type
    using MemoryRequestQueue = std::queue<std::pair<uint32_t, MemoryRequest>>;

    // RAM
    RAM& ram;

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
                std::cout << "CPU " << cpuId << " reads from address " << request.address << ": " << ram.Read(request.address) << std::endl;
            } else {
                // Write to memory
                ram.Write(request.address, request.data);
                std::cout << "CPU " << cpuId << " writes to address " << request.address << ": " << request.data << std::endl;
            }
        }
    }
};

// Define CPU class
class CPU {
public:
    // Constructor to initialize CPU with a memory bus and ID
    CPU(MemoryBus& bus, uint32_t id) : memoryBus(bus), cpuId(id) {}

    // Method to make a memory read request
    void readMemory(uint32_t address) {
        MemoryRequest request{MemoryRequestType::READ, address};
        memoryBus.addMemoryRequest(cpuId, request);
    }

    // Method to make a memory write request
    void writeMemory(uint32_t address, uint32_t data) {
        MemoryRequest request{MemoryRequestType::WRITE, address, data};
        memoryBus.addMemoryRequest(cpuId, request);
    }

private:
    MemoryBus& memoryBus;
    uint32_t cpuId;
};

int main() {
    // Create RAM
    RAM ram(0xFF);

    // Create Memory Bus with RAM
    MemoryBus memoryBus(ram);

    // Create two CPUs with the same memory bus
    CPU cpu1(memoryBus, 1);
    CPU cpu2(memoryBus, 2);

    // Simulate memory access
    cpu1.readMemory(0x1000);
    cpu2.writeMemory(0x1000, 42);

    // Process memory requests on the memory bus
    memoryBus.processMemoryRequests();

    return 0;
}