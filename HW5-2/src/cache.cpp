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
        printCacheLineState(address); // Display the state of the cache line

}

uint64_t Cache::readFromCache(uint64_t address) {
    auto it = cacheData.find(address);
    // Check if cache line is in the cache
    if (it != cacheData.end() && it->second.state != CacheState::INVALID) {
        return it->second.data; // Cache hit
    } else {
        uint64_t data = 0;
        // Cache miss, consult the directory
        if (directory.isCachedElsewhere(address, this->id)) {
            // Logic to handle case when cache line is cached elsewhere
	data = directory.readData(address, this->id, ram);
            cacheData[address] = {CacheState::SHARED, address, data}; // Update cache
        } else {
            // Cache line is not in other caches, fetch from main memory (or simulate fetching)
            data = ram.read(address);
            cacheData[address] = {CacheState::EXCLUSIVE, address, data}; // Update cache
        }
     printCacheLineState(address); // Display the state of the cache line
     return data;
    }
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    directory.updateEntry(address, this->id, CacheState::MODIFIED);
    cacheData[address] = {CacheState::MODIFIED, address, data};
}

void Cache::printCacheLineState(uint64_t address) {
    auto it = cacheData.find(address);
    if (it != cacheData.end()) {
        std::cout << "Address: " << address << " State: ";
        switch (it->second.state) {
            case CacheState::MODIFIED: std::cout << "M"; break;
            case CacheState::EXCLUSIVE: std::cout << "E"; break;
            case CacheState::SHARED: std::cout << "S"; break;
            case CacheState::INVALID: std::cout << "I"; break;
            default: std::cout << "Unknown";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Address: " << address << " State: Not in Cache" << std::endl;
    }
}

