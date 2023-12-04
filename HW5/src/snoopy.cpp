#include "snoopy.h"

bool Snoopy::snoopRead(int callingCache, uint64_t address) {
    // Iterate over all caches except the calling cache
    for (int i = 0; i <  static_cast<int>(allCaches.size()); i++) {
        if (i != callingCache) {
            CacheState state = allCaches[i].getCurrentState(address);
            // Process the state as needed
            switch (state) {
                case CacheState::SHARED:
                case CacheState::MODIFIED:
                case CacheState::EXCLUSIVE:
                    // Write line to the calling cache
                    allCaches[callingCache].writeToCache(address, allCaches[i].readFromCache(address));
                    std::cout << "Snoop Read Update.\n";
                    return true;
                default:
                    break;
            }
        }
    }
    return false;
}

void Snoopy::snoopWrite(int callingCache, uint64_t address) {
    // Iterate over all caches except the calling cache
    for (int i = 0; i <  static_cast<int>(allCaches.size()); i++) {
        if (i != callingCache) {
            CacheState state = allCaches[i].getCurrentState(address);
            // Process the state as needed
            switch (state) {
                case CacheState::MODIFIED:
                    // wite modified line to main memory
                    allCaches[i].handleBusRequest();
                    busArbiter.removeEntry(i);
                default:
                    break;
            }
        }
    }
}