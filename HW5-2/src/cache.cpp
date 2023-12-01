#include "cache.h"

bool Cache::checkOtherStates() {
    
    return false;
}

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
    CacheState state = getCurrentState(address);

    // If the cache line is upto date, return the data from the cache
    if (state != CacheState::INVALID) {
        return cacheData[address].data; 
    } else {
        return -1;
    }
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    // CacheState state = getCurrentState(address);


}

void Cache::handleBusRequest(int processorID, uint64_t address) {
    CacheState currentState = getCurrentState(address);

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

BusSnoop Cache::updateState(bool isRead, bool isHit, uint64_t address) {
    CacheState currentState = getCurrentState(address);
    BusSnoop SnoopingUpdate;

    switch (currentState) {
        case CacheState::INVALID:
            if (!isHit) {
                if(!isRead) {
                    currentState = CacheState::MODIFIED;
                    SnoopingUpdate = BusSnoop::WRITEMISS;
                } else {
                    SnoopingUpdate = BusSnoop::READMISS;
                    // if () {
                    currentState = CacheState::SHARED;
                    // }
                }
            }
            break;

        case CacheState::SHARED:
            if (isHit == 1) {
                if (isRead == 1) {
                    currentState = CacheState::SHARED;
                } else {
                    currentState = CacheState::MODIFIED;
                    SnoopingUpdate = BusSnoop::INVALIDATE;
                }
            }
            break;

        case CacheState::MODIFIED:
            if (isHit == 1) {
                if (isRead == 1) {
                    currentState = CacheState::MODIFIED;
                } else {
                    currentState = CacheState::MODIFIED;
                }
            }
            break;

        case CacheState::EXCLUSIVE:
            if (isHit == 1) {
                if (isRead == 1) {
                    currentState = CacheState::EXCLUSIVE;
                } else {
                    currentState = CacheState::MODIFIED;
                }
            }
            break;

        default:
            break;
    }

    // Update the current state


    return SnoopingUpdate;
}

void Cache::updateSnoopingState(BusSnoop update, uint64_t address) {
    CacheState currentState = getCurrentState(address);
    switch (update) {
        case BusSnoop::READMISS:
            switch(currentState) {
                case CacheState::SHARED:
                case CacheState::MODIFIED:
                case CacheState::EXCLUSIVE:
                default:
                    break;
            }
            break;

        case BusSnoop::WRITEMISS:

            break;
        
        case BusSnoop::INVALIDATE:

            break;

        default:
            break;
    }
}