#include "cache.h"

static const char* cacheStateToString(CacheState state) {
    switch (state) {
        case CacheState::INVALID:
            return "INVALID";
        case CacheState::SHARED:
            return "SHARED";
        case CacheState::MODIFIED:
            return "MODIFIED";
        case CacheState::EXCLUSIVE:
            return "EXCLUSIVE";
        default:
            return "UNKNOWN";
    }
}

void Cache::updateLRUOrder(uint64_t address) {
    // Remove the address from the list (if it exists)
    lruOrder.remove(address);
    // Add the address to the front of the list
    lruOrder.push_front(address);

    // If the list exceeds the max size, remove the least recently used element
    while (lruOrder.size() > maxCacheLines) {
        uint64_t lruAddress = lruOrder.back();
        lruOrder.pop_back();
        cacheData.erase(lruAddress);
    }
}

CacheState Cache::getCurrentState(uint64_t address) {
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        return it->second.state;
    } else {
        return CacheState::INVALID;
    }
}

void Cache::setCurrentState(uint64_t address, CacheState newState) {
    auto result = cacheData.emplace(address, CacheLine{});
    if (result.second) {
        result.first->second.state = newState;
        std::cout << "CPU" << id << " Changing to state : " << cacheStateToString(newState) << "\n";
        updateLRUOrder(address);
    } else {
        // Cache hit: update the state, and the LRU order is already handled in read/write functions
        result.first->second.state = newState;
        std::cout << "CPU" << id << " Changing to state: " << cacheStateToString(newState) << "\n";
    }
}

uint64_t Cache::readFromCache(uint64_t address) {
    updateLRUOrder(address);
    auto it = cacheData.find(address);
    
    if (it != cacheData.end()) {
        return it->second.data;
    }
    return -1;
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    updateLRUOrder(address);
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        it->second.data = data;
    } else {
        // If the cache line does not exist, create a new one
        CacheLine newCacheLine;
        newCacheLine.state = CacheState::INVALID;
        newCacheLine.tag = address;
        newCacheLine.data = data;

        // Insert the new cache line into the cacheData map
        cacheData[address] = newCacheLine;
    }
}

std::pair <BusSnoopState, uint64_t> Cache::handleBusRequest() {
    MemoryAccessRequest request = memBus.getRequestByCPUID(id);
    // if (request.cpuID != id)
        // return 0;
    bool isRead = request.isRead;
    uint64_t address = request.address;
    uint64_t data = request.data;

    if (isRead) {
        std::cout << "Main Memory Read by Cache" << id << "\n";
        memBus.readFromMemory(address);
    } 
    else {
        std::cout << "Main Memory Written to by Cache" << id << "\n";
        memBus.writeToMemory(address, data);
    }

    std::pair <BusSnoopState, uint64_t> output;
    output.first = updateState(isRead, 0, address, shareFlag);
    output.second = address;
    clearShareFlag();

    return output;
}

BusSnoopState Cache::updateState(bool isRead, bool isHit, uint64_t address, bool shared) {
    CacheState currentState = getCurrentState(address);
    CacheState nextState = CacheState::INVALID;
    BusSnoopState SnoopingUpdate;

    switch (currentState) {
        case CacheState::INVALID:
            if (!isHit) {
                if(!isRead) {
                    nextState = CacheState::MODIFIED;
                    SnoopingUpdate = BusSnoopState::WRITEMISS;
                } else {
                    SnoopingUpdate = BusSnoopState::READMISS;
                    if (shared) {
                        nextState = CacheState::SHARED;
                    } else {
                        nextState = CacheState::EXCLUSIVE;
                    }
                }
            }
            break;

        case CacheState::SHARED:
            if (isHit == 1) {
                if (isRead == 1) {
                    nextState = CacheState::SHARED;
                } else {
                    nextState = CacheState::MODIFIED;
                    SnoopingUpdate = BusSnoopState::INVALIDATE;
                }
            }
            break;

        case CacheState::MODIFIED:
            if (isHit == 1) {
                nextState = CacheState::MODIFIED;
            }
            break;

        case CacheState::EXCLUSIVE:
            if (isHit == 1) {
                if (isRead == 1) {
                    nextState = CacheState::EXCLUSIVE;
                } else {
                    nextState = CacheState::MODIFIED;
                }
            }
            break;

        default:
            break;
    }

    // Update the current state
    setCurrentState(address, nextState);
    return SnoopingUpdate;
}

void Cache::updateSnoopingState(BusSnoopState update, uint64_t address) {
    CacheState currentState = getCurrentState(address);
    CacheState nextState;

    switch (update) {
        case BusSnoopState::READMISS:
            switch(currentState) {
                case CacheState::SHARED:
                case CacheState::MODIFIED:
                case CacheState::EXCLUSIVE:
                    nextState = CacheState::SHARED;
                default:
                    break;
            }
            break;

        case BusSnoopState::WRITEMISS:
            switch(currentState) {
                case CacheState::MODIFIED:
                case CacheState::EXCLUSIVE:
                    nextState = CacheState::INVALID;
                default:
                    break;
            }
            break;
        
        case BusSnoopState::INVALIDATE:
            nextState = CacheState::INVALID;
            break;

        default:
            break;
    }
    // Update the state
    setCurrentState(address, nextState);
}