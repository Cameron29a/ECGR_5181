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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    static const int numCPU = 4;

    std::cout << "======================Create BusArbiter=====================\n";
    BusArbiter busArbiter;

    std::cout << "========================Create MemBus=======================\n";
    MemoryBus memBus(busArbiter);

    std::cout << "========================Create Caches=======================\n";
    std::vector<Cache> caches;
    for (int i = 0; i < numCPU; ++i) {
        caches.emplace_back(i, memBus);
    }

    Snoopy snoopy(caches);

    // Simulation loop.
    int loopCnt = 1;
    int loopMax = 100;

    // Start Simulation loop and run until reset
    std::cout << "======================Simulation Begin======================\n";
    while (loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << std::dec << loopCnt++ << "=====================\n";

        // Simulate processor read and write requests
        // update state after each request; syncronous update
        for (int i = 0; i < numCPU; ++i) {
            if (!caches[i].isWaiting()) {
                // Randomly Select to Read or Write
                // Implement other test vectors later
                bool isRead = dis(gen) == 1;
                // bool isRead = rand() % 2;
                // int addressMax = 1024;
                // int dataMax = 1000;
                // int address = rand() % addressMax;
                // int data = rand() % dataMax;
                int address = 100 * (loopCnt % 2);
                int data = 200;
                
                CacheState state = caches[i].getCurrentState(address);

                bool isHit;
                bool shared = false;

                if (isRead == true) {
                    std::cout << "CPU" << i << " Reads from Address 0x" << std::hex << address << std::dec << "\n";
                    if(state == CacheState::INVALID) {
                        isHit = false;
                        std::cout << "Read Miss\n";
                        shared = snoopy.snoopRead(i, address);
                        if (!shared) {
                            caches[i].setWaitFlag();
                            memBus.addRequest(i, 0, address, 0);
                        }
                    } else {
                        isHit = true;
                        std::cout << "Read Hit\n";
                        data = caches[i].readFromCache(address);
                        std::cout << "Address 0x" << std::hex << address << std::dec << " : Data - " << data << "\n";
                    }
                }
                else {
                    std::cout << "CPU" << i << " Write to Address 0x" << std::hex << address << std::dec << "\n";
                    if(state == CacheState::INVALID) {
                        isHit = false;
                        std::cout << "Write Miss\n";
                        snoopy.snoopWrite(i, address);
                        caches[i].setWaitFlag();
                        memBus.addRequest(i, 1, address, data);
                    } else {
                        isHit = true;
                        std::cout << "Write Hit\n";
                        caches[i].writeToCache(address, data);
                        std::cout << "Data: " << data << " written to Address 0x" << std::hex << address << std::dec << "\n";
                    }
                }

                BusSnoopState remoteUpdate = caches[i].updateState(isRead, isHit, address, shared);
                for (int j = 0; j <  numCPU; j++) {
                    if (j != i) {
                        caches[j].updateSnoopingState(remoteUpdate, address);
                    }
                }
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

        // Update Event Queue
        currentTick += 10;
        requestQueue = busArbiter.returnQueue();
        systemEvents.push(Event(currentTick, requestQueue));
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