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
    // decodeStage.front().printAssembly();
    // decodeStage.front().printInstruction();


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
            return;
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

        // Set the loaded data to the destination register
        switch (memoryStage.front().checkFloat()) {
            case true:
                registers.fp_regs[memoryStage.front().getrd()] = loadedData;
                break;
            default:
                registers.int_regs[memoryStage.front().getrd()] = loadedData;
        }

    } else if (memoryStage.front().checkmemWrite()) {
        // Memory write operation
        uint32_t memoryAddress = memoryStage.front().getALUresult();
        uint32_t dataToStore;
        switch (memoryStage.front().checkFloat()) {
            case true:
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
        case 0:
            switch (writeBackStage.front().checkFloat()) {
                case true:
                    registers.fp_regs[writeBackStage.front().getrd()] = ram.Read(writeBackStage.front().getALUresult());
                    break;
                default: 
                    registers.int_regs[writeBackStage.front().getrd()] = ram.Read(writeBackStage.front().getALUresult());
            }
            break;
        case 1:
            switch (writeBackStage.front().checkFloat()) {
                case true:
                    registers.fp_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUresult();
                    break;
                default: 
                    registers.int_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUresult();
                }
        case 2:
            registers.int_regs[writeBackStage.front().getrd()] = prevPC+4;
            break;
        default:
            return;
    }
}

inline void CPU::updatePipeline() {
    // Check and move instructions from Memory to WriteBack stage
    if (!memoryStage.empty()) {
        writeBackStage.push(memoryStage.front());
        memoryStage.pop();
    }

    // Check and move instructions from Execute to Memory stage
    if (!executeStage.empty()) {
        memoryStage.push(executeStage.front());
        executeStage.pop();
    }

    // Check and move instructions from Decode to Execute stage
    if (!decodeStage.empty()) {
        executeStage.push(decodeStage.front());
        decodeStage.pop();
    }

    // Check and move instructions from Fetch to Decode stage
    if (!fetchStage.empty()) {
        decodeStage.push(fetchStage.front());
        fetchStage.pop();
    }

}

// Function to print the events
inline void CPU::printEvents() {
    std::cout << "\t\tFetch\tDecode\tExecute\tStore\t\n";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

inline void CPU::updateEventQueue() {
    std::string fetchString = fetchStage.front().printAssembly();
    std::string decodeString = decodeStage.front().printAssembly();
    std::string executeString = executeStage.front().printAssembly();
    std::string memoryString = memoryStage.front().printAssembly();
    std::string writeBackString = writeBackStage.front().printAssembly();

    events.push(Event(currentTick, fetchString, decodeString, executeString, memoryString, writeBackString));
}

inline void CPU::runCPU() {
    // Main simulation loop
    int loopCnt = 0;
    int loopMax = 20;
    while (reset == false) {
        if(loopCnt >= loopMax){
            reset = 1;
        }
        std::cout << "CPU Clk cycle #" << loopCnt << "******************************\n";
        loopCnt++;

        // Save pc at start of cycle
        prevPC = pc;

        // Update pipeline
        updatePipeline();

        // Run each stage every clock cycle
        Fetch();
        Decode();
        Execute();
        Memory();
        WriteBack();

        // Update event log for each cycle
        updateEventQueue();

        // Uncomment to print event queue every cycle
        // events.front().print();

        // Uncomment to print ram contents every cycle
        // memory.PrintMemoryContents();

    }
    // Uncomment to print event queue at the end of the program
    std::cout << "**********Completed CPU Event Queue**********\n";
    // printEvents();

}

