#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <list>
#include <unordered_map>

#include "MemoryBus.h"

enum class CacheState { INVALID, SHARED, MODIFIED, EXCLUSIVE };

enum class BusSnoopState { READMISS, WRITEMISS, INVALIDATE };

struct CacheLine {
    CacheState state;
    uint64_t tag;  // Replace with actual tag size
    uint64_t data;
};

class Cache {
    int id;
    MemoryBus& memBus;
    std::unordered_map<uint64_t, CacheLine> cacheData;

    std::list<uint64_t> lruOrder;
    static const size_t maxCacheLines = 2;

    void updateLRUOrder(uint64_t address);

    bool waitingForMemoryAccess;


public:
    Cache(int id, MemoryBus& memBus) : id(id), memBus(memBus) {}

    bool isWaiting() { return waitingForMemoryAccess; }
    void setWaitFlag() { waitingForMemoryAccess = true; }
    void clearWaitFlag() { waitingForMemoryAccess = false; }

    int getID() { return id; }

    CacheState getCurrentState(uint64_t);
    void setCurrentState(uint64_t, CacheState);    

    uint64_t readFromCache(uint64_t);
    void writeToCache(uint64_t, uint64_t);

    /////////////////////////////
    void handleBusRequest(int, uint64_t);
    
    BusSnoopState updateState(bool, bool, uint64_t, bool);
    void updateSnoopingState(BusSnoopState, uint64_t);

};


#endif //CACHE_H