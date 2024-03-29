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
    std::cout << "Memory Bus Reads from main memory\n";
    return ram.read(address);
}


void MemoryBus::writeToMemory(uint64_t address, uint64_t data) {
    std::cout << "Memory Bus Writes from main memory\n";
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

MemoryAccessRequest MemoryBus::getRequestByCPUID(int cpuID) {
    // Create a temporary queue for processing
    std::queue<MemoryAccessRequest> tempQueue = requestQueue;

    // Iterate through the temporary queue to find the request with the specified CPU ID
    while (!tempQueue.empty()) {
        MemoryAccessRequest request = tempQueue.front();
        tempQueue.pop();

        if (request.cpuID == cpuID) {
            // Found the matching request in the temporary queue
            // Now remove the request from the original queue
            std::queue<MemoryAccessRequest> newQueue;
            while (!requestQueue.empty()) {
                if (requestQueue.front().cpuID != cpuID) {
                    newQueue.push(requestQueue.front());
                }
                requestQueue.pop();
            }
            requestQueue = newQueue;

            return request;
        }
    }

    // If no matching request is found, return a default-initialized request
    return MemoryAccessRequest(-1, false, 0);  // Modify this as needed
}