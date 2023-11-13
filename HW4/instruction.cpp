#include "instruction.h"
#include <bitset>

int32_t Instruction::setImmediate() {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case JALR:
            return static_cast<int32_t>(instruction) >> 20; // Sign-extend the immediate field
        case S_TYPE:
        case S_TYPE_FP:
            return ((static_cast<int32_t>(instruction) >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F); // Sign-extend and combine imm[11:5] and imm[4:0]
        case SB_TYPE:
            return static_cast<int32_t>((((instruction >> 7) & 0x1E) | (((instruction >> 25) & 0x3F) << 5) | (((instruction >> 7) & 0x1) << 11) | ((instruction >> 31) << 12)) << 19) >> 19; // Sign-extend and combine bits
        case AUIPC:
        case LUI:
            return instruction & 0xFFFFF000; // Zero-extend the immediate field
        case JAL:
            return ((static_cast<int32_t>(instruction) >> 11) & 0xFFE) | ((instruction >> 20) & 0x7FE) | ((instruction >> 9) & 0x1000) | (instruction & 0xFF000); // Sign-extend and combine bits
        default:
            return 0;
    }
}

void Instruction::setregWrite() {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case R_TYPE:
        case LUI:
        case JALR:
        case JAL:
        case FP_TYPE:
            regWrite = 1;
            break;
        case S_TYPE:
        case S_TYPE_FP:
        case SB_TYPE:
        default:
            regWrite = 0;
    }
}

void Instruction::setALUsrc1() {
    switch (opcode) {
        case AUIPC:
        case SB_TYPE:
        case JAL:
            ALUsrc1 = 1;
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case JALR:
        case FP_TYPE:
        default:
            ALUsrc1 = 0;
    }
}

void Instruction::setALUsrc2() {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case SB_TYPE:
        case LUI:
        case JALR:
        case JAL:
            ALUsrc2 = 1;
            break;
        case R_TYPE:
        case FP_TYPE:
        default:
            ALUsrc2 = 0;
    }
}

void Instruction::setmemWrite() {
    switch (opcode) {
        case S_TYPE:
        case S_TYPE_FP:
            memWrite = 1;
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case R_TYPE:
        case LUI:
        case SB_TYPE:
        case JALR:
        case JAL:
        case FP_TYPE:
        default:
            memWrite = 0;
    }
}

void Instruction::setmemRead() {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
            memRead = 1;
            break;
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case SB_TYPE:
        case JALR:
        case JAL:
        case FP_TYPE:
        default:
            memRead = 0;
    }
}

void Instruction::setBranch() {
    switch (opcode) {
        case SB_TYPE:
            branch = 1;
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case JALR:
        case JAL:
        case FP_TYPE:
        default:
            branch = 0;
    }
}

void Instruction::setJump() {
    switch (opcode) {
        case JALR:
        case JAL:
            jump = 1;
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case SB_TYPE:
        case FP_TYPE:
        default:
            jump = 0;
    }
}

void Instruction::setRM() {
    switch (opcode) {
        case FP_TYPE:
            switch (funct7) {
                case 0b0000000:
                case 0b0000100:
                case 0b0001000:
                case 0b0001100:
                case 0b0101100:
                case 0b1100000:
                case 0b1101000:
                    rm = 1;
                    break;
                default:
                    rm = 0;
            }
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case SB_TYPE:
        case JALR:
        case JAL:
        default:
            rm = 0;
    }
}

void Instruction::setPCsel(bool takeBranch) {
    switch (opcode) {
        case SB_TYPE:
            switch (takeBranch) {
                case true:
                    PCsel = 1;
                    break;
                default:
                    PCsel = 0;
            }
            break;
        case JALR:
        case JAL:
            PCsel = 1;
            break;
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case R_TYPE:
        case LUI:
        case FP_TYPE:
        default:
            PCsel = 0;
    }
}

void Instruction::setALUop() {
    switch (opcode) {
        case I_TYPE:
        case R_TYPE:
            ALUop = ADD;
            break;
        case FP_TYPE:
            ALUop = ADD_FP;
            break;
        case LOAD:
        case LOAD_FP:
        case S_TYPE:
        case S_TYPE_FP:
        case SB_TYPE:
        case JALR:
        case JAL:
        case AUIPC:
        case LUI:
            ALUop = ADD;
            break;
        default:
            ALUop = ADD;
    }
}

void Instruction::setWBsel() {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
            WBsel = 0;
            break;
        case I_TYPE:
        case AUIPC:
        case R_TYPE:
        case LUI:
        case FP_TYPE:
            WBsel = 1;
            break;
        case JALR:
        case JAL:
            WBsel = 2;
            break;
        case S_TYPE:
        case S_TYPE_FP:
        case SB_TYPE:
        default:
            WBsel = 3;
    }
}

void Instruction::printSignals() {
    std::cout << "***************Fetched Instruction***************\n";
    std::cout << "Instruction: " << std::bitset<32>(instruction) << "\n";
    std::cout << "Decoded Instruction:\n";
    std::cout << "Opcode: " << opcode << "\n";
    std::cout << "RS1: " << rs1 << "\n";
    std::cout << "RS2: " << rs2 << "\n";
    std::cout << "RD: " << rd << "\n";
    std::cout << "Funct3: " << funct3 << "\n";
    std::cout << "Funct7: " << funct7 << "\n";
    std::cout << "Immediate: " << std::dec << imm <<"\n";

    // Control Signals
    std::cout << "Control Signals:\n";
    std::cout << "regWrite = " << regWrite << "\n";
    std::cout << "ALUop = " << ALUop << "\n";
    std::cout << "ALUsrc1 = " << ALUsrc1 << "\n";
    std::cout << "ALUsrc2 = " << ALUsrc2 << "\n";
    std::cout << "memWrite = " << memWrite << "\n";
    std::cout << "memRead = " << memRead << "\n";
    std::cout << "WBsel = " << WBsel << "\n";
    std::cout << "branch = " << branch << "\n";
    std::cout << "jump = " << jump << "\n";
    std::cout << "rm = " << rm << "\n";
}

void Instruction::printInstruction() {
    std::cout << "Instruction: 0b" << std::bitset<32>(instruction) << "\n";
}

void Instruction::printAssembly() {
    std::cout << "Assembly Instruction: " << assemblyString << "\n";
}

void Instruction::assembleString() {
    std::stringstream instruction;
    switch (opcode) {
        case LOAD:
            switch (funct3) {
                case 0b000:
                    instruction << "lb";
                    break;
                case 0b001:
                    instruction << "lh";
                    break;
                case 0b010:
                    instruction << "lw";
                    break;
                case 0b100:
                    instruction << "lbu";
                    break;
                case 0b101:
                    instruction << "lhu";
                    break;
            }
            instruction << " x" << rd << ", " << std::dec << imm << "(x" << rs1 << ")";
            break;
        case LOAD_FP:
            if(funct3 != 0b010) break;
            instruction << "flw f" << rd << ", " << std::dec << imm << "(x" << rs1 << ")";
            break;
        case I_TYPE:
            switch (funct3) {
                case 0b000:
                    instruction << "addi";
                    break;
                case 0b001:
                    instruction << "slli";
                    break;
                case 0b010:
                    instruction << "slti";
                    break;
                case 0b011:
                    instruction << "sltui";
                    break;
                case 0b100:
                    instruction << "xori";
                    break;
                case 0b101:
                    if (funct7 == 0b0000000) {
                        instruction << "srli";
                    } else {
                        instruction << "srai";
                    }
                    break;
                case 0b110:
                    instruction << "ori";
                    break;
                case 0b111:
                    instruction << "andi";
                    break;
            }
            instruction << " x" << rd << ", x" << rs1 << ", " << std::dec << imm;
            break;
        case AUIPC:
            instruction << "auipc x" << rd << ", " << std::dec << imm;
            break;
        case S_TYPE:
            switch (funct3) {
                case 0b000:
                    instruction << "sb";
                    break;
                case 0b001:
                    instruction << "sh";
                    break;
                case 0b010:
                    instruction << "sw";
                    break;
            }
            instruction << " x" << rs1 << ", " << std::dec << imm << "(x" << rs2 << ")";
            break;
        case S_TYPE_FP:
            if(funct3 != 0b100) break;
            instruction << "fsw f" << rs1 << ", " << std::dec << imm << "(x" << rs2 << ")";
            break;
        case R_TYPE:
            switch (funct3) {
                case 0b000:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "add";
                            break;
                        case 0b0000001:
                            instruction << "mul";
                            break;
                        case 0b0100000:
                            instruction << "sub";
                            break;
                    }
                    break;
                case 0b001:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "sll";
                            break;
                        case 0b0000001:
                            instruction << "mulh";
                            break;
                    }
                    break;
                case 0b010:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "slt";
                            break;
                        case 0b0000001:
                            instruction << "mulhsu";
                            break;
                    }
                    break;
                case 0b011:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "sltu";
                            break;
                        case 0b0000001:
                            instruction << "mulhu";
                            break;
                    }
                    break;
                case 0b100:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "xor";
                            break;
                        case 0b0000001:
                            instruction << "div";
                            break;
                    }
                    break;
                case 0b101:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "srl";
                            break;
                        case 0b0000001:
                            instruction << "divu";
                            break;
                        case 0b0100000:
                            instruction << "sra";
                            break;
                    }
                    break;
                case 0b110:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "or";
                            break;
                        case 0b0000001:
                            instruction << "rem";
                            break;
                    }
                    break;
                case 0b111:
                    switch (funct7) {
                        case 0b0000000:
                            instruction << "and";
                            break;
                        case 0b0000001:
                            instruction << "remu";
                            break;
                    }
                    break;
                break;
            }
            instruction << " x" << rd << ", x" << rs1 << ", x" << rs2;
            break;
        case LUI:
            instruction << "lui x" << rd << ", " << std::dec << imm;
            break;
        case SB_TYPE:
            switch (funct3) {
                case 0b000:
                    instruction << "beq";
                    break;
                case 0b001:
                    instruction << "bne";
                    break;
                case 0b100:
                    instruction << "blt";
                    break;
                case 0b101:
                    instruction << "bge";
                    break;
                case 0b110:
                    instruction << "bltu";
                    break;
                case 0b111:
                    instruction << "bgeu";
                    break;
            }
            instruction << " x" << rs1 << ", x" << rs2 << ", " << std::dec << imm;
            break;
        case JALR:
            instruction << "jalr x" << rd << ", x" << rs1 << ", " << std::dec << imm;
            break;
        case JAL:
            instruction << "jal x" << rd << ", " << std::dec << imm;
            break;
        case FP_TYPE:
            switch (funct7) {
                case 0b0000000:
                    instruction << "fadd.s";
                    break;
                case 0b0000100:
                    instruction << "fsub.s";
                    break;
                case 0b0001000:
                    instruction << "fmul.s";
                    break;
                case 0b0001100:
                    instruction << "fdiv.s";
                    break;
                case 0b0010000:
                    switch (funct3) {
                        case 0b000:
                            instruction << "fsgnj.s";
                            break;
                        case 0b001:
                            instruction << "fsgnjn.s";
                            break;
                        case 0b010:
                            instruction << "fsgnjx.s";
                            break;
                    }
                    break;
                case 0b0010100:
                    switch (funct3) {
                        case 0b000:
                            instruction << "fmin.s";
                            break;
                        case 0b001:
                            instruction << "fmax.s";
                            break;
                    }
                    break;
                case 0b0101100:
                    instruction << "fsqrt.s";
                    break;
                case 0b1010000:
                    switch (funct3) {
                        case 0b000:
                            instruction << "feq.s";
                            break;
                        case 0b001:
                            instruction << "flt.s";
                            break;
                        case 0b010:
                            instruction << "fle.s";
                            break;
                    }
                    break;
                case 0b1100000:
                    switch (rs2) {
                        case 0b00000:
                            instruction << "fcvt.w.s";
                            break;
                        case 0b00001:
                            instruction << "fcvt.wu.s";
                            break;
                    }
                    break;
                case 0b1101000:
                    switch (rs2) {
                        case 0b00000:
                            instruction << "fcvt.s.w";
                            break;
                        case 0b00001:
                            instruction << "fcvt.s.wu";
                            break;
                    }
                    break;
                case 0b1110000:
                    switch (funct3) {
                        case 0b00000:
                            instruction << "fmv.x.w";
                            break;
                        case 0b00001:
                            instruction << "fclass.s";
                            break;
                    }
                    break;
                case 0b1111000:
                    instruction << "fmv.w.x";
                    break;
                break;
            }
            switch(operation){
                case fsqrt_s:
                case fcvt_w_s:
                case fcvt_wu_s:
                case fcvt_s_w:
                case fcvt_s_wu:
                case fmv_x_w:
                case fclass_s:
                case fmv_w_x:
                    instruction << " f" << rd << ", f" << rs1;
                    break;
                default:
                    instruction << " f" << rd << ", f" << rs1 << ", f" << rs2;
                    break;
            }
            break;
        default:
            instruction << "Invalid Op.";
    }
    assemblyString = instruction.str();
}