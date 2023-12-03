#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <bitset>
#include "cachestate.h"
#include "ram.h"
#include "network.h"  

class Cache;  // Forward declaration of Cache

struct DirectoryEntry {
    uint32_t tag;  // Tag to identify the cache line
    std::bitset<4> cpuMask;  // Bitmask for up to 4 CPUs
    CacheState state;  // Cache line state
    DirectoryEntry() : tag(0), cpuMask(0), state(CacheState::INVALID) {}
};


class Directory {
    size_t numEntries;
    std::vector<DirectoryEntry> entries;
    Network* network;
    Ram& ram;
        
public:
     Directory(size_t numEntries, Network* net, Ram& ram) : 
        numEntries(numEntries), entries(numEntries), network(net), ram(ram) {}
    
    DirectoryEntry& getEntry(size_t index) { return entries.at(index); }
    const DirectoryEntry& getEntry(size_t index) const { return entries.at(index); }
    
    // Methods for updating and querying directory entries
    void updateEntry(uint64_t address, int cpuID, CacheState newState);
    bool isCachedElsewhere(uint64_t address, int cpuID);
    uint64_t readData(uint64_t address, int requestingCpuID, Ram& ram);

    void sendNetworkMessage(const Message& message);  // Method to send network messages
    void receiveMessage(const Message& message);
    
    
void printDirectoryEntryState(uint64_t address) const {
    size_t index = address % numEntries;
    const DirectoryEntry& entry = entries[index];

    std::cout << "Directory Entry State: Address=" << address 
              << ", State=" << toString(entry.state) 
              << ", CPU Mask=" << entry.cpuMask << std::endl;
              
}
              
    // Helper method to convert state enum to string
    std::string toString(CacheState state) const {
        switch (state) {
            case CacheState::INVALID: return "INVALID";
            case CacheState::SHARED: return "SHARED";
            case CacheState::MODIFIED: return "MODIFIED";
            case CacheState::EXCLUSIVE: return "EXCLUSIVE";
            default: return "UNKNOWN";
        }
    }

};


#endif // DIRECTORY_H

