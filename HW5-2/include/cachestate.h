#ifndef CACHE_STATE_H
#define CACHE_STATE_H

// Enum for cache line states
enum class CacheState {
    INVALID,  // Cache line is not valid
    SHARED,   // Cache line is in shared state
    MODIFIED, // Cache line is modified
    EXCLUSIVE // Cache line is in exclusive state
};

#endif // CACHE_STATE_H
