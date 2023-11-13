#include "simulation.h"

// Function to print the events
inline void Simulation::printEvents() {
    // std::cout << "Header";
    while (!systemEvents.empty()) {
        systemEvents.front().print();
        systemEvents.pop();
    }
}

inline void Simulation::loadInstructionsToMemory(const std::string& filename, RAM &memory, uint32_t startAddress, uint32_t stopAddress) {
    std::ifstream file(filename);
    // if (!file.is_open()) {
    //     std::cerr << "Error: Unable to open the file: " << filename << "\n";
    //     return;
    // }

    std::string line;
    uint32_t address = startAddress;

    while (std::getline(file, line) && address <= stopAddress) {
        if (line.size() != 32) {
            std::cerr << "Error: Invalid binary format in the file (not 32 bits).\n";
            break; // Exit the loop
        }

        try {
            uint32_t instruction = 0;
            // Convert the binary string directly into a 32-bit integer
            for (int i = 0; i < 32; i++) {
                instruction = (instruction << 1) | (line[i] - '0');
            }

            memory.Write(address, instruction);
            address += 4;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid binary format in the file.\n";
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
    RAM memory(0x77);

    // write instructions to addresses 0x0 – 0x093
    std::cout << "Write Instructions to Memory\n";
    std::string filename = "instructions.txt";
    uint32_t startAddress = 0x0;
    uint32_t stopAddress = 0x94;
    loadInstructionsToMemory(filename, memory, startAddress, stopAddress);

    std::cout << "Check Instructions after writing to memory\n";
    uint32_t i = startAddress;
    while (i <= stopAddress && memory.Read(i) != 0) {
        Instruction temp = memory.Read(i);
        std::cout << "Instruction #" << i/4 << ": " << temp.getAssemblyString() << "\n";
        i += 4;
    }
    std::cout << "First Empty Address: 0x" << std::hex << i << std::dec << "\n";

    // Allocate addresses 0x200 - 0x2FF for the stack
    uint32_t startStack = 0x200;
    // uint32_t endStack = 0x2FF;

    // Initialize addresses 0x400 - 0xbFF (ARRAY_A & ARRAY_B) with random FP32 values.
    // std::cout << "Fill ranges 0x400-0xBFF with random values\n";
    // fillRandomData(memory, 0x400, 0xBFF);
    
    // std::cout << "=====Memory contents before start of Simulation=====\n";
    // memory.PrintMemoryContents();

    bool pipeline = false;
    std::cout << "=========================Create CPU=========================\n";
    CPU cpu1{ memory, startAddress, startStack, pipeline };

    
    // Main simulation loop.
    int loopCnt = 0;
    int loopMax = 750;

    // for testing lab 2
    cpu1.writeIntRegister(1, 160);

    // Start CPU and start running until it resets
    std::cout << "======================Simulation Begin======================\n";
    while (cpu1.checkReset() == false && loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << std::dec << loopCnt++ << "=====================\n";

        cpu1.runCPUcycle();

        // Uncomment to print event queue every cycle
        cpu1.printCurrentEvent();

        // Uncomment to print registers every cycle
        // cpu1.printRegisters();

        // Uncomment to print ram contents every cycle
        // memory.PrintMemoryContents();
    }
    std::cout << "\n=======================Simulation Ended=======================\n";
    if (loopCnt >= loopMax)
        std::cout << "=====Reason for Termination: Maximum loop counter reached=====\n"; 
    // std::cout << "=====Event Queue for Simulation=====\n";
    // cpu1.printEventQueue();

    // std::cout << "=====Registers after end of Simulation=====\n";
    // cpu1.printRegisters();

    // std::cout << "=====Memory contents after end of Simulation=====\n";
    // memory.PrintMemoryContents();
    
    std::cout.rdbuf(coutBuffer); // Restore the original cout buffer
    outputFile.close();
}