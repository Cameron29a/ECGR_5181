#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    uint32_t nextInstruction = 0;
    std::cout << "***************Instruction Fetch***************\n";
    for(int i = 0; i <= 3; i++) {
        uint32_t byte = ram.Read(pc++);
        nextInstruction = nextInstruction | (byte << (i * 8));
        // Print out to check instructions are compiled correctly
        std::bitset<8> byteBinary(byte);
        std::cout << "Byte " << pc-1 << ": 0b" << byteBinary << "\n";
    }
    fetchStage.push(nextInstruction);

    // // Adjust PC if there is a branch or jump
    // if (.checkBranch() == 1) {
    //     pc = .getImm();
    // } else if (.checkJump() == 1){
    //     if (.checkRegtoPC() == 1){
    //         pc = .getImm + .getrs2();
    //     } else{
    //         pc += .getImm;
    //     }
    // }    
}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    decodeStage.push(fetchStage.front());
    fetchStage.pop();
    decodeStage.front().printAssembly();
    decodeStage.front().printInstruction();


}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands
    executeStage.push(decodeStage.front());
    decodeStage.pop();
}

inline void CPU::WriteBack() {
    // Perform memory store operation if required
    writeBackStage.push(executeStage.front());
    executeStage.pop();

    writeBackStage.pop();
}

// Function to print the events
inline void CPU::printEvents() {
    // std::cout << "Header";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

inline void CPU::runCPU() {
    // Main simulation loop
    while (reset == false) {
        Fetch();
        Decode();
        Execute();
        WriteBack();

        // Create event log for each cycle
        events.push(Event(currentTick));

        // Uncomment to print event queue every cycle
        // events.front().print();

        // Uncomment to print ram contents every cycle
        // memory.PrintMemoryContents();
    }
    // Uncomment to print event queue at the end of the program
    // std::cout << "**********Completed CPU Event Queue**********\n";
    // printEvents();
}

