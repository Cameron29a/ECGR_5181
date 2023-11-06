#include "instruction.h"
#include <bitset>

int32_t Instruction::getImmediate(uint32_t opcode) {
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
            return static_cast<int32_t>(((instruction >> 7) & 0x1E) | (((instruction >> 25) & 0x3F) << 5) | (((instruction >> 7) & 0x1) << 11) | ((instruction >> 31) << 12)); // Sign-extend and combine bits
        case AUIPC:
        case LUI:
            return instruction & 0xFFFFF000; // Zero-extend the immediate field
        case JAL:
            return ((static_cast<int32_t>(instruction) >> 11) & 0xFFE) | ((instruction >> 20) & 0x7FE) | ((instruction >> 9) & 0x1000) | (instruction & 0xFF000); // Sign-extend and combine bits
        default:
            return 0;
    }
}

void Instruction::setregWrite(uint32_t opcode) {
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
            regWrite = 0;
            break;
        default:
            regWrite = 0;
    }
}

void Instruction::setALUsrc(uint32_t opcode) {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
        case I_TYPE:
        case AUIPC:
        case S_TYPE:
        case S_TYPE_FP:
        case LUI:
        case JALR:
        case JAL:
        case FP_TYPE:
            ALUsrc = 1;
            break;
        case R_TYPE:
        case SB_TYPE:
            ALUsrc = 0;
            break;
        default:
            ALUsrc = 0;
    }
}

void Instruction::setmemWrite(uint32_t opcode) {
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
            memWrite = 0;
            break;
        default:
            memWrite = 0;
    }
}

void Instruction::setmemRead(uint32_t opcode) {
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
            memRead = 0;
            break;
        default:
            memRead = 0;
    }
}

void Instruction::setmemToReg(uint32_t opcode) {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
            memToReg = 1;
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
            memToReg = 0;
            break;
        default:
            memToReg = 0;
    }
}

void Instruction::setBranch(uint32_t opcode) {
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
            branch = 0;
            break;
        default:
            branch = 0;
    }
}

void Instruction::setJump(uint32_t opcode) {
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
            jump = 0;
            break;
        default:
            jump = 0;
    }
}

void Instruction::setPCtoReg(uint32_t opcode) {
    switch (opcode) {
        case JALR:
        case JAL:
            PCtoReg = 1;
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
            PCtoReg = 0;
            break;
        default:
            PCtoReg = 0;
    }
}

void Instruction::setRegtoPC(uint32_t opcode) {
    switch (opcode) {
        case JALR:
            RegToPC = 1;
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
        case JAL:
        case FP_TYPE:
            RegToPC = 0;
            break;
        default:
            RegToPC = 0;
    }
}

void Instruction::setRM(uint32_t opcode, uint32_t funct7) {
    switch (opcode) {
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
        case FP_TYPE:
            rm = 1;
            break;
        default:
            return;
    }
}

void Instruction::setALUop(uint32_t opcode) {
    switch (opcode) {
        case LOAD:
        case LOAD_FP:
        case S_TYPE:
        case S_TYPE_FP:
            ALUop = 0;
            break;
        case SB_TYPE:
            ALUop = 1;
            break;
        case I_TYPE:
        case AUIPC:
        case R_TYPE:
        case LUI:
        case JALR:
        case JAL:
        case FP_TYPE:
            ALUop = 3;
            break;
        default:
            ALUop = 0;
    }
}

void Instruction::printInstruction() {
    std::cout << "***************Fetched Instruction***************\n";
    std::cout << "Instruction: " << std::bitset<32>(instruction) << "\n";
    std::cout << "Decoded Instruction:\n";
    std::cout << "Opcode: " << opcode << "\n";
    std::cout << "RS1: " << rs1 << "\n";
    std::cout << "RS2: " << rs2 << "\n";
    std::cout << "RD: " << rd << "\n";
    std::cout << "Funct3: " << funct3 << "\n";
    std::cout << "Funct7: " << funct7 << "\n";
    std::cout << "Immediate: " << std::dec << "\n";

    // Control Signals
    std::cout << "Control Signals:\n";
    std::cout << "regWrite = " << regWrite << "\n";
    std::cout << "ALUop = " << ALUop << "\n";
    std::cout << "ALUsrc = " << ALUsrc << "\n";
    std::cout << "memWrite = " << memWrite << "\n";
    std::cout << "memRead = " << memRead << "\n";
    std::cout << "memToReg = " << memToReg << "\n";
    std::cout << "branch = " << branch << "\n";
    std::cout << "jump = " << jump << "\n";
    std::cout << "PCtoReg = " << PCtoReg << "\n";
    std::cout << "RegToPC = " << RegToPC << "\n";
    std::cout << "rm = " << rm << "\n";
}

void Instruction::printAssembly() {
    std::cout << "***************Fetched Assembly Instruction***************\n";
    switch (opcode) {
        case LOAD:
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
            std::cout << " x" << rd << ", " << std::dec << imm << "(x" << rs1 << ")\n";
            break;
        case LOAD_FP:
            if(funct3 != 0b010) break;
            std::cout << "flw f" << rd << ", " << std::dec << imm << "(x" << rs1 << ")\n";
            break;
        case I_TYPE:
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
                    if (funct7 == 0b0000000) {
                        std::cout << "srli";
                    } else {
                        std::cout << "srai";
                    }
                    break;
                case 0b110:
                    std::cout << "ori";
                    break;
                case 0b111:
                    std::cout << "andi";
                    break;
            }
            std::cout << " x" << rd << ", x" << rs1 << ", " << std::dec << imm << "\n";
            break;
        case AUIPC:
            std::cout << "auipc x" << rd << ", " << std::dec << imm << "\n";
            break;
        case S_TYPE:
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
            std::cout << " x" << rs1 << ", " << std::dec << imm << "(x" << rs2 << ")\n";
            break;
        case S_TYPE_FP:
            if(funct3 != 0b010) break;
            std::cout << "fsw f" << rs1 << ", " << std::dec << imm << "(x" << rs2 << ")\n";
            break;
        case R_TYPE:
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
            std::cout << "lui x" << rd << ", " << std::dec << imm << "\n";
            break;
        case SB_TYPE:
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
            std::cout << " x" << rs1 << ", x" << rs2 << ", " << std::dec << imm << "\n";
            break;
        case JALR:
            std::cout << "jalr x" << rd << ", x" << rs1 << ", " << std::dec << imm << "\n";
            break;
        case JAL:
            std::cout << "jal x" << rd << ", " << std::dec << imm << "\n";
            break;
        case FP_TYPE:
            switch (funct7) {
                case 0b0000000:
                    std::cout << "fadd.s";
                    break;
                case 0b0000100:
                    std::cout << "fsub.s";
                    break;
                case 0b0001000:
                    std::cout << "fmul.s";
                    break;
                case 0b0001100:
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
            switch(operation){
                case fsqrt_s:
                case fcvt_w_s:
                case fcvt_wu_s:
                case fcvt_s_w:
                case fcvt_s_wu:
                case fmv_x_w:
                case fclass_s:
                case fmv_w_x:
                    std::cout << " f" << rd << ", f" << rs1 << "\n";
                    break;
                default:
                    std::cout << " f" << rd << ", f" << rs1 << ", f" << rs2 << "\n";
                    break;
            }
            break;
        default:
            std::cout << "Invalid instruction.\n";
            break;
    }
}
