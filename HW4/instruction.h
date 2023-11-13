#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <sstream>
// #include <stdint.h>

typedef uint64_t tick_t;

// Define RISC-V opcodes and instruction formats
#define LOAD         0b0000011
#define LOAD_FP      0b0000111
#define I_TYPE       0b0010011
#define AUIPC        0b0010111
#define S_TYPE       0b0100011
#define S_TYPE_FP    0b0100111
#define R_TYPE       0b0110011
#define LUI          0b0110111
#define SB_TYPE      0b1100011
#define JALR         0b1100111
#define JAL          0b1101111
#define FP_TYPE      0b1010011

#define STALL 0b00000000000000000000000000010011    // ADDI X0, X0, 0

enum ALUop {
    ADD = 0,    // 0b0000
    SUB,        // 0b0001
    AND,        // 0b0010
    OR,         // 0b0011
    XOR,        // 0b0100
    SLL,        // 0b0101
    SRL,        // 0b0110
    SRA,        // 0b0111
    SLT,        // 0b1000
    SLTU,       // 0b1001
    MUL,        // 0b
    MULH,       // 0b
    MULHSU,     // 0b
    MULHU,      // 0b
    DIV,        // 0b
    DIVU,       // 0b
    REM,        // 0b
    REMU,       // 0b
    ADD_FP,     // 0b
    SUB_FP,     // 0b
    MUL_FP,     // 0b
    DIV_FP,     // 0b
};

// Define a class for RISC-V instructions
class Instruction {
public:
    uint32_t instruction;
    std::string assemblyString;

    // Instruction Variables
    uint32_t opcode, rs1, rs2, rd, funct3, funct7;
    int32_t imm = 0;

    // Check Instruction Variables
    uint32_t getrd() { return rd; }
    uint32_t getOpcode() { return opcode; }
    uint32_t getrs1() { return rs1; }
    uint32_t getrs2() { return rs2; }
    uint32_t getfunct3() { return funct3; }
    int32_t getImm() { return imm; }

    // Control Signals
    bool regWrite, ALUsrc1, ALUsrc2, memWrite, memRead, branch, jump, rm, PCsel;
    int ALUop, WBsel;

    // Check Control Signals
    bool checkregWrite() { return regWrite; }
    bool checkALUsrc1() { return ALUsrc1; }
    bool checkALUsrc2() { return ALUsrc2; }
    bool checkmemWrite() { return memWrite; }
    bool checkmemRead() { return memRead; }
    bool checkBranch() { return branch; }
    bool checkJump() { return jump; }
    bool checkRM() { return rm; }
    bool checkPCsel() { return PCsel; }
    int checkALUop() { return ALUop; }
    int checkWBsel() { return WBsel; }

    // Float
    bool isFloat;
    void setIsFloat();
    bool checkFloat() { return isFloat; }    

    // Decode functions
    int32_t setImmediate();
    void setregWrite();
    void setALUsrc1();
    void setALUsrc2();
    void setmemWrite();
    void setmemRead();
    void setBranch();
    void setJump();
    void setRM();
    void setPCsel(bool);
    void setALUop();
    void setWBsel();

    void printSignals();
    void printInstruction();
    void printAssembly();
    void assembleString();    
    std::string getAssemblyString() { return assemblyString; }
    
    // ALU related variables and functions 
    int32_t ALUresult;
    void setALUresult(int32_t ALUresult) { this->ALUresult = ALUresult; }
    int32_t getALUresult() { return ALUresult; }

    float ALUfloatResult;
    void setALUfloatResult(float ALUresultFP) { this->ALUfloatResult = ALUfloatResult; }
    float getALUfloatResult() { return ALUfloatResult; }

    Instruction(uint32_t instruction) {
        // Get Values
        this->instruction = instruction;
        opcode = (instruction & 0x7F);
        rs1 = (instruction >> 15) & 0x1F;
        rs2 = (instruction >> 20) & 0x1F;
        rd = (instruction >> 7) & 0x1F;
        funct3 = (instruction >> 12) & 0x7;
        funct7 = (instruction >> 25) & 0x7F;
        imm = setImmediate();
        assembleString();

        ALUresult = 0;
        ALUfloatResult = 0;
    }
};

#endif //INSTRUCTION_H