#include "simulation.h"

// Function to print the events
inline void Simulation::printEvents() {
    // std::cout << "Header";
    while (!systemEvents.empty()) {
        systemEvents.front().print();
        systemEvents.pop();
    }
}

inline void Simulation::loadInstructionsToMemory(const std::string& filename, RAM &ram, uint32_t startAddress, uint32_t stopAddress) {
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

            ram.Write(address, instruction);
            address += 4;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid binary format in the file.\n";
            break; // Exit the loop
        }
    }

    file.close();
}

inline void Simulation::fillRandomData(RAM &ram, uint32_t startAddress, uint32_t endAddress) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    for (uint32_t address = startAddress; address <= endAddress; ++address) {
        uint32_t randomValue = dist(gen);
        ram.Write(address, randomValue);
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
    RAM ram(0x13FF);

    // write instructions to addresses 0x0 – 0x093
    std::cout << "Write CPU 1's Instructions to Memory\n";
    std::string filename = "assembly_code/instructions.txt";
    uint32_t startAddress = 0x0;
    uint32_t stopAddress = 0x94;
    loadInstructionsToMemory(filename, ram, startAddress, stopAddress);

    std::cout << "Check Instructions after writing to memory\n";
    uint32_t i = startAddress;
    while (i <= stopAddress && ram.Read(i) != 0) {
        Instruction temp = ram.Read(i);
        std::cout << "Instruction #" << i/4 << ": " << temp.getAssemblyString() << "\n";
        i += 4;
    }

    // write instructions to addresses 0x100 – 0x193
    std::cout << "Write CPU 2's Instructions to Memory\n";
    std::string filename1 = "assembly_code/instructions2.txt";
    uint32_t startAddress1 = 0x100;
    uint32_t stopAddress1 = 0x194;
    loadInstructionsToMemory(filename1, ram, startAddress1, stopAddress1);

    std::cout << "Check Instructions after writing to memory\n";
    i = startAddress1;
    while (i <= stopAddress1 && ram.Read(i) != 0) {
        Instruction temp = ram.Read(i);
        std::cout << "Instruction #" << i/4 << ": " << temp.getAssemblyString() << "\n";
        i += 4;
    }

    // Allocate addresses 0x200 - 0x2FF for stack 0
    uint32_t stackAddress = 0x200;
    // uint32_t endStack = 0x2FF;

    // Allocate addresses 0x300 - 0x3FF for stack 1
    uint32_t stackAddress1 = 0x300;
    // uint32_t endStack = 0x3FF;

    // Initialize addresses 0x400 - 0xbFF (ARRAY_A & ARRAY_B) with random FP32 values.
    std::cout << "Fill ranges 0x400-0xBFF with random values\n";
    fillRandomData(ram, 0x400, 0xBFF);
    
    // std::cout << "=====Memory contents after Initilization=====\n";
    // ram.PrintMemoryContents();

    // Create Memory Bus with RAM
    std::cout << "========================Create MemBus=======================\n";
    MemBus memBus(ram);

    bool pipeline = false;
    pipeline = true;     // comment this out to run as single cycle processor
    std::cout << "=========================Create CPU=========================\n";
    CPU cpu1{ memBus, 1, startAddress, stackAddress, pipeline };
    CPU cpu2{ memBus, 2, startAddress1, stackAddress1, pipeline };

    
    // Main simulation loop.
    int loopCnt = 1;
    int loopMax = 1500;

    // for testing lab 2
    cpu1.writeIntRegister(1, 160);

    // Start CPU and start running until it resets
    std::cout << "======================Simulation Begin======================\n";
    while (cpu1.checkReset() == false && loopCnt <= loopMax) {
        std::cout << "\n=====================Simulation Loop #" << std::dec << loopCnt++ << "=====================\n";

        cpu1.runCPUcycle();
        // cpu2.runCPUcycle();

        memBus.processMemoryRequests();

        // Uncomment to print event queue every cycle
        cpu1.printCurrentEvent();
        // cpu2.printCurrentEvent();

        // Uncomment to print registers every cycle
        // cpu1.printRegisters();
        // cpu2.printRegisters();

        // Uncomment to print memory contents every cycle
        // ram.PrintMemoryContents();
    }
    std::cout << "\n=======================Simulation Ended=======================\n";
    if (loopCnt >= loopMax)
        std::cout << "=====Reason for Termination: Maximum loop counter reached=====\n"; 
        
    // std::cout << "=====Event Queue for Simulation=====\n";
    // cpu1.printEventQueue();
    // cpu2.printEventQueue();

    // std::cout << "=====Registers after end of Simulation=====\n";
    // cpu1.printRegisters();
    // cpu2.printRegisters();

    // std::cout << "=====Memory contents after end of Simulation=====\n";
    // ram.PrintMemoryContents();
    
    cpu1.printExecutedInstructions();

    std::cout.rdbuf(coutBuffer); // Restore the original cout buffer
    outputFile.close();
}

// need num of executed instructions
// num cycles
// cpi
// clock cycles
