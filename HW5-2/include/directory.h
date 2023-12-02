#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <bitset>
#include "cachestate.h"
#include "ram.h"

struct DirectoryEntry {
    uint32_t tag;  // Tag to identify the cache line
    std::bitset<4> cpuMask;  // Bitmask for up to 4 CPUs
    CacheState state;  // Cache line state
    DirectoryEntry() : tag(0), cpuMask(0), state(CacheState::INVALID) {}
};


class Directory {
    size_t numEntries;
    std::vector<DirectoryEntry> entries;
public:
    Directory(size_t numEntries) : numEntries(numEntries), entries(numEntries) {}

    DirectoryEntry& getEntry(size_t index) { return entries.at(index); }
    const DirectoryEntry& getEntry(size_t index) const { return entries.at(index); }
    
    // Methods for updating and querying directory entries
    void updateEntry(uint64_t address, int cpuID, CacheState newState);
    bool isCachedElsewhere(uint64_t address, int cpuID);
    uint64_t readData(uint64_t address, int requestingCpuID, Ram& ram);


};


#endif // DIRECTORY_H

