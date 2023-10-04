#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>

// Enum to represent the instructions in the instruction queue
enum InstructionSet {
    NO_OP, ld, add, sd, addi, bne
};

// Class to represent an event in the pipeline
class Event {
public:
    int fetch, decode, execute, store, data, clock_cycle;

    // Constructor to initialize an event
    Event(int fe, int de, int ex, int st, int d, int c) : fetch(fe), decode(de), execute(ex), store(st), data(d), clock_cycle(c) {}
    
    // Function to print the details of an event
    void print() const {
        std::string instructionNames[] = { "NO_OP", "ld", "add", "sd", "addi", "bne"};
        std::cout << "Clock: " << clock_cycle << "\t" << instructionNames[fetch] << "\t" << instructionNames[decode] << "\t" << instructionNames[execute] << "\t" << instructionNames[store] << "\t" << "x1 => " << data << "\t|\t";
        
        if(fetch != NO_OP){
            std::cout << instructionNames[fetch] << "(F)." << clock_cycle << "\t";
        }
        if(decode != NO_OP){
            std::cout << instructionNames[decode] << "(D)." << clock_cycle << "\t";
        }
        if(execute != NO_OP){
            std::cout << instructionNames[execute] << "(E)." << clock_cycle << "\t";
        }
        if(store != NO_OP){
            std::cout << instructionNames[store] << "(S)." << clock_cycle << "\t";
        }
        std::cout << "\n";
    }
};

#endif //EVENT_H