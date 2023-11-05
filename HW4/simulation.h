#ifndef SIMULATION_H
#define SIMULATION_H

#include "CPU.cpp"
#include "ram.h"

class Simulation {
    std::queue<Event> systemEvents;  // Event queue for entire simulation

public:
    // Function to run the simulation
    void printEvents();
    void runSimulation();
    
};

#endif //SIMULATION_H