#ifndef SIMULATION_H
#define SIMULATION_H

#include <queue>
#include <cstdlib>

#include "event.h"
#include "cache.h"
#include "BusArbiter.h"

class Simulation {
    std::queue<Event> systemEvents;
    tick_t currentTick;

public:
    // Function to run the simulation
    void printEvents();
    void runSimulation();
    
};

#endif //SIMULATION_H