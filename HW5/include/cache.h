#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <unordered_map>

#include "MemoryBus.h"

enum class CacheState { INVALID, SHARED, MODIFIED, EXCLUSIVE };

struct CacheLine {
    CacheState state;
    uint64_t tag;  // Replace with actual tag size
};

class Cache {
    int id;
    MemoryBus& memBus;
    std::unordered_map<uint64_t, CacheLine> cacheData;

public:
    Cache(int id, MemoryBus& memBus) : id(id), memBus(memBus) {}

    void read(uint64_t);
    void write(uint64_t);
    void handleBusRequest(int, CacheState, uint64_t);

};


#endif //CACHE_H