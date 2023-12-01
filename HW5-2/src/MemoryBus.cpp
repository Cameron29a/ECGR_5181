#include "MemoryBus.h"

void MemoryBus::requestBusAccess(int processorID) {
    if (!busInUse) {
        // Bus is available, grant access to the requesting processor
        busInUse = true;
        currentProcessorID = processorID;
    } else {
        std::cerr << "Memory bus is already in use. Access denied for processor " << processorID << "\n";
    }
}

void MemoryBus::releaseBusAccess(int processorID) {
    if (busInUse && currentProcessorID == processorID) {
        // Release the bus if it was granted to the releasing processor
        busInUse = false;
        currentProcessorID = -1;  // Reset to an invalid ID
    } else {
        std::cerr << "Error: Processor " << processorID << " trying to release bus not granted to it." << "\n";
    }
}

uint64_t MemoryBus::readFromMemory(uint64_t address) {
    return ram.read(address);
}


void MemoryBus::writeToMemory(uint64_t address, uint64_t data) {
    ram.write(address, data);
}

void MemoryBus::addRequest(int cpuID, bool isRead, uint64_t address, uint64_t data) {
    requestQueue.push(MemoryAccessRequest(cpuID, isRead, address, data));
}

MemoryAccessRequest MemoryBus::getNextRequest() {
    MemoryAccessRequest request = requestQueue.front();
    requestQueue.pop();
    return request;
}