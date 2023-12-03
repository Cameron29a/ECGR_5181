#ifndef SNOOPY_H
#define SNOOPY_H

#include "cache.h"

class Cache;  // Forward declaration

class Snoopy {
    std::vector<Cache> allCaches;

public:
    Snoopy(std::vector<Cache>& allCaches) : allCaches(allCaches) {}
    
    bool snoopRead(int, uint64_t);
    void snoopWrite(int, uint64_t);

};

#endif //SNOOPY_H
