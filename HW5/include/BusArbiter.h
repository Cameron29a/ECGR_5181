#ifndef BUSARBITER_H
#define BUSARBITER_H

#include <iostream>
#include <queue>

class BusArbiter {
    std::queue<int> requestQueue;
    
public:
    BusArbiter() {}

    void requestAccess(int processorID);
    int grantAccess();

    std::queue<int> returnQueue() { return requestQueue; }

};

#endif //BUSARBITER_H