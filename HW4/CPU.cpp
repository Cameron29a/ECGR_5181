#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    if (fetchStage.empty()) { return; }
    std::cout << "Fetch Stage\n";
    
    // fetchStage.front().printInstruction();
    instructionCnt++;
}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    if (decodeStage.empty()) { return; }
    std::cout << "Decode Stage\n";

    // decodeStage.front().printAssembly();
    // decodeStage.front().printSignals();
}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands
    if (executeStage.empty())  { return; }
    std::cout << "Execute Stage\n";

    int32_t rs1 = registers.int_regs[executeStage.front().getrs1()];
    int32_t rs2 = registers.int_regs[executeStage.front().getrs2()];

    float rs1_fp = registers.fp_regs[executeStage.front().getrs1()];
    float rs2_fp = registers.fp_regs[executeStage.front().getrs2()];

    bool takeBranch = false;
    if (executeStage.front().checkBranch()) {
        takeBranch = branchCtl.shouldTakeBranch(executeStage.front().getfunct3(), rs1, rs2);
        std::cout << "Branch check: ";
            if (takeBranch) std::cout << "True\n";
            else std::cout << "False\n";
    }
    // set PCsel after branch check
    executeStage.front().setPCsel(takeBranch);

    // Check ALU input control signals
    if (executeStage.front().checkALUsrc1()) {
        // rs1 = pc;  // signed <-unsigned
        rs1 = static_cast<int32_t>(pc);
        std::cout << "ALU source 1 switched \n";
    }
    if (executeStage.front().checkALUsrc2()) {
        rs2 = executeStage.front().getImm();
        std::cout << "ALU source 2 switched \n";
    }
    
    // // Have ALU execute the instruction
    executeStage.front().setALUresult(alu.doTheThing(executeStage.front().checkALUop(), rs1, rs2, rs1_fp, rs2_fp));

    // Adjust PC if there is a branch or jump
    if (executeStage.front().checkPCsel()) {
        pc = executeStage.front().getALUresult();
        std::cout << "Next Instruction: PC = ALU result\n";
    } else {
        pc +=4;
        std::cout << "Next Instruction: PC += 4\n";
    }
    std::cout << "data in rs1: " << rs1 << "\n";
    std::cout << "data in rs2: " << rs2 << "\n";
    std::cout << "data in rs1_fp: " << rs1_fp << "\n";
    std::cout << "data in rs2_fp: " << rs2_fp << "\n";
    std::cout << "ALU result: " << executeStage.front().getALUresult() << "\n";
}

inline void CPU::Memory() {
    // Perform memory store operation if required
    if (memoryStage.empty())  { return; }
    std::cout << "Memory Stage\n";

    if (memoryStage.front().checkmemRead()) {
        // Memory read operation
        uint32_t memoryAddress = memoryStage.front().getALUresult();
        // std::cout << "ALU Result = " << memoryAddress << "\n";
        int32_t loadedData = ram.Read(memoryAddress);

        // Set the loaded data to the destination register
        if (memoryStage.front().checkFloat()) {
            registers.fp_regs[memoryStage.front().getrd()] = loadedData;
        } else {
            registers.int_regs[memoryStage.front().getrd()] = loadedData;
        }

    } 
    else if (memoryStage.front().checkmemWrite()) {
        // Memory write operation
        uint32_t memoryAddress = memoryStage.front().getALUresult();
        uint32_t dataToStore;
        if (memoryStage.front().checkFloat()) {
            dataToStore = registers.fp_regs[memoryStage.front().getrs2()];
        } else {
            dataToStore = registers.int_regs[memoryStage.front().getrs2()];
        }
        ram.Write(memoryAddress, dataToStore);
    }
}

inline void CPU::WriteBack() {
    // Perform memory write back
    if (writeBackStage.empty()) { return; }
    std::cout << "WriteBack Stage\n";

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
    // }
}

inline void CPU::updateDataPath() {
    // Update Fetch stage
    if (fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty()) {
        uint32_t nextInstruction = ram.Read(pc);
        if (nextInstruction != 0)       fetchStage.push(nextInstruction);
        if (!writeBackStage.empty())    writeBackStage.pop();
    }

    // Check and move instructions from Fetch to Decode stage
    else if (!fetchStage.empty()) {
        decodeStage.push(fetchStage.front());
        fetchStage.pop();
    }

    // Check and move instructions from Decode to Execute stage
    else if (!decodeStage.empty()) {
        executeStage.push(decodeStage.front());
        decodeStage.pop();
    }

    // Check and move instructions from Execute to Memory stage
    else if (!executeStage.empty()) {
        memoryStage.push(executeStage.front());
        executeStage.pop();
    }

    // Check and move instructions from Memory to WriteBack stage
    else if (!memoryStage.empty()) {
        writeBackStage.push(memoryStage.front());
        memoryStage.pop();
    }
}

inline void CPU::updatePipeLine() {

}

inline void CPU::printRegisters() {
    std::cout << "Integer Registers:\n";
    for (int i = 0; i < 32; ++i) {
        std::cout << "x" << i << " (" << std::bitset<5>(i) << "): " << std::bitset<32>(registers.int_regs[i]) << "\n";
    }

    std::cout << "Floating-Point Registers:\n";
    for (int i = 0; i < 32; ++i) {
        std::cout << "f" << i << " (" << std::bitset<5>(i) << "): " << std::bitset<32>(*(reinterpret_cast<uint32_t*>(&registers.fp_regs[i])) ) << "\n";
    }
}

// Function to print the event queue
inline void CPU::printEventQueue() {
    std::cout << "**************Event Queue***************\n";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

// Function to print the current event
inline void CPU::printCurrentEvent() {
    std::cout << "*************Current Event**************\n";
    events.back().print();
}

inline void CPU::updateEventQueue() {
    std::string fetchString, decodeString, executeString, memoryString, writeBackString;

    if (fetchStage.empty()) fetchString = "No_Op";
    else fetchString = fetchStage.front().getAssemblyString();
    
    if (decodeStage.empty()) decodeString = "No_Op";
    else decodeString = decodeStage.front().getAssemblyString();
    
    if (executeStage.empty()) executeString = "No_Op";
    else executeString = executeStage.front().getAssemblyString();
    
    if (memoryStage.empty()) memoryString = "No_Op";
    else memoryString = memoryStage.front().getAssemblyString();
    
    if (writeBackStage.empty()) writeBackString = "No_Op";
    else writeBackString = writeBackStage.front().getAssemblyString();

    
    events.push(Event(currentTick, pc, instructionCnt, fetchString, decodeString, executeString, memoryString, writeBackString));
}

inline void CPU::runCPUcycle() {
    if (reset == false) {
        // Save pc at start of cycle
        prevPC = pc;

        // Update datapath
        switch (pipeline) {
            case true:
                updatePipeLine();
            case false:
                updateDataPath();
        }

        // Run each stage of the pipeline
        Fetch();
        Decode();
        Execute();
        Memory();
        WriteBack();

        // Update event log for each cycle
        updateEventQueue();

        // Update System Clock
        currentTick += 10;

        if (fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty() && writeBackStage.empty()) {
            reset = true;
            std::cout << "All Stages of the data path are empty. Resetting CPU.\n";
        }

        // debug statements
        std::cout << "Cpu Cycle Debug Statements\n";
        std::cout << "X1 value: " << std::dec << readIntRegister(1) << "\n";
    }
}

