#ifndef SIMULATION_H
#define SIMULATION_H

#include <queue>
#include <cstdlib>
#include <random>

#include "event.h"
#include "cache.h"
#include "BusArbiter.h"
#include "snoopy.h"

class Simulation{
    // Event Log Data
    std::queue<Event> systemEvents;
    tick_t currentTick;
    std::queue<int> requestQueue;

public:
    Simulation() : currentTick(0) {}

    // Function to run the simulation
    void printEvents();
    void runSimulation();
    
};

#endif //SIMULATION_H