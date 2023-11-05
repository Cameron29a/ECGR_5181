#include "instruction.h"

uint32_t Instruction::getRS1(uint32_t instruction) {
    uint32_t rs1 = (instruction >> 15) & 0x1F;  // Mask the lower 5 bits (0x1F)
    return rs1;
}

uint32_t Instruction::getRS2(uint32_t instruction) {
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    return rs2;
}

uint32_t Instruction::getRD(uint32_t instruction) {
    uint32_t rd = (instruction >> 7) & 0x1F;
    return rd;
}

int32_t Instruction::getImmediate(uint32_t instruction) {
    switch (instruction & 0x7F) {
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case JALR:
            return static_cast<int32_t>(instruction) >> 20; // Sign-extend the immediate field
        case S_TYPE:
        case S_TYPE_FP:
            return ((static_cast<int32_t>(instruction) >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F); // Sign-extend and combine imm[11:5] and imm[4:0]
        case SB_TYPE:
            return (static_cast<int32_t>(instruction) >> 19) | ((instruction >> 7) & 0x1E) | ((instruction << 4) & 0x800); // Sign-extend and combine bits
        case AUIPC:
        case LUI:
            return instruction & 0xFFFFF000; // Zero-extend the immediate field
        case JAL:
            return ((static_cast<int32_t>(instruction) >> 11) & 0xFFE) | ((instruction >> 20) & 0x7FE) | ((instruction >> 9) & 0x1000) | (instruction & 0xFF000); // Sign-extend and combine bits
        default:
            return 0;
    }
}

uint32_t Instruction::getFunct3(uint32_t instruction) {
    uint32_t funct3 = (instruction >> 12) & 0x7;
    return funct3;
}

uint32_t Instruction::getFunct7(uint32_t instruction) {
    uint32_t funct7 = (instruction >> 25) & 0x7F;
    return funct7;
}

enum instructions {
    lb, lh, lw, lbu, lhu, flw,
    addi, slli, slti, sltui, xori, srli, srai, ori, andi,
    auipc,
    sb, sh, sw, fsw,
    add, mul, sub, sll, mulh, slt, mulhsu, sltu, mulhu, _xor, div, srl, divu, sra, _or, rem, _and, remu, 
    lui, 
    beq, bne, blt, bge, bltu, bgeu, 
    jalr, jal, 
    fadd_s, fsub_s, fmul_s, fdiv_s, fsgnj_s, fsgnjn_s, fsgnjx_s, fmin_s, fmax_s, fsqrt_s, feq_s, flt_s, fle_s, 
    fcvt_w_s, fcvt_wu_s, fcvt_s_w, fcvt_s_wu, fmv_x_w, fclass_s, fmv_w_x
};

// void Instruction::decodeInstruction(uint32_t instruction){
//     opcode = instruction & 0x7F;
//     switch (opcode) {
//         case LOAD:
//             std::cout << "Executing LOAD instruction.\n";
//             rs1 = getRS1(instruction);
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             regWrite = 1;
//             ALUop = 0;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 1;
//             memToReg = 1;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             switch (funct3) {
//                 case 0b000:
//                     std::cout << "lb";
//                     break;
//                 case 0b001:
//                     std::cout << "lh";
//                     break;
//                 case 0b010:
//                     std::cout << "lw";
//                     break;
//                 case 0b100:
//                     std::cout << "lbu";
//                     break;
//                 case 0b101:
//                     std::cout << "lhu";
//                     break;
//             }
//             std::cout << " x" << rd << ", " << imm << "(x" << rs1 << ")\n";
//             break;
//         case LOAD_FP:
//             std::cout << "Executing LOAD_FP instruction.\n";
//             rs1 = getRS1(instruction);
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             std::cout << "f3 = " << funct3 << "\n";
//             regWrite = 1;
//             ALUop = 0;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 1;
//             memToReg = 1;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             if(funct3 != 0b010) break;
//             std::cout << "flw f" << rd << ", " << imm << "(x" << rs1 << ")\n";
//             break;
//         case I_TYPE:
//             std::cout << "Executing I_TYPE instruction.\n";
//             rs1 = getRS1(instruction);
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);   
//             funct3 = getFunct3(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             switch (funct3) {
//                 case 0b000:
//                     std::cout << "addi";
//                     break;
//                 case 0b001:
//                     std::cout << "slli";
//                     break;
//                 case 0b010:
//                     std::cout << "slti";
//                     break;
//                 case 0b011:
//                     std::cout << "sltui";
//                     break;
//                 case 0b100:
//                     std::cout << "xori";
//                     break;
//                 case 0b101:
//                     if (funct7 == 0b0000000) std::cout << "srli";
//                     else std::cout << "srai";
//                     break;
//                 case 0b110:
//                     std::cout << "ori";
//                     break;
//                 case 0b111:
//                     std::cout << "andi";
//                     break;
//             }
//             std::cout << " x" << rd << ", x" << rs1 << ", " << imm << "\n";
//             break;
//         case AUIPC:
//             std::cout << "Executing AUIPC instruction.\n";
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             std::cout << "auipc x" << rd << ", " << imm << "\n";
//             break;
//         case S_TYPE:
//             std::cout << "Executing S_TYPE instruction.\n";
//             rs1 = getRS1(instruction);
//             rs2 = getRS2(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             regWrite = 0;
//             ALUop = 0;
//             ALUsrc = 1;
//             memWrite = 1;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             switch (funct3) {
//                 case 0b000:
//                     std::cout << "sb";
//                     break;
//                 case 0b001:
//                     std::cout << "sh";
//                     break;
//                 case 0b010:
//                     std::cout << "sw";
//                     break;
//             }
//             std::cout << " x" << rs2 << ", " << imm << "(x" << rs1 << ")\n";
//             break;
//         case S_TYPE_FP:
//             std::cout << "Executing S_TYPE_FP instruction.\n";
//             rs1 = getRS1(instruction);
//             rs2 = getRS2(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             regWrite = 0;
//             ALUop = 0;
//             ALUsrc = 1;
//             memWrite = 1;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             if(funct3 != 0b010) break;
//             std::cout << "fsw f" << rs2 << ", " << imm << "(x" << rs1 << ")\n";
//             break;
//         case R_TYPE:
//             std::cout << "Executing R_TYPE instruction.\n";
//             rs1 = getRS1(instruction);
//             rs2 = getRS2(instruction);
//             rd = getRD(instruction);
//             funct3 = getFunct3(instruction);
//             funct7 = getFunct7(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 0;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             switch (funct3) {
//                 case 0b000:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "add";
//                             break;
//                         case 0b0000001:
//                             std::cout << "mul";
//                             break;
//                         case 0b0100000:
//                             std::cout << "sub";
//                             break;
//                     }
//                     break;
//                 case 0b001:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "sll";
//                             break;
//                         case 0b0000001:
//                             std::cout << "mulh";
//                             break;
//                     }
//                     break;
//                 case 0b010:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "slt";
//                             break;
//                         case 0b0000001:
//                             std::cout << "mulhsu";
//                             break;
//                     }
//                     break;
//                 case 0b011:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "sltu";
//                             break;
//                         case 0b0000001:
//                             std::cout << "mulhu";
//                             break;
//                     }
//                     break;
//                 case 0b100:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "xor";
//                             break;
//                         case 0b0000001:
//                             std::cout << "div";
//                             break;
//                     }
//                     break;
//                 case 0b101:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "srl";
//                             break;
//                         case 0b0000001:
//                             std::cout << "divu";
//                             break;
//                         case 0b0100000:
//                             std::cout << "sra";
//                             break;
//                     }
//                     break;
//                 case 0b110:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "or";
//                             break;
//                         case 0b0000001:
//                             std::cout << "rem";
//                             break;
//                     }
//                     break;
//                 case 0b111:
//                     switch (funct7) {
//                         case 0b0000000:
//                             std::cout << "and";
//                             break;
//                         case 0b0000001:
//                             std::cout << "remu";
//                             break;
//                     }
//                     break;
//                 break;
//             }
//             std::cout << " x" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
//             break;
//         case LUI:
//             std::cout << "Executing LUI instruction.\n";
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             std::cout << "lui x" << rd << ", " << imm << "\n";
//             break;
//         case SB_TYPE:
//             std::cout << "Executing SB_TYPE instruction.\n";
//             rs1 = getRS1(instruction);
//             rs2 = getRS2(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             regWrite = 0;
//             ALUop = 1;
//             ALUsrc = 0;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 1;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 0;
//             rm = 0;
//             switch (funct3) {
//                 case 0b000:
//                     std::cout << "beq";
//                     break;
//                 case 0b001:
//                     std::cout << "bne";
//                     break;
//                 case 0b100:
//                     std::cout << "blt";
//                     break;
//                 case 0b101:
//                     std::cout << "bge";
//                     break;
//                 case 0b110:
//                     std::cout << "bltu";
//                     break;
//                 case 0b111:
//                     std::cout << "bgeu";
//                     break;
//             }
//             std::cout << " x" << rs1 << ", x" << rs2 << ", " << imm << "\n";
//             break;
//         case JALR:
//             std::cout << "Executing JALR instruction.\n";
//             rs1 = getRS1(instruction);
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             funct3 = getFunct3(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 1;
//             PCtoReg = 1;
//             RegToPC = 1;
//             std::cout << "jalr x" << rd << ", x" << rs1 << ", " << imm << "\n";
//             break;
//         case JAL:
//             std::cout << "Executing JAL instruction.\n";
//             rd = getRD(instruction);
//             imm = getImmediate(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 1;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 1;
//             PCtoReg = 1;
//             RegToPC = 0;
//             rm = 0;
//             std::cout << "jal x" << rd << ", " << imm << "\n";
//             break;
//         case FP_TYPE:
//             std::cout << "Executing FP_TYPE instruction.\n";
//             rs1 = getRS1(instruction);
//             rs2 = getRS2(instruction);
//             rd = getRD(instruction);
//             funct3 = getFunct3(instruction);
//             funct7 = getFunct7(instruction);
//             regWrite = 1;
//             ALUop = 2;
//             ALUsrc = 0;
//             memWrite = 0;
//             memRead = 0;
//             memToReg = 0;
//             branch = 0;
//             jump = 0;
//             PCtoReg = 0;
//             RegToPC = 1;
//             rm = 0;
//             switch (funct7) {
//                 case 0b0000000:
//                     rm = 1;
//                     std::cout << "fadd.s";
//                     break;
//                 case 0b0000100:
//                     rm = 1;
//                     std::cout << "fsub.s";
//                     break;
//                 case 0b0001000:
//                     rm = 1;
//                     std::cout << "fmul.s";
//                     break;
//                 case 0b0001100:
//                     rm = 1;
//                     std::cout << "fdiv.s";
//                     break;
//                 case 0b0010000:
//                     switch (funct3) {
//                         case 0b000:
//                             std::cout << "fsgnj.s";
//                             break;
//                         case 0b001:
//                             std::cout << "fsgnjn.s";
//                             break;
//                         case 0b010:
//                             std::cout << "fsgnjx.s";
//                             break;
//                     }
//                     break;
//                 case 0b0010100:
//                     switch (funct3) {
//                         case 0b000:
//                             std::cout << "fmin.s";
//                             break;
//                         case 0b001:
//                             std::cout << "fmax.s";
//                             break;
//                     }
//                     break;
//                 case 0b0101100:
//                     rm = 1;
//                     std::cout << "fsqrt.s";
//                     break;
//                 case 0b1010000:
//                     switch (funct3) {
//                         case 0b000:
//                             std::cout << "feq.s";
//                             break;
//                         case 0b001:
//                             std::cout << "flt.s";
//                             break;
//                         case 0b010:
//                             std::cout << "fle.s";
//                             break;
//                     }
//                     break;
//                 case 0b1100000:
//                     switch (rs2) {
//                         case 0b00000:
//                             std::cout << "fcvt.w.s";
//                             break;
//                         case 0b00001:
//                             std::cout << "fcvt.wu.s";
//                             break;
//                     }
//                     rm = 1;
//                     break;
//                 case 0b1101000:
//                     switch (rs2) {
//                         case 0b00000:
//                             std::cout << "fcvt.s.w";
//                             break;
//                         case 0b00001:
//                             std::cout << "fcvt.s.wu";
//                             break;
//                     }
//                     rm = 1;
//                     break;
//                 case 0b1110000:
//                     switch (funct3) {
//                         case 0b00000:
//                             std::cout << "fmv.x.w";
//                             break;
//                         case 0b00001:
//                             std::cout << "fclass.s";
//                             break;
//                     }
//                     break;
//                 case 0b1111000:
//                     std::cout << "fmv.w.x";
//                     break;
//                 break;
//             }
//             std::cout << " rd = " << rd << ", rs1 = " << rs1 << ", rs2 = " << rs2 << "\n";
//             break;
//         default:
//             std::cout << "Unknown instruction.\n";
//             std::cout << "Magic 8-ball says you suck, try again.\n";
//             break;
//     }
//     std::cout << "regWrite = " << regWrite << "\n"
//         << "ALUop = " << ALUop << "\n"
//         << "ALUsrc = " << ALUsrc << "\n"
//         << "memWrite = " << memWrite << "\n"
//         << "memRead = " << memRead << "\n"
//         << "memToReg = " << memToReg << "\n"
//         << "branch = " << branch << "\n"
//         << "jump = " << jump << "\n"
//         << "PCtoReg = " << PCtoReg << "\n"
//         << "RegToPC = " << RegToPC << "\n"; 
// }