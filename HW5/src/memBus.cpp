#include "memBus.h"

uint32_t MemBus::readInstruction(uint32_t pc, uint32_t instructionStart) {
    // return readInstructionPort(pc + instructionStart);
    return 0;
}

// Method to process memory requests
void MemBus::processMemoryRequests() {
    // Process requests for RAM
    processMemoryBankRequests();
}

// Method to add memory request to the bus
void MemBus::addMemoryRequest(uint32_t cpuId, MemoryRequest request) {
    memoryRequests.push({cpuId, request});
}

// Method to check if a memory request is complete
bool MemBus::isMemoryRequestComplete() {
    return !memoryResponses.empty();
    // bool response = !memoryResponses.empty();
    // return response;
}

// Method to get the response from the completed memory request
uint32_t MemBus::getMemoryResponse(uint32_t cpuId) {
    if (!memoryResponses.empty()) {
        auto response = memoryResponses.front();
        if (response.first == cpuId) {
            memoryResponses.pop();
            return response.second;
        }
    }
    return 0; // Placeholder value, adjust as needed
}

bool MemBus::isWriteRequestPending(uint32_t cpuId){
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

uint32_t MemBus::readInstructionPort(uint32_t address) {
    return ram.Read(address);
}

// Method to process memory requests for RAM
void MemBus::processMemoryBankRequests() {
    if (!memoryRequests.empty()) {
        auto [cpuId, request] = memoryRequests.front();
        memoryRequests.pop();
        std::cout << "Memory Bus Access:\n";
        // Check if the address is already being accessed
        if (request.type == MemoryRequestType::READ) {
            // Read from memory
            uint32_t data = ram.Read(request.address);
            std::cout << "CPU " << cpuId << " reads from address: 0x" << std::hex << request.address << std::dec << ": 0b" << std::bitset<32>(data) << "\n";
            memoryResponses.push({cpuId, data});
        } else {
            // Write to memory
            ram.Write(request.address, request.data);
            std::cout << "CPU " << cpuId << " writes to address " << std::hex << request.address << std::dec << ": " << std::bitset<32>(request.data) << "\n";
        }
    }
}