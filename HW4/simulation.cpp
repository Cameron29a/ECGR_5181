#include "simulation.h"

// Function to print the events
inline void Simulation::printEvents() {
    // std::cout << "Header";
    while (!systemEvents.empty()) {
        systemEvents.front().print();
        systemEvents.pop();
    }
}

inline void Simulation::loadInstructionsToMemory(const std::string& filename, RAM &memory, uint32_t startAddress) {
    std::ifstream file(filename);
    // if (!file.is_open()) {
    //     std::cerr << "Error: Unable to open the file: " << filename << "\n";
    //     return;
    // }

    std::string line;
    int address = startAddress;

    while (std::getline(file, line)) {
        try {
            // Convert the binary string to an integer (assuming the line contains binary machine code)
            uint32_t instruction = std::stoi(line, nullptr, 2);

            // Ensure that the instruction is within the valid range (32 bits)
            if (instruction < 0 || instruction > 0xFFFFFFFF) {
                std::cerr << "Error: Invalid binary instruction value in the file.\n";
                break; // Exit the loop
            }

            // Store the instruction in memory at the specified address
            uint32_t byte;
            for (int i = 0; i <= 3; i++) {
                byte = (instruction >> (i*8)) & 0xFF;
                memory.Write(address++, byte);
            }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid binary format in the file." << std::endl;
            break; // Exit the loop
        }
    }

    file.close();
}

inline void Simulation::fillRandomData(RAM &memory, uint32_t startAddress, uint32_t endAddress) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    for (uint32_t address = startAddress; address <= endAddress; ++address) {
        uint32_t randomValue = dist(gen);
        memory.Write(address, randomValue);
    }
}

inline void Simulation::runSimulation() {
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        return; // Return an error code
    }
    
    std::streambuf *coutBuffer = std::cout.rdbuf(); // Save the original cout buffer
    std::cout.rdbuf(outputFile.rdbuf()); // Redirect cout to the output file stream

    std::cout << "Begin System Initialization\n";

    std::cout << "Create Virtual Memory\n";
    RAM memory(0xF);

    // write instructions to addresses 0x0 – 0x093
    std::cout << "Write Instructions to Memory\n";
    std::string filename = "instructions.txt";
    uint32_t startAddress = 0x0;
    loadInstructionsToMemory(filename, memory, startAddress);
    
    // addresses 0x200 - 0x2FF will allocated for the stack

    // address ranges 0x400 - 0x7FF (ARRAY_A) and 0x800 - 0xBFF (ARRAY_B) 
    // will be initialized as arrays of random FP32 values.
    // std::cout << "Fill ranges 0x400-0xBFF with random values\n";
    // fillRandomData(memory, 0x400, 0xBFF);
    
    std::cout << "Memory contents before start of Simulation\n";
    memory.PrintMemoryContents();

    std::cout << "Create CPU\n";
    CPU cpu1{ memory };

    // Start CPU and start running instructions from memory
    // while (cpu1.getPC() < 0x94) {
        
        // cpu1.runCPU();
        cpu1.Fetch();
        cpu1.Decode();

        // Uncomment to print ram contents every cycle
        // memory.PrintMemoryContents();

    // }

    std::cout << "Memory contents after end of Simulation\n";
    // memory.PrintMemoryContents();
    
    std::cout.rdbuf(coutBuffer); // Restore the original cout buffer
    outputFile.close();
}