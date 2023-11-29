#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <string>
#include <random>

#include "CPU.h"

class Simulation {
    std::queue<Event> systemEvents;  // Event queue for entire simulation

public:
    ALU alu;
    // Function to run the simulation
    void printEvents();
    void loadInstructionsToMemory(const std::string&, RAM&, uint32_t, uint32_t);
    void fillRandomData(RAM&, uint32_t, uint32_t);
    void runSimulation();
    
};

#endif //SIMULATION_H