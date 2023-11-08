#include "CPU.h"

inline void CPU::Fetch() {
    if (!fetchStage.empty()) {
        return;
    }
    // Fetch an instruction from memory based on the current PC
    uint32_t nextInstruction = 0;
    std::cout << "***************Instruction Fetch***************\n";
    nextInstruction = ram.Read(pc);
    pc +=4;

    fetchStage.push(nextInstruction);
    // PC is automatically incremented +4 when fetching an instruction.
    // In case if branch or jump, PC is adjusted in the execute stage during this cycle
    std::cout << (fetchStage.front().getAssemblyString()) << "\n";
}

inline void CPU::Decode() {
    if (decodeStage.empty()) {
        return;
    }
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    // decodeStage.front().printAssembly();
    // decodeStage.front().printSignals();

}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands
    if (executeStage.empty()) {
        return;
    }
    
    uint32_t rs1 = ram.Read(executeStage.front().getrs1());
    uint32_t rs2 = ram.Read(executeStage.front().getrs2());

    bool takeBranch = false;
    if (executeStage.front().checkBranch()) {
        takeBranch = branchCtl.shouldTakeBranch(executeStage.front().getfunct3(), rs1, rs2);
    }
    // set PCsel after branch check
    executeStage.front().setPCsel(executeStage.front().getOpcode(), takeBranch);

    // Check ALU input control signals
    if (executeStage.front().checkALUsrc1()) {
        rs1 = pc;
    }
    if (executeStage.front().checkALUsrc2()) {
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
        std::cout << "ALU Result = " << memoryAddress << "\n";
        uint32_t loadedData = ram.Read(memoryAddress);

        // // Set the loaded data to the destination register
        // switch (memoryStage.front().checkFloat()) {
        //     case true:
        //         registers.fp_regs[memoryStage.front().getrd()] = loadedData;
        //         break;
        //     default:
        //         registers.int_regs[memoryStage.front().getrd()] = loadedData;
        // }

    } else if (memoryStage.front().checkmemWrite()) {
        // // Memory write operation
        // uint32_t memoryAddress = memoryStage.front().getALUresult();
        // uint32_t dataToStore;
        // switch (memoryStage.front().checkFloat()) {
        //     case true:
        //         dataToStore = registers.fp_regs[ram.Read(memoryStage.front().getrs2())];
        //         break;
        //     default: 
        //         dataToStore = registers.int_regs[ram.Read(memoryStage.front().getrs2())];
        // }
        // ram.Write(memoryAddress, dataToStore);
    }
}

inline void CPU::WriteBack() {
    // Perform memory write back
    if (writeBackStage.empty()) {
        return;
    }

    // switch(writeBackStage.front().checkWBsel()){
    //     case 0:
    //         switch (writeBackStage.front().checkFloat()) {
    //             case true:
    //                 registers.fp_regs[writeBackStage.front().getrd()] = ram.Read(writeBackStage.front().getALUresult());
    //                 break;
    //             default: 
    //                 registers.int_regs[writeBackStage.front().getrd()] = ram.Read(writeBackStage.front().getALUresult());
    //         }
    //         break;
    //     case 1:
    //         switch (writeBackStage.front().checkFloat()) {
    //             case true:
    //                 registers.fp_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUresult();
    //                 break;
    //             default: 
    //                 registers.int_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUresult();
    //             }
    //     case 2:
    //         registers.int_regs[writeBackStage.front().getrd()] = prevPC+4;
    //         break;
    //     default:
    // }
    
}

inline void CPU::updatePipeline() {
    // Check and move instructions from Fetch to Decode stage
    if (!fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty() && writeBackStage.empty()) {
        decodeStage.push(fetchStage.front());
        fetchStage.pop();
    }

    // Check and move instructions from Decode to Execute stage
    else if (!fetchStage.empty() && !decodeStage.empty() && executeStage.empty() && memoryStage.empty() && writeBackStage.empty()) {
        executeStage.push(decodeStage.front());
        decodeStage.pop();
    }

    // Check and move instructions from Execute to Memory stage
    else if (!fetchStage.empty() && decodeStage.empty() && !executeStage.empty() && memoryStage.empty() && writeBackStage.empty()) {
        memoryStage.push(executeStage.front());
        executeStage.pop();
    }

    // Check and move instructions from Memory to WriteBack stage
    else if (!fetchStage.empty() && decodeStage.empty() && executeStage.empty() && !memoryStage.empty() && writeBackStage.empty()) {
        writeBackStage.push(memoryStage.front());
        memoryStage.pop();
    }

    else if (!fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty() && !writeBackStage.empty()) {
        writeBackStage.pop();
    }

}

// Function to print the events
inline void CPU::printEvents() {
    // std::cout << "Header\n";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

inline void CPU::updateEventQueue() {
    std::string fetchString = fetchStage.front().getAssemblyString();
    std::string decodeString = decodeStage.front().getAssemblyString();
    std::string executeString = executeStage.front().getAssemblyString();
    std::string memoryString = memoryStage.front().getAssemblyString();
    std::string writeBackString = writeBackStage.front().getAssemblyString();

    events.push(Event(currentTick, fetchString, decodeString, executeString, memoryString, writeBackString));
}

inline void CPU::runCPU() {
    // Main simulation loop.
    int loopCnt = 0;
    int loopMax = 20;
    
    while (reset == false) {
        if(loopCnt >= loopMax){
            reset = 1;
            break;
        }
        std::cout << "********************************************CPU Clk cycle #" << std::dec << loopCnt << "********************************************\n";
        loopCnt++;

        // Save pc at start of cycle
        prevPC = pc;

        // Run each stage of the pipeline
        Fetch();
        Decode();
        Execute();
        // Memory();
        WriteBack();

        // Update event log for each cycle
        updateEventQueue();

        // Update pipeline
        updatePipeline();

        // Update System Clock
        currentTick++;

        // Uncomment to print event queue every cycle
        // events.front().print();

        // Uncomment to print ram contents every cycle
        // memory.PrintMemoryContents();

    }
    // Uncomment to print event queue at the end of the program
    std::cout << "**********Completed CPU Event Queue**********\n";
    printEvents();

}

