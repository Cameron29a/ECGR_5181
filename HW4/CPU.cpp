#include "CPU.h"

inline void CPU::Fetch() {
    // Fetch an instruction from memory based on the current PC
    if (fetchStage.empty()) { return; }

    std::cout << "//////////////Fetch Stage///////////////\n";

    // Flow control places an empty instruction in the queue, so this stage will activate
    // If there is a stall, the pc will not update twice on the same instruction
    if (fetchStage.front().getInstruction() == 0) {
        fetchStage.pop();
        fetchStage.push(memBus.readInstruction(pc, instructionStart));
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

    // Adjust PC if there is a branch or jump
    if (memoryStage.front().checkPCsel()) {
        pc = memoryStage.front().getALUresult();
        std::cout << "Next Instruction: PC = ALU result\n";
        return;
    }

    if (!memoryStage.front().checkmemRead() && !memoryStage.front().checkmemWrite()) { return; }

    if (!memRequestPending) {
        if (memoryStage.front().checkmemRead()) {
            // Memory read operation
            // Set the loaded data to the destination register
            if (memoryStage.front().checkFloat()) {
                float memoryAddressFP = stackAddress + memoryStage.front().getALUfloatResult();
                Read(memoryAddressFP); // read float
                std::cout << "FP Memory Read Operation\n";
                // std::cout << "Memory Address: " << memoryAddressFP << "\n";
            } else {
                uint32_t memoryAddress = stackAddress + memoryStage.front().getALUresult();
                Read(memoryAddress);
                std::cout << "Int Memory Read Operation\n";
                // std::cout << "Memory Address: " << memoryAddress << "\n";
            }
        }
        else if (memoryStage.front().checkmemWrite()) {
            // Memory write operation
            // Write Data to the memory location
            if (memoryStage.front().checkFloat()) {
                float memoryAddressFP = stackAddress + memoryStage.front().getALUresult();
                float dataToStoreFP = registers.fp_regs[memoryStage.front().getrs2()];
                uint32_t memoryAddress = static_cast<uint32_t>(memoryAddressFP);
                uint32_t dataToStore = static_cast<uint32_t>(dataToStoreFP);
                Write(memoryAddress, dataToStore);
                std::cout << "FP Memory Write Operation\n";
                std::cout << "Memory Address: " << memoryAddress << "\n";
                std::cout << "Data To Store: " << dataToStoreFP << "\n";
            } else {
                uint32_t memoryAddress = stackAddress + memoryStage.front().getALUresult();
                uint32_t dataToStore = registers.int_regs[memoryStage.front().getrs2()];
                Write(memoryAddress, dataToStore);
                std::cout << "Int Memory Write Operation\n";
                std::cout << "Memory Address: " << memoryAddress << "\n";
                std::cout << "Data To Store: " << dataToStore << "\n";
            }
        }
        memRequestPending = true;
    } else {
        if (memBus.isMemoryRequestComplete()) {
            if (memoryStage.front().checkmemRead()) {
                if (memoryStage.front().checkFloat()) {

                    memoryStage.front().loadDataFP(memBus.getMemoryResponse(cpuId));
                } else {
                    memoryStage.front().loadData(memBus.getMemoryResponse(cpuId));
                }
            }
            memRequestPending = false;
               // Return if request isn't complete
        } if (!memBus.isWriteRequestPending(cpuId)) {
            memRequestPending = false;
        } else {
            std::cout << "Memory Access still Pending\n";
            return; 
        }
    }
}

inline void CPU::WriteBack() {
    // Perform memory write back
    if (writeBackStage.empty()) { return; }
    std::cout << "/////////////WriteBack Stage////////////\n";

    switch(writeBackStage.front().checkWBsel()){
        case 0:
            if (writeBackStage.front().checkFloat()) {
                registers.fp_regs[writeBackStage.front().getrd()] = writeBackStage.front().getLoadedDataFP();
                std::cout << "FP Write Back Case 0\n";
            } else {
                registers.int_regs[writeBackStage.front().getrd()] = writeBackStage.front().getLoadedData();
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
        default:
            return;
    }
}

inline void CPU::updateDataPath() {
    // Update Fetch stage
    if (fetchStage.empty() && decodeStage.empty() && executeStage.empty() && memoryStage.empty()) {
        if (memBus.readInstruction(pc, instructionStart) != 0) fetchStage.push(0);
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
        if(!memRequestPending) {
            writeBackStage.push(memoryStage.front());
            memoryStage.pop();
        }
    }
}

// place update and fluch pipeline here when done





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
    std::cout << "***********CPU:" << cpuId << " Event Queue************\n";
    while (!events.empty()) {
        events.front().print();
        events.pop();
    }
}

// Function to print the current event
inline void CPU::printCurrentEvent() {
    std::cout << "*************CPU:" << cpuId << " Current Event**************\n";
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
        std::cout << "Start Cycle for CPU:" << cpuId << "\n";
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
    if (decodeStage.front().checkPCsel() && !decodeStage.empty()) {
        if (!fetchStage.empty()) {
            discardedInstructions.push(fetchStage.front());
            fetchStage.pop();
            std::cout << "Fetch stage flushed:\n";
            pc -= 4;
        }
    }
}

inline void CPU::controlHazardCheck() {
    if (executeStage.front().checkPCsel() && !executeStage.empty()) {
        std::cout << "Control Hazard detected in: Execute\n";
        controlHazard = 1;
    } else if (memoryStage.front().checkPCsel() && !memoryStage.empty()) {
        std::cout << "Control Hazard detected in: Memory\n";
        controlHazard = 1;
    } else {
        controlHazard = 0;
    }
}

inline void CPU::dataHazardCheck() {
    // Check for Data Hazards
    if (!memoryStage.empty()) {
        uint32_t rd = memoryStage.front().getrd();
        uint32_t rs1 = decodeStage.front().getrs1();
        uint32_t rs2 = decodeStage.front().getrs2();
        if ((rd == rs1) || (rd == rs2)) {
            std::cout << "Data Hazard detected in: Memory\n";
            dataHazard = 1;
        }
    } else if (!writeBackStage.empty()) {
        uint32_t rd = writeBackStage.front().getrd();
        uint32_t rs1 = decodeStage.front().getrs1();
        uint32_t rs2 = decodeStage.front().getrs2();
        if ((rd == rs1) || (rd == rs2)) {
            std::cout << "Data Hazard detected in: Write Back\n";
            dataHazard = 1;
        }
    } else {
        dataHazard = 0;
    }
}

inline void CPU::updatePipeline() {
    // flush before updating pipeline
    flushPipeline();
    
    // Check and update Write Back Stage. Move instructions from here to queue of executed functions
    if (!writeBackStage.empty()) {
        executedInstructions.push(writeBackStage.front());
        writeBackStage.pop();
    }

    // Check and move instructions from Memory to WriteBack stage
    if (!memoryStage.empty() && writeBackStage.empty()) {
        if (!memRequestPending) {
            writeBackStage.push(memoryStage.front());
            memoryStage.pop();
        }
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
        dataHazardCheck();
        if(!dataHazard) {
            executeStage.push(decodeStage.front());
            decodeStage.pop();
        }
    }

    // Check and move instructions from Fetch to Decode stage
    if (!fetchStage.empty() && decodeStage.empty()) {
        decodeStage.push(fetchStage.front());
        fetchStage.pop();
    }
            
    // Update Fetch stage
    if (fetchStage.empty()) {
        controlHazardCheck();
        if (!controlHazard) {
            if (memBus.readInstruction(pc, instructionStart) != 0) {
                fetchStage.push(0);
                // std::cout << "New Instruction Fetched\n";
            }
        }
    }
}