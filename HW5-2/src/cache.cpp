#include "cache.h"

CacheState Cache::getCurrentState(uint64_t address)
{
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        return it->second.state;
    } else {
        return CacheState::INVALID;
    }
}

void Cache::setCurrentState(uint64_t address, CacheState newState) {
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        it->second.state = newState;
    }
}

uint64_t Cache::readFromCache(uint64_t address) {
    auto it = cacheData.find(address);
    // Check if cache line is in the cache
    if (it != cacheData.end() && it->second.state != CacheState::INVALID) {
        return it->second.data; // Cache hit
    } else {
        // Cache miss, consult the directory
        if (directory.isCachedElsewhere(address, this->id)) {
            // Logic to handle case when cache line is cached elsewhere
	uint64_t data = directory.readData(address, this->id, ram);
            cacheData[address] = {CacheState::SHARED, address, data}; // Update cache
            return data;
        } else {
            // Cache line is not in other caches, fetch from main memory (or simulate fetching)
            uint64_t data = ram.read(address);
            cacheData[address] = {CacheState::EXCLUSIVE, address, data}; // Update cache
            return data;
        }
    }
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    directory.updateEntry(address, this->id, CacheState::MODIFIED);
    cacheData[address] = {CacheState::MODIFIED, address, data};
}
