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
