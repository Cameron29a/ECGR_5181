#ifndef BUSARBITER_H
#define BUSARBITER_H

#include <queue>

class BusArbiter {
    std::queue<int> requestQueue;
    
public:
    BusArbiter() {}

    void requestAccess(int processorID);
    int grantAccess();

};

#endif //BUSARBITER_H