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
    srand(time(NULL));

    int numCPU = 4;
    const size_t numEntries = 4096; // Total number of cache lines

    std::cout << "========================Create Caches and Directory=======================\n";
    Directory directory(numEntries);
    std::vector<Cache> caches;
    for (int i = 0; i < numCPU; ++i) {
       caches.emplace_back(i, mainMemory, directory);  // Pass directory and mainMemory to each Cache
    }

    // Simulation loop
    int loopCnt = 1;
    int loopMax = 1500;

    std::cout << "======================Simulation Begin======================\n";
    while (loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << loopCnt++ << "=====================\n";

        // Simulate processor read and write requests
        for (int i = 0; i < numCPU; ++i) {
            // Randomly Select to Read or Write
            int isRead = rand() % 2;
            int addressMax = 1024;
            int dataMax = 1000;
            int address = rand() % addressMax;
            int data = rand() % dataMax;

            if (isRead) {
                std::cout << "CPU" << i << " Reads from Address: " << address << "\n";
                data = caches[i].readFromCache(address);
            }
            else {
                std::cout << "CPU" << i << " Writes to Address: " << address << "\n";
                caches[i].writeToCache(address, data);
            }

            // Additional logic can be added here to simulate network communication delays,
            // directory updates, and invalidation messages to other caches.
        }

        // Increment simulation time
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
