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
Directory directory(numEntries, &network, mainMemory);
std::vector<NetworkNode> networkNodes;

// Create NetworkNode instances
for (int i = 0; i < numCPU; ++i) {
    networkNodes.emplace_back(i, nullptr); // Initially, Cache pointers are null
    network.addNode(networkNodes.back());
}

// Now create Cache instances
std::vector<Cache> caches;
for (int i = 0; i < numCPU; ++i) {
    caches.emplace_back(i, mainMemory, directory, &networkNodes[i]);
    networkNodes[i].cache = &caches.back(); // Link back NetworkNode to Cache
}

    // Simulation loop
    int loopCnt = 1;
    int loopMax = 1500;

    std::cout << "======================Simulation Begin======================\n";
    while (loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << loopCnt++ << "=====================\n";
     uint64_t testAddress = 100; // Example address for testing
    // Manually triggering state transitions for a cache block and directory
    for (int cpu = 0; cpu < numCPU; ++cpu) {
        std::cout << "\n====== CPU " << cpu << " ======\n";

        // INVALID -> SHARED (Read Miss)
        std::cout << "Transition: INVALID -> SHARED (Read Miss) at Address " << testAddress << "\n";
        caches[cpu].readFromCache(testAddress);

        // SHARED -> MODIFIED (Write Miss)
        std::cout << "Transition: SHARED -> MODIFIED (Write Miss) at Address " << testAddress << "\n";
        caches[cpu].writeToCache(testAddress, 123); // Example data

        // MODIFIED -> INVALID (Data Write-Back)
        std::cout << "Transition: MODIFIED -> INVALID (Data Write-Back) at Address " << testAddress << "\n";
        // Implement logic for Data Write-Back
        // ...

        // INVALID -> MODIFIED (Write Miss)
        std::cout << "Transition: INVALID -> MODIFIED (Write Miss) at Address " << testAddress << "\n";
        caches[cpu].writeToCache(testAddress, 456); // Example data

        // Print the cache line state after each operation
        for (Cache& cache : caches) {
            cache.printCacheLineState(testAddress);
        }
        
            directory.printDirectoryEntryState(testAddress); // Assuming such a method exists


        // Increment simulation time and record the event
        currentTick += 10;
        systemEvents.push(Event(currentTick));
    }
}

    std::cout << "\n=======================Simulation Ended=======================\n";
 //   std::cout << "=====Event Queue for Simulation=====\n";
 //   printEvents();
}

// every possible transation for both state machines, 
// interface with bus and processors

// print out cycle number, state of queue, processor each cycle


// output:
// Cycle #
// Access Request queue
