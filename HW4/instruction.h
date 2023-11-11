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

enum instructions {
    lb, lh, lw, lbu, lhu, flw,
    addi, slli, slti, sltui, xori, srli, srai, ori, andi,
    auipc,
    sb, sh, sw, fsw,
    add, mul, sub, sll, mulh, slt, mulhsu, sltu, mulhu, _xor, _div, srl, divu, sra, _or, rem, _and, remu, 
    lui, 
    beq, bne, blt, bge, bltu, bgeu, 
    jalr, jal, 
    fadd_s, fsub_s, fmul_s, fdiv_s, fsgnj_s, fsgnjn_s, fsgnjx_s, fmin_s, fmax_s, fsqrt_s, feq_s, flt_s, fle_s, 
    fcvt_w_s, fcvt_wu_s, fcvt_s_w, fcvt_s_wu, fmv_x_w, fclass_s, fmv_w_x
};

// Define a class for RISC-V instructions
class Instruction {
public:
    uint32_t instruction;
    int operation;
    std::string assemblyString;

    // Instruction Variables
    uint32_t opcode, rs1, rs2, rd, funct3, funct7;
    int32_t imm = 0;

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
    bool checkFloat() { return isFloat; }

    // Decode functions
    int32_t setImmediate(uint32_t);
    void setregWrite(uint32_t);
    void setALUsrc1(uint32_t);
    void setALUsrc2(uint32_t);
    void setmemWrite(uint32_t);
    void setmemRead(uint32_t);
    void setBranch(uint32_t);
    void setJump(uint32_t);
    void setRM(uint32_t, uint32_t);
    void setPCsel(uint32_t, bool);
    void setALUop(uint32_t);
    void setWBsel(uint32_t);

    void printSignals();
    void assembleString();    
    std::string getAssemblyString() { return assemblyString; }
    
    uint32_t ALUresult;
    void setALUresult(uint32_t);
    uint32_t getALUresult() { return ALUresult; }

    Instruction(uint32_t instruction) {
        // Get Values
        this->instruction = instruction;
        opcode = (instruction & 0x7F);
        rs1 = (instruction >> 15) & 0x1F;
        rs2 = (instruction >> 20) & 0x1F;
        rd = (instruction >> 7) & 0x1F;
        funct3 = (instruction >> 12) & 0x7;
        funct7 = (instruction >> 25) & 0x7F;
        imm = setImmediate(opcode);

        // Set Control Signals
        setregWrite(opcode);
        setALUsrc1(opcode);
        setALUsrc1(opcode);
        setmemWrite(opcode);
        setmemRead(opcode);
        setBranch(opcode);
        setJump(opcode);
        setRM(opcode, funct7);
        setALUop(opcode);
        setWBsel(opcode);

        assembleString();

        switch (opcode) {
            case LOAD_FP:
            case S_TYPE_FP:
            case FP_TYPE:
                // isFloat = true;
                // break;
            default: 
                isFloat = false;
        }

        switch (opcode) {
            case LOAD:
                switch (funct3) {
                    case 0b000:
                        operation = lb;
                        break;
                    case 0b001:
                        operation = lh;
                        break;
                    case 0b010:
                        operation = lw;
                        break;
                    case 0b100:
                        operation = lbu;
                        break;
                    case 0b101:
                        operation = lhu;
                        break;
                }
                break;
            case LOAD_FP:
                if(funct3 != 0b010) break;
                operation = flw;
                break;
            case I_TYPE:
                switch (funct3) {
                    case 0b000:
                        operation = addi;
                        break;
                    case 0b001:
                        operation = slli;
                        break;
                    case 0b010:
                        operation = slti;
                        break;
                    case 0b011:
                        operation = sltui;
                        break;
                    case 0b100:
                        operation = xori;
                        break;
                    case 0b101:
                        if (funct7 == 0b0000000) {
                            operation = srli;
                        } else {
                            operation = srai;
                        }
                        break;
                    case 0b110:
                        operation = ori;
                        break;
                    case 0b111:
                        operation = andi;
                        break;
                }
                break;
            case AUIPC:
                operation = auipc;
                break;
            case S_TYPE:
                switch (funct3) {
                    case 0b000:
                        operation = sb;
                        break;
                    case 0b001:
                        operation = sh;
                        break;
                    case 0b010:
                        operation = sw;
                        break;
                }
                break;
            case S_TYPE_FP:
                if(funct3 != 0b010) break;
                operation = fsw;
                break;
            case R_TYPE:
                switch (funct3) {
                    case 0b000:
                        switch (funct7) {
                            case 0b0000000:
                                operation = add;
                                break;
                            case 0b0000001:
                                operation = mul;
                                break;
                            case 0b0100000:
                                operation = sub;
                                break;
                        }
                        break;
                    case 0b001:
                        switch (funct7) {
                            case 0b0000000:
                                operation = sll;
                                break;
                            case 0b0000001:
                                operation = mulh;
                                break;
                        }
                        break;
                    case 0b010:
                        switch (funct7) {
                            case 0b0000000:
                                operation = slt;
                                break;
                            case 0b0000001:
                                operation = mulhsu;
                                break;
                        }
                        break;
                    case 0b011:
                        switch (funct7) {
                            case 0b0000000:
                                operation = sltu;
                                break;
                            case 0b0000001:
                                operation = mulhu;
                                break;
                        }
                        break;
                    case 0b100:
                        switch (funct7) {
                            case 0b0000000:
                                operation = _xor;
                                break;
                            case 0b0000001:
                                operation = _div;
                                break;
                        }
                        break;
                    case 0b101:
                        switch (funct7) {
                            case 0b0000000:
                                operation = srl;
                                break;
                            case 0b0000001:
                                operation = divu;
                                break;
                            case 0b0100000:
                                operation = sra;
                                break;
                        }
                        break;
                    case 0b110:
                        switch (funct7) {
                            case 0b0000000:
                                operation = _or;
                                break;
                            case 0b0000001:
                                operation = rem;
                                break;
                        }
                        break;
                    case 0b111:
                        switch (funct7) {
                            case 0b0000000:
                                operation = _and;
                                break;
                            case 0b0000001:
                                operation = remu;
                                break;
                        }
                        break;
                    break;
                }
                break;
            case LUI:
                operation = lui;
                break;
            case SB_TYPE:
                switch (funct3) {
                    case 0b000:
                        operation = beq;
                        break;
                    case 0b001:
                        operation = bne;
                        break;
                    case 0b100:
                        operation = blt;
                        break;
                    case 0b101:
                        operation = bge;
                        break;
                    case 0b110:
                        operation = bltu;
                        break;
                    case 0b111:
                        operation = bgeu;
                        break;
                }
                break;
            case JALR:
                operation = jalr;
                break;
            case JAL:
                operation = jal;
                break;
            case FP_TYPE:
                switch (funct7) {
                    case 0b0000000:
                        operation = fadd_s;
                        break;
                    case 0b0000100:
                        operation = fsub_s;
                        break;
                    case 0b0001000:
                        operation = fmul_s;
                        break;
                    case 0b0001100:
                        operation = fdiv_s;
                        break;
                    case 0b0010000:
                        switch (funct3) {
                            case 0b000:
                                operation = fsgnj_s;
                                break;
                            case 0b001:
                                operation = fsgnjn_s;
                                break;
                            case 0b010:
                                operation = fsgnjx_s;
                                break;
                        }
                        break;
                    case 0b0010100:
                        switch (funct3) {
                            case 0b000:
                                operation = fmin_s;
                                break;
                            case 0b001:
                                operation = fmax_s;
                                break;
                        }
                        break;
                    case 0b0101100:
                        operation = fsqrt_s;
                        break;
                    case 0b1010000:
                        switch (funct3) {
                            case 0b000:
                                operation = feq_s;
                                break;
                            case 0b001:
                                operation = flt_s;
                                break;
                            case 0b010:
                                operation = fle_s;
                                break;
                        }
                        break;
                    case 0b1100000:
                        switch (rs2) {
                            case 0b00000:
                                operation = fcvt_w_s;
                                break;
                            case 0b00001:
                                operation = fcvt_wu_s;
                                break;
                        }
                        break;
                    case 0b1101000:
                        switch (rs2) {
                            case 0b00000:
                                operation = fcvt_s_w;
                                break;
                            case 0b00001:
                                operation = fcvt_s_wu;
                                break;
                        }
                        break;
                    case 0b1110000:
                        switch (funct3) {
                            case 0b00000:
                                operation = fmv_x_w;
                                break;
                            case 0b00001:
                                operation = fclass_s;
                                break;
                        }
                        break;
                    case 0b1111000:
                        operation = fmv_w_x;
                        break;
                    break;
                }
                break;
            default:
                // std::cout << "***************Unknown instruction.***************\n";
                // std::cout << "Magic 8-ball says you suck, try again.\n";
                break;
        }
        // printInstruction();
    }
};

#endif //INSTRUCTION_H