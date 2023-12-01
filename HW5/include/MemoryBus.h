#ifndef MEMORYBUS_H
#define MEMORYBUS_H

class MemoryBus {
public:
    void requestBusAccess(int processorID);
    void releaseBusAccess(int processorID);

};


#endif //MEMORYBUS_H