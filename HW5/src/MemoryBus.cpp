#include "MemoryBus.h"

void MemoryBus::requestBusAccess(int processorID) {

}

void MemoryBus::releaseBusAccess(int processorID) {

}

uint64_t MemoryBus::readFromMemory(uint64_t address) {
    return ram.read(address);
}


void MemoryBus::writeToMemory(uint64_t address, uint64_t data) {
    ram.write(address, data);
}