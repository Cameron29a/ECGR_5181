#ifndef SNOOPY_H
#define SNOOPY_H

#include "cache.h"
#include "BusArbiter.h"

class Cache;  // Forward declaration

class Snoopy {
    std::vector<Cache>& allCaches;
    BusArbiter& busArbiter;

public:
    Snoopy(std::vector<Cache>& allCaches, BusArbiter& busArbiter) : allCaches(allCaches), busArbiter(busArbiter) {}
    
    bool snoopRead(int, uint64_t);
    void snoopWrite(int, uint64_t);

};

#endif //SNOOPY_H
