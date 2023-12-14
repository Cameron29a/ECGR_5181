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
        caches.emplace_back(i, numCPU, memBus);
    }

    std::cout << "========================Create Snoopy=======================\n";
    Snoopy snoopy(caches, busArbiter);

    // Test Vectors
    std::vector<bool> readArray = {
        {}, 
        {}, 
        {}, 
        {}
    };
    std::vector<int> addressArray = {
        {}, 
        {}, 
        {}, 
        {}
    };
    std::vector<int> dataArray = {
        {}, 
        {}, 
        {}, 
        {}
    };

    // Simulation loop.
    int loopMax = 100;
    int loopCnt;

    // Start Simulation loop and run until reset
    std::cout << "======================Simulation Begin======================\n";
    for (loopCnt = 0; loopCnt <= loopMax; loopCnt++) {
        std::cout << "\n=====================Simulation Loop #" << loopCnt << "=====================\n";

        // Simulate processor read and write requests
        // update state after each request; syncronous update
        for (int i = 0; i < numCPU; ++i) {
            if (!caches[i].isWaiting()) {
                // Randomly Select to Read or Write
                // Implement other test vectors later
                bool isRead = dis(gen) == 1;
                // bool isRead = rand() % 2;
                int addressMax = 1024;
                int dataMax = 1000;
                int address = rand() % addressMax;
                int data = rand() % dataMax;
                // int address = addressArray[i][loopCnt];
                // int data = dataArray[i][1];
                
                CacheState state = caches[i].getCurrentState(address);

                bool isHit;
                bool shared = false;

                if (isRead) {
                    std::cout << "CPU" << i << " Reads from Address 0x" << std::hex << address << std::dec << "\n";
                    if(state == CacheState::INVALID) {
                        isHit = false;
                        std::cout << "Read Miss\n";
                        shared = snoopy.snoopRead(i, address);
                        if (!shared) {
                            caches[i].setWaitFlag();
                            memBus.addRequest(i, 0, address, 0);
                        } else caches[i].setShareFlag();
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
            std::cout << "Bus Arbiter Granted Access to CPU" << grantedProcessor << "\n";
            // Simulate bus request handling
            memBus.requestBusAccess(grantedProcessor);
            if (memBus.isBusinUse() && memBus.getCurrentProcessorID() ==  grantedProcessor) {
                std::cout << "Memory Bus Accessed\n";
                std::pair <BusSnoopState, uint64_t> updatebus = caches[grantedProcessor].handleBusRequest();
                caches[grantedProcessor].clearWaitFlag();
                for (int j = 0; j <  numCPU; j++) {
                    if (j != grantedProcessor) {
                        caches[j].updateSnoopingState(updatebus.first, updatebus.second);
                    }
                }
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
// 95