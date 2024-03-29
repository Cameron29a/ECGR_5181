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
    int numCPU;
    MemoryBus& memBus;
    std::unordered_map<uint64_t, CacheLine> cacheData;

    std::list<uint64_t> lruOrder;
    static const size_t maxCacheLines = 2;

    void updateLRUOrder(uint64_t address);

    bool waitingForMemoryAccess;
    bool shareFlag;


public:
    Cache(int id, int numCPU, MemoryBus& memBus) : id(id), numCPU(numCPU), memBus(memBus) {}

    bool isWaiting() { return waitingForMemoryAccess; }
    void setWaitFlag() { waitingForMemoryAccess = true; }
    void clearWaitFlag() { waitingForMemoryAccess = false; }

    void setShareFlag() { shareFlag = true; }
    void clearShareFlag() { shareFlag = false; }

    int getID() { return id; }

    CacheState getCurrentState(uint64_t);
    void setCurrentState(uint64_t, CacheState);    

    uint64_t readFromCache(uint64_t);
    void writeToCache(uint64_t, uint64_t);

    /////////////////////////////
    std::pair <BusSnoopState, uint64_t> handleBusRequest();
    
    BusSnoopState updateState(bool, bool, uint64_t, bool);
    void updateSnoopingState(BusSnoopState, uint64_t);

};


#endif //CACHE_H