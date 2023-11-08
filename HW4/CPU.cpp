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
    // PC is automatically incremented +4 when fetching an instruction.
    // In case if branch or jump, PC is adjusted in the execute stage during this cycle
}

inline void CPU::Decode() {
    if (decodeStage.empty()) {
        return;
    }
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    decodeStage.front().printAssembly();
    decodeStage.front().printInstruction();


}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands
    if (executeStage.empty()) {
        return;
    }
    
    uint32_t rs1 = ram.Read(executeStage.front().getrs1());
    uint32_t rs2 = ram.Read(executeStage.front().getrs2());

    bool takeBranch = false;
    if (executeStage.front().checkBranch() == 1) {
        takeBranch = branchCtl.shouldTakeBranch(executeStage.front().getfunct3(), rs1, rs2);
    }
    // set PCsel after branch check
    executeStage.front().setPCsel(executeStage.front().getOpcode(), takeBranch);

    // Check ALU input control signals
    if (executeStage.front().checkALUsrc1() == 1) {
        rs1 = pc;
    }
    if (executeStage.front().checkALUsrc2() == 1) {
        rs2 = executeStage.front().getImm();
    }
    
    // Have ALU execute the instruction
    executeStage.front().setALUresult(alu.doTheThing(executeStage.front().checkALUop(), rs1, rs2));

    // Adjust PC if there is a branch or jump
    switch (executeStage.front().checkPCsel()) {
        case true:
            pc = executeStage.front().getALUresult();
            break;
        default:
    }

}

inline void CPU::Memory() {
    // Perform memory store operation if required
    if (memoryStage.empty()) {
        return;
    }

    if (memoryStage.front().checkmemRead()) {
        // Memory read operation
        uint32_t memoryAddress = memoryStage.front().getALUresult();
        uint32_t loadedData = ram.Read(memoryAddress);
        uint32_t opcode = memoryStage.front().getOpcode();

        // Set the loaded data to the destination register
        switch (opcode) {
            case LOAD_FP:
                registers.fp_regs[memoryStage.front().getrd()] = loadedData;
                break;
            default: 
                registers.int_regs[memoryStage.front().getrd()] = loadedData;
        }

    } else if (memoryStage.front().checkmemWrite()) {
        // Memory write operation
        uint32_t memoryAddress = memoryStage.front().getALUresult();
        uint32_t dataToStore;
        switch (memoryStage.front().getOpcode()) {
            case S_TYPE_FP:
                dataToStore = registers.fp_regs[ram.Read(memoryStage.front().getrs2())];
                break;
            default: 
                dataToStore = registers.int_regs[ram.Read(memoryStage.front().getrs2())];
        }
        ram.Write(memoryAddress, dataToStore);
    }


}

inline void CPU::WriteBack() {
    // Perform memory write back
    if (writeBackStage.empty()) {
        return;
    }

    switch(writeBackStage.front().checkWBsel()){

    }
}

inline void CPU::updatePipeline() {
    // Check and move instructions from Fetch to Decode stage
    if (!fetchStage.empty()) {
        decodeStage.push(fetchStage.front());
        fetchStage.pop();
    }

    // Check and move instructions from Decode to Execute stage
    if (!decodeStage.empty()) {
        executeStage.push(decodeStage.front());
        decodeStage.pop();
    }

    // Check and move instructions from Execute to Memory stage
    if (!executeStage.empty()) {
        memoryStage.push(executeStage.front());
        executeStage.pop();
    }

    // Check and move instructions from Memory to WriteBack stage
    if (!memoryStage.empty()) {
        writeBackStage.push(memoryStage.front());
        memoryStage.pop();
    }

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
        // Update pipeline
        updatePipeline();

        // Run each stage every clock cycle
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

