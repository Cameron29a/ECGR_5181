#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <string>
#include <random>

#include "CPU.cpp"
#include "ram.h"

class Simulation {
    std::queue<Event> systemEvents;  // Event queue for entire simulation

public:
    // Function to run the simulation
    void printEvents();
    void loadInstructionsToMemory(const std::string& filename, RAM &memory, uint32_t startAddress);
    void fillRandomData(RAM &memory, uint32_t startAddress, uint32_t endAddress);
    void runSimulation();
    
};

#endif //SIMULATION_H