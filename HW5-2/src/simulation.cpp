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
Network network;
std::vector<Directory> directories;
std::vector<Cache> caches;
std::vector<NetworkNode> networkNodes;

// Initialize directories
for (int i = 0; i < numCPU; ++i) {
    directories.emplace_back(numEntries, &network, mainMemory, i);
}

// Initialize caches
for (int i = 0; i < numCPU; ++i) {
    caches.emplace_back(i, mainMemory, directories[i], nullptr);
}

// Initialize network nodes
for (int i = 0; i < numCPU; ++i) {
    networkNodes.emplace_back(i, &caches[i], &directories[i]);
    caches[i].setNetworkNode(&networkNodes[i]);
    network.addNode(networkNodes[i]);
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
                caches[i].readFromCache(address, [i](uint64_t data) {
                            std::cout << "CPU" << i << " Received Data: " << data << std::endl;
            });
            } else {
                std::cout << "CPU" << i << " Writes to Address: " << address << "\n";
                caches[i].writeToCache(address, data);
            }
     // After each significant step, or at the end of each loop iteration
        // Print the state of the current address in all caches
        for (int j = 0; j < numCPU; ++j) {
            caches[j].printCacheLineState(address, j);
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
    mainMemory.printContents();
        
}

// every possible transation for both state machines, 
// interface with bus and processors

// print out cycle number, state of queue, processor each cycle


// output:
// Cycle #
// Access Request queue
