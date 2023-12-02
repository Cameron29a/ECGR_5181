#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <unordered_map>

#include "MemoryBus.h"

enum class CacheState { INVALID, SHARED, MODIFIED, EXCLUSIVE };

enum class BusSnoop { READMISS, WRITEMISS, INVALIDATE };

struct CacheLine {
    CacheState state;
    uint64_t tag;  // Replace with actual tag size
    uint64_t data;
};

class Cache {
    int id;
    MemoryBus& memBus;
    std::unordered_map<uint64_t, CacheLine> cacheData;
    bool waitingForMemoryAccess;

    bool checkOtherStates();
 static const size_t LocalMemoryBankSize = 256 * 1024;  // 256 KB in bytes
    std::vector<uint8_t> localMemoryBank;  // Represents local memory bank

public:
    Cache(int id, MemoryBus& memBus) : id(id), memBus(memBus) {        localMemoryBank.resize(LocalMemoryBankSize);
}

    bool isWaiting() { return waitingForMemoryAccess; }
    void setWaitFlag() { waitingForMemoryAccess = true; }
    void clearWaitFlag() { waitingForMemoryAccess = false; }

    void setCurrentState(uint64_t, CacheState);
    CacheState getCurrentState(uint64_t);

    uint64_t readFromCache(uint64_t);
    void writeToCache(uint64_t, uint64_t);
    
    void handleBusRequest(int, uint64_t);
    
    BusSnoop updateState(bool, bool, uint64_t);
    void updateSnoopingState(BusSnoop, uint64_t);

};


#endif //CACHE_H
