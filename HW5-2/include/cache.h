#ifndef CACHE_H
#define CACHE_H

#include "cachestate.h"
#include "ram.h"
#include "network.h"
#include "message.h" 

class Directory; 
struct CacheLine {
    CacheState state;
    uint64_t tag;
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
        : id(id), ram(memory), directory(dir), networkNode(node) {}

    bool isWaiting() { return waitingForMemoryAccess; }
    void setWaitFlag() { waitingForMemoryAccess = true; }
    void clearWaitFlag() { waitingForMemoryAccess = false; }

    void setCurrentState(uint64_t, CacheState);
    CacheState getCurrentState(uint64_t);
    
        // Modified methods
    uint64_t readFromCache(uint64_t address);
    void writeToCache(uint64_t address, uint64_t data);
    void printCacheLineState(uint64_t address,int cpuID) const;
    
    void handleNetworkMessage(const Message& message);
     void setNetworkNode(NetworkNode* node) {
        networkNode = node;
    }


};


#endif //CACHE_H
