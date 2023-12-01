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
    return 0;
}

void Cache::writeToCache(uint64_t address) {

}

void Cache::handleBusRequest(int processorID, CacheState busRequestState, uint64_t tag) {
    // Implement switch case for remotely initiated accesses
    CacheState currentState = getCurrentState(tag);

    switch (currentState) {
        case CacheState::INVALID:
            // Handle transitions for remotely initiated access in INVALID state
            // Update cacheData based on busRequestState
            break;

        case CacheState::SHARED:
            // Handle transitions for remotely initiated access in SHARED state
            // Update cacheData based on busRequestState
            break;

        case CacheState::MODIFIED:
            // Handle transitions for remotely initiated access in MODIFIED state
            // Update cacheData based on busRequestState
            break;

        case CacheState::EXCLUSIVE:
            // Handle transitions for remotely initiated access in EXCLUSIVE state
            // Update cacheData based on busRequestState
            break;

        default:
            // Handle other cases or throw an error
            break;
    }
}

void Cache::handleLocalAccess(uint64_t address, bool isWrite) {
    // Implement switch case for locally initiated accesses
    CacheState currentState = getCurrentState(address);

    switch(currentState) {
        case CacheState::INVALID:
            // Handle transitions for locally initiated access in INVALID state
            if (isWrite) {
                // Transition to MODIFIED or EXCLUSIVE based on your policy
                // Update cacheData and send write request to Memory Bus if needed
            } else {
                // Send read request to Memory Bus to check for the data
            }
            break;

        case CacheState::SHARED:
            // Handle transitions for locally initiated access in SHARED state
            if (isWrite) {
                // Transition to MODIFIED and invalidate other caches
                // Update cacheData and send write request to Memory Bus if needed
            }
            // Read operation doesn't require state transition in SHARED state
            break;

        case CacheState::MODIFIED:
            // Handle transitions for locally initiated access in MODIFIED state
            // No state change for read, just update cacheData
            // For write, update cacheData
            break;

        case CacheState::EXCLUSIVE:
            // Handle transitions for locally initiated access in EXCLUSIVE state
            // No state change for read, just update cacheData
            // For write, update cacheData
            break;

        default:
            // Handle other cases or throw an error
            break;
    }
}



