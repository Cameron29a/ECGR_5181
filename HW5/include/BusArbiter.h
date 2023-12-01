#ifndef BUSARBITER_H
#define BUSARBITER_H

#include <vector>

class BusArbiter {
    std::vector<int> requestQueue;
public:
    BusArbiter() {}

    int grantAccess();

};

#endif //BUSARBITER_H