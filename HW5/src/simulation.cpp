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
    srand (time(NULL));

    int numCPU = 4;

    std::cout << "========================Create MemBus=======================\n";
    MemoryBus memBus;
    
    std::cout << "======================Create BusArbiter=====================\n";
    BusArbiter busArbiter;

    std::cout << "========================Create Caches=======================\n";
    std::vector<Cache> caches;
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
        // update state after each request; syncronous update
        for (int i = 0; i < numCPU; ++i) {
            if (i != caches[i].isWaiting()) {
                caches[i].setWaitFlag();

                // Randomly Select to Read or Write
                // Implement other test vectors later
                int isRead = rand() % 2;
                int addressMax = 1024;
                int dataMax = 1000;
                int address = rand() % addressMax;
                int data = rand() % dataMax;
                
                bool isHit;

                if (isRead == true) {
                    std::cout << "CPU" << i << " Reads from Address:" << 0 << "\n";
                    data = caches[i].readFromCache(address);
                    if(data == -1) {
                        isHit = false;
                        caches[i].clearWaitFlag();
                    } else {
                        busArbiter.requestAccess(i);
                        memBus.addRequest(i, 0, address, 0);
                    }
                }
                else {
                    std::cout << "CPU" << i << " Write to Address:" << 0 << "\n";
                    caches[i].writeToCache(address, data);
                    if(data == -1) {
                        isHit = false;
                        caches[i].clearWaitFlag();
                    } else {
                        busArbiter.requestAccess(i);
                        memBus.addRequest(i, 1, address, data);
                    }
                }
                caches[i].updateState(isRead, isHit, address);
            }
        }

        // Simulate bus operations
        int grantedProcessor = busArbiter.grantAccess();
        if (grantedProcessor != -1) {
            // Simulate bus request handling
            memBus.requestBusAccess(grantedProcessor);
            if (memBus.isBusinUse() && memBus.getCurrentProcessorID() ==  grantedProcessor) {
                // caches[grantedProcessor].handleBusRequest(grantedProcessor, address);

                // caches[i].updateState(isRead, isHit, address);
                caches[grantedProcessor].clearWaitFlag();
            }
            memBus.releaseBusAccess(grantedProcessor);
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

// every possible transation for both state machines, 
// interface with bus and processors

// print out cycle number, state of queue, processor each cycle


// output:
// Cycle #
// Access Request queue