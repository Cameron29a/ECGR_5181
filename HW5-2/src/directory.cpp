#include "directory.h"

void Directory::updateEntry(uint64_t address, int cpuID, CacheState newState) {
    size_t index = address % numEntries;  // Simple hash function for indexing
    DirectoryEntry& entry = entries[index];

    // Update the tag and state
    entry.tag = address;  // Assuming full address is used as a tag
    entry.state = newState;

    // Update the CPU mask based on the new state
    if (newState == CacheState::MODIFIED || newState == CacheState::EXCLUSIVE) {
        // Only one CPU can have the line in these states
        entry.cpuMask.reset();
        entry.cpuMask.set(cpuID);
    } else if (newState == CacheState::SHARED) {
        // Multiple CPUs can share the line
        entry.cpuMask.set(cpuID);
    } else {
        // If state is INVALID, remove the CPU from the mask
        entry.cpuMask.reset(cpuID);
    }
}

bool Directory::isCachedElsewhere(uint64_t address, int cpuID) {
    size_t index = address % numEntries;
    const DirectoryEntry& entry = entries[index];

    if (entry.tag != address || entry.state == CacheState::INVALID) {
        // The cache line is not present or is invalid in the directory
        return false;
    }

    // Check if any other CPU has this cache line
    return entry.cpuMask.any() && !entry.cpuMask.test(cpuID);
}

uint64_t Directory::readData(uint64_t address, int requestingCpuID, Ram& ram) {
    size_t index = address % numEntries;
    DirectoryEntry& entry = entries[index];

    if (entry.tag != address) {
        // The cache line is not present in the directory, fetch from main memory
        // and update the directory entry accordingly
        uint64_t data = ram.read(address);
        entry.tag = address;
        entry.state = CacheState::SHARED;
        entry.cpuMask.reset();
        entry.cpuMask.set(requestingCpuID);
        return data;
    }

    if (entry.state == CacheState::EXCLUSIVE || entry.state == CacheState::MODIFIED) {
        // In Exclusive or Modified state, the data should be fetched from the owner node
        // For simplicity, we're fetching it from memory here
        uint64_t data = ram.read(address);
        entry.state = CacheState::SHARED;
        entry.cpuMask.set(requestingCpuID);
        return data;
    } else if (entry.state == CacheState::SHARED) {
        // If the block is already in Shared state, simply add the requesting CPU to the sharers
        entry.cpuMask.set(requestingCpuID);
        return ram.read(address);
    }

    // If the state is INVALID or not handled, return an indicative value
    return 0;  // Indicative of an error or invalid state
}


