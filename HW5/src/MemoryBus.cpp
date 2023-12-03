#include "MemoryBus.h"

void MemoryBus::requestBusAccess(int cpuID) {
    if (!busInUse) {
        // Bus is available, grant access to the requesting processor
        busInUse = true;
        currentProcessorID = cpuID;
    } else {
        std::cerr << "Memory bus is already in use. Access denied for processor " << cpuID << "\n";
    }
}

void MemoryBus::releaseBusAccess(int cpuID) {
    if (busInUse && currentProcessorID == cpuID) {
        // Release the bus if it was granted to the releasing processor
        busInUse = false;
        currentProcessorID = -1;  // Reset to an invalid ID
    } else {
        std::cerr << "Error: Processor " << cpuID << " trying to release bus not granted to it." << "\n";
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
    busArbiter.requestAccess(cpuID);
}

MemoryAccessRequest MemoryBus::getNextRequest() {
    MemoryAccessRequest request = requestQueue.front();
    requestQueue.pop();
    return request;
}