#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <unordered_map>
#include "cachestate.h"
#include "ram.h"
#include "network.h"  // Include network.h

class Directory; 
struct CacheLine {
    CacheState state;
    uint64_t tag;  // Replace with actual tag size
    uint64_t data;
};

class Cache {
    int id;
    Ram& ram;
    Directory& directory;
    NetworkNode* networkNode; 
    std::unordered_map<uint64_t, CacheLine> cacheData;
    bool waitingForMemoryAccess;

    bool checkOtherStates();
 static const size_t LocalMemoryBankSize = 256 * 1024; // 256 KB in bytes
    std::vector<uint8_t> localMemoryBank;  // Represents local memory bank

public:
 Cache(int id, Ram& memory, Directory& dir, NetworkNode* node) 
        : id(id), ram(memory), directory(dir), networkNode(node) {
        }
    bool isWaiting() { return waitingForMemoryAccess; }
    void setWaitFlag() { waitingForMemoryAccess = true; }
    void clearWaitFlag() { waitingForMemoryAccess = false; }

    void setCurrentState(uint64_t, CacheState);
    CacheState getCurrentState(uint64_t);
    
        // Modified methods
    uint64_t readFromCache(uint64_t address);
    void writeToCache(uint64_t address, uint64_t data);
void printCacheLineState(uint64_t address);

    void handleNetworkMessage(const Message& message);  // Method to handle network messages

};


#endif //CACHE_H
