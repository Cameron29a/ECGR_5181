#include "BusArbiter.h"

void BusArbiter::requestAccess(int processorID) {
    requestQueue.push(processorID);
}

int BusArbiter::grantAccess() {
    if (!requestQueue.empty()) {
        int grantedProcessor = requestQueue.front();
        requestQueue.pop();
            return grantedProcessor;
    } else {
        // No pending requests, return an invalid value or handle accordingly
        return -1;
    }
}

void BusArbiter::removeEntry(int processorID) {
    std::queue<int> newQueue;
    
    while (!requestQueue.empty()) {
        if (requestQueue.front() != processorID) {
            newQueue.push(requestQueue.front());
        }
        requestQueue.pop();
    }
    requestQueue = newQueue;
}