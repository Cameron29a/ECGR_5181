#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <string>

#include "CPU.cpp"
#include "ram.h"

class Simulation {
    std::queue<Event> systemEvents;  // Event queue for entire simulation

public:
    // Function to run the simulation
    void printEvents();
    void loadInstructionsToMemory(const std::string& filename, RAM &memory, uint32_t startAddress);
    void runSimulation();
    
};

#endif //SIMULATION_H