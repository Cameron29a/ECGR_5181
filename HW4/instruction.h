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
// enum Format { R_FORMAT, I_FORMAT, S_FORMAT, B_FORMAT, U_FORMAT, J_FORMAT };

// Define a class for RISC-V instructions
class Instruction {
public:
    uint32_t instruction;
    // Format format;

    // Instruction Variables
    uint32_t opcode, rs1, rs2, rd, funct3, funct7;
    int32_t imm;

    // Control Signals
    bool regWrite, ALUsrc, memWrite, memRead, memToReg, branch, jump, PCtoReg, RegToPC, rm = 0;
    int ALUop;

    // Decode functions
    // void decodeInstruction(uint32_t instruction);
    uint32_t getRS1(uint32_t instruction);
    uint32_t getRS2(uint32_t instruction);
    uint32_t getRD(uint32_t instruction);
    int32_t getImmediate(uint32_t instruction);
    uint32_t getFunct3(uint32_t instruction);
    uint32_t getFunct7(uint32_t instruction);

    Instruction(uint32_t instruction) {
        // void decodeInstruction(uint32_t instruction);
        opcode = instruction & 0x7F;
        switch (opcode) {
            case LOAD:
                std::cout << "Executing LOAD instruction.\n";
                rs1 = getRS1(instruction);
                rd = getRD(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                        std::cout << "lb";
                        break;
                    case 0b001:
                        std::cout << "lh";
                        break;
                    case 0b010:
                        std::cout << "lw";
                        break;
                    case 0b100:
                        std::cout << "lbu";
                        break;
                    case 0b101:
                        std::cout << "lhu";
                        break;
                }
                std::cout << " x" << rd << ", " << imm << "(x" << rs1 << ")\n";
                break;
            case LOAD_FP:
                std::cout << "Executing LOAD_FP instruction.\n";
                rs1 = getRS1(instruction);
                rd = getRD(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                std::cout << "flw f" << rd << ", " << imm << "(x" << rs1 << ")\n";
                break;
            case I_TYPE:
                std::cout << "Executing I_TYPE instruction.\n";
                rs1 = getRS1(instruction);
                rd = getRD(instruction);
                imm = getImmediate(instruction);   
                funct3 = getFunct3(instruction);
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
                        std::cout << "addi";
                        break;
                    case 0b001:
                        std::cout << "slli";
                        break;
                    case 0b010:
                        std::cout << "slti";
                        break;
                    case 0b011:
                        std::cout << "sltui";
                        break;
                    case 0b100:
                        std::cout << "xori";
                        break;
                    case 0b101:
                        if (funct7 == 0b0000000) std::cout << "srli";
                        else std::cout << "srai";
                        break;
                    case 0b110:
                        std::cout << "ori";
                        break;
                    case 0b111:
                        std::cout << "andi";
                        break;
                }
                std::cout << " x" << rd << ", x" << rs1 << ", " << imm << "\n";
                break;
            case AUIPC:
                std::cout << "Executing AUIPC instruction.\n";
                rd = getRD(instruction);
                imm = getImmediate(instruction);
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
                std::cout << "auipc x" << rd << ", " << imm << "\n";
                break;
            case S_TYPE:
                std::cout << "Executing S_TYPE instruction.\n";
                rs1 = getRS1(instruction);
                rs2 = getRS2(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                        std::cout << "sb";
                        break;
                    case 0b001:
                        std::cout << "sh";
                        break;
                    case 0b010:
                        std::cout << "sw";
                        break;
                }
                std::cout << " x" << rs2 << ", " << imm << "(x" << rs1 << ")\n";
                break;
            case S_TYPE_FP:
                std::cout << "Executing S_TYPE_FP instruction.\n";
                rs1 = getRS1(instruction);
                rs2 = getRS2(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                std::cout << "fsw f" << rs2 << ", " << imm << "(x" << rs1 << ")\n";
                break;
            case R_TYPE:
                std::cout << "Executing R_TYPE instruction.\n";
                rs1 = getRS1(instruction);
                rs2 = getRS2(instruction);
                rd = getRD(instruction);
                funct3 = getFunct3(instruction);
                funct7 = getFunct7(instruction);
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
                                std::cout << "add";
                                break;
                            case 0b0000001:
                                std::cout << "mul";
                                break;
                            case 0b0100000:
                                std::cout << "sub";
                                break;
                        }
                        break;
                    case 0b001:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "sll";
                                break;
                            case 0b0000001:
                                std::cout << "mulh";
                                break;
                        }
                        break;
                    case 0b010:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "slt";
                                break;
                            case 0b0000001:
                                std::cout << "mulhsu";
                                break;
                        }
                        break;
                    case 0b011:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "sltu";
                                break;
                            case 0b0000001:
                                std::cout << "mulhu";
                                break;
                        }
                        break;
                    case 0b100:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "xor";
                                break;
                            case 0b0000001:
                                std::cout << "div";
                                break;
                        }
                        break;
                    case 0b101:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "srl";
                                break;
                            case 0b0000001:
                                std::cout << "divu";
                                break;
                            case 0b0100000:
                                std::cout << "sra";
                                break;
                        }
                        break;
                    case 0b110:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "or";
                                break;
                            case 0b0000001:
                                std::cout << "rem";
                                break;
                        }
                        break;
                    case 0b111:
                        switch (funct7) {
                            case 0b0000000:
                                std::cout << "and";
                                break;
                            case 0b0000001:
                                std::cout << "remu";
                                break;
                        }
                        break;
                    break;
                }
                std::cout << " x" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
                break;
            case LUI:
                std::cout << "Executing LUI instruction.\n";
                rd = getRD(instruction);
                imm = getImmediate(instruction);
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
                std::cout << "lui x" << rd << ", " << imm << "\n";
                break;
            case SB_TYPE:
                std::cout << "Executing SB_TYPE instruction.\n";
                rs1 = getRS1(instruction);
                rs2 = getRS2(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                        std::cout << "beq";
                        break;
                    case 0b001:
                        std::cout << "bne";
                        break;
                    case 0b100:
                        std::cout << "blt";
                        break;
                    case 0b101:
                        std::cout << "bge";
                        break;
                    case 0b110:
                        std::cout << "bltu";
                        break;
                    case 0b111:
                        std::cout << "bgeu";
                        break;
                }
                std::cout << " x" << rs1 << ", x" << rs2 << ", " << imm << "\n";
                break;
            case JALR:
                std::cout << "Executing JALR instruction.\n";
                rs1 = getRS1(instruction);
                rd = getRD(instruction);
                imm = getImmediate(instruction);
                funct3 = getFunct3(instruction);
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
                std::cout << "jalr x" << rd << ", x" << rs1 << ", " << imm << "\n";
                break;
            case JAL:
                std::cout << "Executing JAL instruction.\n";
                rd = getRD(instruction);
                imm = getImmediate(instruction);
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
                std::cout << "jal x" << rd << ", " << imm << "\n";
                break;
            case FP_TYPE:
                std::cout << "Executing FP_TYPE instruction.\n";
                rs1 = getRS1(instruction);
                rs2 = getRS2(instruction);
                rd = getRD(instruction);
                funct3 = getFunct3(instruction);
                funct7 = getFunct7(instruction);
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
                        std::cout << "fadd.s";
                        break;
                    case 0b0000100:
                        rm = 1;
                        std::cout << "fsub.s";
                        break;
                    case 0b0001000:
                        rm = 1;
                        std::cout << "fmul.s";
                        break;
                    case 0b0001100:
                        rm = 1;
                        std::cout << "fdiv.s";
                        break;
                    case 0b0010000:
                        switch (funct3) {
                            case 0b000:
                                std::cout << "fsgnj.s";
                                break;
                            case 0b001:
                                std::cout << "fsgnjn.s";
                                break;
                            case 0b010:
                                std::cout << "fsgnjx.s";
                                break;
                        }
                        break;
                    case 0b0010100:
                        switch (funct3) {
                            case 0b000:
                                std::cout << "fmin.s";
                                break;
                            case 0b001:
                                std::cout << "fmax.s";
                                break;
                        }
                        break;
                    case 0b0101100:
                        rm = 1;
                        std::cout << "fsqrt.s";
                        break;
                    case 0b1010000:
                        switch (funct3) {
                            case 0b000:
                                std::cout << "feq.s";
                                break;
                            case 0b001:
                                std::cout << "flt.s";
                                break;
                            case 0b010:
                                std::cout << "fle.s";
                                break;
                        }
                        break;
                    case 0b1100000:
                        switch (rs2) {
                            case 0b00000:
                                std::cout << "fcvt.w.s";
                                break;
                            case 0b00001:
                                std::cout << "fcvt.wu.s";
                                break;
                        }
                        rm = 1;
                        break;
                    case 0b1101000:
                        switch (rs2) {
                            case 0b00000:
                                std::cout << "fcvt.s.w";
                                break;
                            case 0b00001:
                                std::cout << "fcvt.s.wu";
                                break;
                        }
                        rm = 1;
                        break;
                    case 0b1110000:
                        switch (funct3) {
                            case 0b00000:
                                std::cout << "fmv.x.w";
                                break;
                            case 0b00001:
                                std::cout << "fclass.s";
                                break;
                        }
                        break;
                    case 0b1111000:
                        std::cout << "fmv.w.x";
                        break;
                    break;
                }
                std::cout << " rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << "\n";
                break;
            default:
                std::cout << "Unknown instruction.\n";
                std::cout << "Magic 8-ball says you suck, try again.\n";
                break;
        }
        std::cout << "regWrite = " << regWrite << "\n"
            << "ALUop = " << ALUop << "\n"
            << "ALUsrc = " << ALUsrc << "\n"
            << "memWrite = " << memWrite << "\n"
            << "memRead = " << memRead << "\n"
            << "memToReg = " << memToReg << "\n"
            << "branch = " << branch << "\n"
            << "jump = " << jump << "\n"
            << "PCtoReg = " << PCtoReg << "\n"
            << "RegToPC = " << RegToPC << "\n"
            << "rm = " << rm << "\n"; 
    }
};

#endif //INSTRUCTION_H





