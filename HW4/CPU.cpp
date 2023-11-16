#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    if (fetchStage.empty()) { return; }

    std::cout << "//////////////Fetch Stage///////////////\n";

    // Flow control places an empty instruction in the queue, so this stage will activate
    // If there is a stall, the pc will not update twice on the same instruction
    if (fetchStage.front().getInstruction() == 0) {
        fetchStage.pop();
        fetchStage.push(ram.Read(pc));
        pc +=4;
        std::cout << "Next Instruction: PC += 4\n";
    }
    
    fetchStage.front().printInstruction();
    fetchStage.front().printAssembly();
}

inline void CPU::Decode() {
    // Decode the fetched instruction and extract opcode, registers, and immediate values
    if (decodeStage.empty()) { return; }
    std::cout << "//////////////Decode Stage//////////////\n";

    // Set Control Signals
    decodeStage.front().setregWrite();
    decodeStage.front().setALUsrc1();
    decodeStage.front().setALUsrc2();
    decodeStage.front().setmemWrite();
    decodeStage.front().setmemRead();
    decodeStage.front().setBranch();
    decodeStage.front().setRM();
    decodeStage.front().setALUop();
    decodeStage.front().setWBsel();
    decodeStage.front().setIsFloat();
    decodeStage.front().setPCsel(true); // assume branch true to handle hazards, update in execute

    // decodeStage.front().printAssembly();
    // decodeStage.front().printSignals();
}

inline void CPU::Execute() {
    // Execute the instruction based on its opcode and operands
    if (executeStage.empty())  { return; }
    std::cout << "/////////////Execute Stage/////////////\n";

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
        rs1 = static_cast<int32_t>(pc);
        std::cout << "ALU source 1 switched \n";
    }
    if (executeStage.front().checkALUsrc2()) {
        rs2 = executeStage.front().getImm();
        std::cout << "ALU source 2 switched \n";
    }

    std::cout << "ALU op: " << executeStage.front().checkALUop() << "\n";
    
    // Have ALU execute the instruction
    auto result = alu.doTheThing(executeStage.front().checkALUop(), rs1, rs2, rs1_fp, rs2_fp);
    if (result.index() == 0) {
        int32_t intResult = std::get<int32_t>(result);
        executeStage.front().setALUresult(intResult);
        std::cout << "ALU Result: " << executeStage.front().getALUresult() << "\n";
    } else if (result.index() == 1) {
        float floatResult = std::get<float>(result);
        executeStage.front().setALUfloatResult(floatResult);
        std::cout << "ALU FP Result: " << executeStage.front().getALUfloatResult() << "\n";
    }
    
    std::cout << "Data in rs1: " << rs1 << "\n";
    std::cout << "Data in rs2: " << rs2 << "\n";
    std::cout << "Data in rs1_fp: " << rs1_fp << "\n";
    std::cout << "Data in rs2_fp: " << rs2_fp << "\n";
    std::cout << "ALU result: " << executeStage.front().getALUresult() << "\n";
}

inline void CPU::Memory() {
    // Perform memory store operation if required
    if (memoryStage.empty())  { return; }
    std::cout << "//////////////Memory Stage//////////////\n";

    if (memoryStage.front().checkmemRead()) {
        // Memory read operation
        // Set the loaded data to the destination register
        if (memoryStage.front().checkFloat()) {
            float memoryAddressFP = memoryStage.front().getALUfloatResult();
            float loadedDataFP = ram.ReadFloat(memoryAddressFP);
            registers.fp_regs[memoryStage.front().getrd()] = loadedDataFP;
            std::cout << "FP Memory Read Operation\n";
            std::cout << "Memory Address: " << memoryAddressFP << "\n";
            std::cout << "Loaded Data: " << loadedDataFP << "\n";
        } else {
            uint32_t memoryAddress = memoryStage.front().getALUresult();
            int32_t loadedData = ram.Read(memoryAddress);
            registers.int_regs[memoryStage.front().getrd()] = loadedData;
            std::cout << "Int Memory Read Operation\n";
            std::cout << "Memory Address: " << memoryAddress << "\n";
            std::cout << "Loaded Data: " << loadedData << "\n";
        }
    } 
    else if (memoryStage.front().checkmemWrite()) {
        // Memory write operation
        // Write Data to the memory location
        if (memoryStage.front().checkFloat()) {
            float memoryAddressFP = stackAddress + memoryStage.front().getALUresult();
            float dataToStoreFP;
            dataToStoreFP = registers.fp_regs[memoryStage.front().getrs2()];
            ram.WriteFloat(memoryAddressFP, dataToStoreFP);
            std::cout << "FP Memory Write Operation\n";
            std::cout << "Memory Address: " << memoryAddressFP << "\n";
            std::cout << "Data To Store: " << dataToStoreFP << "\n";
        } else {
            uint32_t memoryAddress = stackAddress + memoryStage.front().getALUresult();
            uint32_t dataToStore;
            dataToStore = registers.int_regs[memoryStage.front().getrs2()];
            ram.Write(memoryAddress, dataToStore);
            std::cout << "Int Memory Write Operation\n";
            std::cout << "Memory Address: " << memoryAddress << "\n";
            std::cout << "Data To Store: " << dataToStore << "\n";
        }
    }

    // Adjust PC if there is a branch or jump
    if (memoryStage.front().checkPCsel()) {
        pc = memoryStage.front().getALUresult();
        std::cout << "Next Instruction: PC = ALU result\n";
    } else {
        
    }
}

inline void CPU::WriteBack() {
    // Perform memory write back
    if (writeBackStage.empty()) { return; }
    std::cout << "/////////////WriteBack Stage////////////\n";

    switch(writeBackStage.front().checkWBsel()){
        case 0:
            if (writeBackStage.front().checkFloat()) {
                registers.fp_regs[writeBackStage.front().getrd()] = ram.ReadFloat(writeBackStage.front().getALUfloatResult());
                std::cout << "FP Write Back Case 0\n";
            } else {
                registers.int_regs[writeBackStage.front().getrd()] = ram.Read(writeBackStage.front().getALUresult());
                std::cout << "Int Write Back Case 0\n";
            }
            break;
        case 1:
            if (writeBackStage.front().checkFloat()) {
                registers.fp_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUfloatResult();
                std::cout << "FP Write Back Case 1\n";
            } else {
                registers.int_regs[writeBackStage.front().getrd()] = writeBackStage.front().getALUresult();
                std::cout << "Int Write Back Case 1\n";
            }
            break;
        case 2:
            // registers.int_regs[writeBackStage.front().getrd()] = prevPC+4;
            std::cout << "Int Write Back Case 2\n";
            break;
    }
}

inline void CPU::updateDataPath() {
    // Update Fetch stage
    if (fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty()) {
        if (ram.Read(pc) != 0) fetchStage.push(0);
        if (!writeBackStage.empty()) {
            executedInstructions.push(writeBackStage.front());
            writeBackStage.pop();
        }
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
        if (executeStage.front().checkFloat() == false || fpDelay >= 5) {
            fpDelay = 0;
            memoryStage.push(executeStage.front());
            executeStage.pop();
        } else fpDelay++;
    }

    // Check and move instructions from Memory to WriteBack stage
    else if (!memoryStage.empty()) {
        if ((memoryStage.front().checkmemWrite() == false && memoryStage.front().checkmemRead() == false) || memDelay == true) {
            memDelay = 0;
            writeBackStage.push(memoryStage.front());
            memoryStage.pop();
        } else memDelay = true;
    }
}

// place update and fluch pipeline here when done



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
inline void CPU::printExecutedInstructions() {
    std::cout << "*********Executed Instructions**********\n";
    uint32_t i = 0;
    while (!executedInstructions.empty()) {
        std::cout << "Instruction #" << i++ << ":\t" << executedInstructions.front().getAssemblyString() << "\n";
        executedInstructions.pop();
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

    events.push(Event(currentTick, prevPC, pc, fetchString, decodeString, executeString, memoryString, writeBackString));
}

inline void CPU::runCPUcycle() {
    if (reset == false) {
        // Save pc at start of cycle
        prevPC = pc;

        // Update datapath
        if (pipeline) updatePipeline();
        else updateDataPath();
        
        // Run each stage of the pipeline
        Fetch();
        Decode();
        Execute();
        Memory();
        WriteBack();

        // Update System Clock
        currentTick += 10;

        // Update event log for each cycle
        updateEventQueue();
// look at x1 end condition
        if (fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty() && writeBackStage.empty()) {
            reset = true;
            std::cout << "All Stages of the data path are empty. Resetting CPU.\n";
            return;
        }

        // debug statements
        std::cout << "///////Cpu Cycle Debug Statements///////\n";
        // std::cout << "PC: " << pc << "\n";
        std::cout << "X1 value: " << std::dec << readIntRegister(1) << "\n";
    }
}

inline void CPU::flushPipeline() {
// need to fic pc decrement after flush
    if (decodeStage.empty()) { return; }
    
    if (decodeStage.front().checkPCsel()) {
        if (!fetchStage.empty()) {
            discardedInstructions.push(fetchStage.front());
            fetchStage.pop();
            std::cout << "Fetch stage flushed:\n";
            // if (pc >= 4) 
                // pc -= 4; // seg fault if un commented
                // prevPC -=4;
                // std::cout << "prevPC - 4 = " << prevPC << "\n";
                // pc = prevPC;
                // 
        }
    }
    return;
}

inline bool CPU::controlHazardCheck() {
    // PRIORITY ONE#### need to fix logic for not fetching while branch/jump in data path
    if (fetchStage.empty()) {
        std::cout << "Branch detected in: ";
        if (decodeStage.front().checkPCsel() && !decodeStage.empty()) {
            fetchStage.push(0b1111111);
            std::cout << "Decode\n";
        }

        else if (executeStage.front().checkPCsel() && !executeStage.empty()) {
            fetchStage.push(0b1111111);
            std::cout << "Execute\n";
        }

        // else if (memoryStage.front().checkPCsel() /*&& !memoryStage.empty()*/) {
        //     fetchStage.push(0b1111111);
        //     std::cout << "Memory\n";
        // }
        else {
            std::cout << "None\n";
        }
    }

    // 
}

// handle hazards with bubbling, look into raw war waw
inline bool CPU::dataHazardCheck() {

    return false;
}


inline void CPU::updatePipeline() {
    // run branch prediction first before updating pipeline
// need to prevent from grabbing instruction again after flush, fix before fixing the decrease on flush
// getting +8 too high on pc, 4 is from not decreasing the pc after flush, the other 4 is because double grabbing
    
    flushPipeline();
    controlHazardCheck();
    // dataHazardCheck();
    
    // Check and update Write Back Stage. Move instructions from here to queue of executed functions
    if (!writeBackStage.empty()) {
        executedInstructions.push(writeBackStage.front());
        writeBackStage.pop();
    }

    // Check and move instructions from Memory to WriteBack stage
    if (!memoryStage.empty() && writeBackStage.empty()) {
        // if ((memoryStage.front().checkmemWrite() == false && memoryStage.front().checkmemRead() == false) || memDelay == true) {
            // memDelay = 0;
            writeBackStage.push(memoryStage.front());
            memoryStage.pop();
        // } else memDelay = true;
    }

    // Check and move instructions from Execute to Memory stage
    if (!executeStage.empty() && memoryStage.empty()) {
        // if (executeStage.front().checkFloat() == false || fpDelay > 5) {
            // fpDelay = 0;
            memoryStage.push(executeStage.front());
            executeStage.pop();
        // } else fpDelay++;
    }

    // Check and move instructions from Decode to Execute stage
    if (!decodeStage.empty() && executeStage.empty()) {
        // // Check for Data Hazards
        // uint32_t rd = memoryStage.front().getrd();
        // uint32_t rs1 = decodeStage.front().getrs1();
        // uint32_t rs2 = decodeStage.front().getrs2();
        // if ((!memoryStage.empty() && ((rd != rs1) && (rd != rs2))) || memoryStage.empty()) {
            executeStage.push(decodeStage.front());
            decodeStage.pop();
        // }
    }

    // Check and move instructions from Fetch to Decode stage
    if (!fetchStage.empty() && decodeStage.empty()) {
        // if (fetchStage.front().getInstruction() != 0b1111111) {
            decodeStage.push(fetchStage.front());
            fetchStage.pop();
        // }
    }
            

    // Update Fetch stage
    if (fetchStage.empty()) {
        if (ram.Read(pc) != 0) {
            fetchStage.push(0);
            std::cout << "New Instruction Fetched\n";
        }
    }

    // if (fetchStage.front().getInstruction() == 0b1111111 && !fetchStage.empty()) {
    //     fetchStage.pop();
    // }


}