#include "simulation.h"

// Function to print the events
void Simulation::printEvents() {
    // std::cout << "Header";
    while (!systemEvents.empty()) {
        systemEvents.front().print();
        systemEvents.pop();
    }
}

void Simulation::runSimulation() {
    std::cout << "Begin System Initialization\n";

    std::cout << "========================Create MemBus=======================\n";
    MemoryBus memBus;
    
    std::cout << "======================Create BusArbiter=====================\n";
    BusArbiter busArbiter;

    std::cout << "========================Create Caches=======================\n";
    std::vector<Cache> caches;
    int numCPU = 4;
    for (int i = 0; i < numCPU; ++i) {
        caches.emplace_back(i, memBus);
    }

    // Simulation loop.
    int loopCnt = 1;
    int loopMax = 1500;

    // Start Simulation loop and run until reset
    std::cout << "======================Simulation Begin======================\n";
    while (loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << std::dec << loopCnt++ << "=====================\n";

        // Simulate processor read and write requests
        for (int i = 0; i < numCPU; ++i) {
            caches[i].read(address);
            caches[i].write(address);
        }

        // Simulate bus operations
        for (int i = 0; i < numCPU; ++i) {
            if (busArbiter.grantAccess() == i) {
                // Simulate bus request handling
                memBus.requestBusAccess(i);
                for (int j = 0; j < numCPU; ++j) {
                    if (j != i) {
                        caches[j].handleBusRequest(i, CacheState::SHARED, tag);
                    }
                }
                memBus.releaseBusAccess(i);
            }
        }

        currentTick += 10;
        systemEvents.push(Event(currentTick));
    }
    std::cout << "\n=======================Simulation Ended=======================\n";
    if (loopCnt >= loopMax)
        std::cout << "=====Reason for Termination: Maximum loop counter reached=====\n"; 
        
    std::cout << "=====Event Queue for Simulation=====\n";
    printEvents();

    
}

