#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <stdint.h>

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

    // Instruction Variables
    uint32_t opcode, rs1, rs2, rd, funct3, funct7;
    int32_t imm = 0;

    int32_t getImm() { return imm; }

    // Control Signals
    bool regWrite, ALUsrc, memWrite, memRead, memToReg, branch, jump, PCtoReg, RegToPC, rm;
    int ALUop;

    bool checkregWrite() { return regWrite; }
    bool checkALUsrc() { return ALUsrc; }
    bool checkmemWrite() { return memWrite; }
    bool checkmemRead() { return memRead; }
    bool checkmemToReg() { return memToReg; }
    bool checkBranch() { return branch; }
    bool checkJump() { return jump; }
    bool checkPCtoReg() { return PCtoReg; }
    bool checkRegToPC() { return RegToPC; }
    bool checkRM() { return rm; }
    int checkALUop() { return ALUop; }

    // Decode functions
    int32_t getImmediate(uint32_t instruction);
    void printInstruction();
    void printAssembly();

    Instruction(uint32_t instruction) {
        this->instruction = instruction;
        opcode = (instruction & 0x7F);
        rs1 = (instruction >> 15) & 0x1F;
        rs2 = (instruction >> 20) & 0x1F;
        rd = (instruction >> 7) & 0x1F;
        funct3 = (instruction >> 12) & 0x7;
        funct7 = (instruction >> 25) & 0x7F;
        imm = getImmediate(instruction);

        switch (opcode) {
            case LOAD:
                regWrite = 1;
                ALUop = 0;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 1;
                memToReg = 1;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
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
                regWrite = 1;
                ALUop = 0;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 1;
                memToReg = 1;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
                if(funct3 != 0b010) break;
                operation = flw;
                break;
            case I_TYPE:
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
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
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
                operation = auipc;
                break;
            case S_TYPE:
                regWrite = 0;
                ALUop = 0;
                ALUsrc = 1;
                memWrite = 1;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
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
                regWrite = 0;
                ALUop = 0;
                ALUsrc = 1;
                memWrite = 1;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
                if(funct3 != 0b010) break;
                operation = fsw;
                break;
            case R_TYPE:
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 0;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
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
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
                operation = lui;
                break;
            case SB_TYPE:
                regWrite = 0;
                ALUop = 1;
                ALUsrc = 0;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 1;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 0;
                rm = 0;
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
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 1;
                PCtoReg = 1;
                RegToPC = 1;
                rm = 0;
                operation = jalr;
                break;
            case JAL:
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 1;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 1;
                PCtoReg = 1;
                RegToPC = 0;
                rm = 0;
                operation = jal;
                break;
            case FP_TYPE:
                regWrite = 1;
                ALUop = 2;
                ALUsrc = 0;
                memWrite = 0;
                memRead = 0;
                memToReg = 0;
                branch = 0;
                jump = 0;
                PCtoReg = 0;
                RegToPC = 1;
                rm = 0;
                switch (funct7) {
                    case 0b0000000:
                        rm = 1;
                        operation = fadd_s;
                        break;
                    case 0b0000100:
                        rm = 1;
                        operation = fsub_s;
                        break;
                    case 0b0001000:
                        rm = 1;
                        operation = fmul_s;
                        break;
                    case 0b0001100:
                        rm = 1;
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
                        rm = 1;
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
                        rm = 1;
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
                        rm = 1;
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
                std::cout << "***************Unknown instruction.***************\n";
                std::cout << "Magic 8-ball says you suck, try again.\n";
                break;
        }
        // printInstruction();
    }
};

#endif //INSTRUCTION_H