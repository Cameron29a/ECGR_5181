#include <iostream>
#include <cstdint>

// Define RISC-V opcodes
#define OPCODE_LOAD         0b0000011
#define OPCODE_LOAD_FP      0b0000111
#define OPCODE_I_TYPE       0b0010011
#define OPCODE_AUIPC        0b0010111
#define OPCODE_S_TYPE       0b0100011
#define OPCODE_S_TYPE_FP    0b0100111
#define OPCODE_R_TYPE       0b0110011
#define OPCODE_LUI          0b0110111
#define OPCODE_SB_TYPE      0b1100011
#define OPCODE_JALR         0b1100111
#define OPCODE_JAL          0b1101111

uint32_t getRS1(uint32_t instruction) {
    uint32_t rs1 = (instruction >> 15) & 0x1F;  // Mask the lower 5 bits (0x1F)
    return rs1;
}

uint32_t getRS2(uint32_t instruction) {
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    return rs2;
}

uint32_t getRD(uint32_t instruction) {
    uint32_t rd = (instruction >> 7) & 0x1F;
    return rd;
}

int32_t getImmediate(uint32_t instruction) {
    // uint32_t opcode = instruction & 0x7F;
    switch (instruction & 0x7F) {
        case OPCODE_LOAD:
        case OPCODE_LOAD_FP:
        case OPCODE_I_TYPE:
        case OPCODE_JALR:
            return static_cast<int32_t>(instruction) >> 20; // Sign-extend the immediate field
        case OPCODE_S_TYPE:
        case OPCODE_S_TYPE_FP:
            return ((static_cast<int32_t>(instruction) >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F); // Sign-extend and combine imm[11:5] and imm[4:0]
        case OPCODE_SB_TYPE:
            return (static_cast<int32_t>(instruction) >> 19) | ((instruction >> 7) & 0x1E) | ((instruction << 4) & 0x800); // Sign-extend and combine bits
        case OPCODE_AUIPC:
        case OPCODE_LUI:
            return instruction & 0xFFFFF000; // Zero-extend the immediate field
        case OPCODE_JAL:
            return ((static_cast<int32_t>(instruction) >> 11) & 0xFFE) | ((instruction >> 20) & 0x7FE) | ((instruction >> 9) & 0x1000) | (instruction & 0xFF000); // Sign-extend and combine bits
        default:
            return 0;
    }
}

uint32_t getFunct3(uint32_t instruction) {
    uint32_t funct3 = (instruction >> 12) & 0x7;
    return funct3;
}

uint32_t getFunct7(uint32_t instruction) {
    uint32_t funct7 = (instruction >> 25) & 0x7F;
    return funct7;
}

// Function to decode RISC-V instructions
// Then extract the fields from the instruction
void decodeInstruction(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    
    // Instruction Variables
    uint32_t rs1, rs2, rd, funct3, funct7;
    int32_t imm;
    
    // Control Signals
    bool regWrite, ALUsrc, memWrite, memRead, memToReg, branch, jump, PCtoReg, RegToPC = 0;
    int ALUop;
    
    // Decode instruction based on the opcode
    switch (opcode) {
        case OPCODE_LOAD:
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
        case OPCODE_LOAD_FP:
            std::cout << "Executing LOAD_FP instruction.\n";
            rs1 = getRS1(instruction);
            rd = getRD(instruction);
            imm = getImmediate(instruction);
            funct3 = getFunct3(instruction);
            std::cout << "f3 = " << funct3 << "\n";
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
            if(funct3 != 0b010) break;
            std::cout << "flw f" << rd << ", " << imm << "(x" << rs1 << ")\n";
            break;
        case OPCODE_I_TYPE:
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
        case OPCODE_AUIPC:
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
            std::cout << "auipc x" << rd << ", " << imm << "\n";
            break;
        case OPCODE_S_TYPE:
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
        case OPCODE_S_TYPE_FP:
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
            if(funct3 != 0b010) break;
            std::cout << "fsw f" << rs2 << ", " << imm << "(x" << rs1 << ")\n";
            break;
        case OPCODE_R_TYPE:
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
        case OPCODE_LUI:
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
            std::cout << "lui x" << rd << ", " << imm << "\n";
            break;
        case OPCODE_SB_TYPE:
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
        case OPCODE_JALR:
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
        case OPCODE_JAL:
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
            std::cout << "jal x" << rd << ", " << imm << "\n";
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
        << "RegToPC = " << RegToPC << "\n"; 
}

int main() {
    
    // Sample RISC-V instruction
    uint32_t inst;

    // uncomment to run desired instruction
    // inst = 0b00000000010100100000000110110011;      // add x3, x4, x5
    // inst = 0b00000000011100110000001010110011;      // add x5, x6, x7
    inst = 0b00000000000000000001000010110111;      // lui x1, 4096
    // inst = 0b11111100111000001000011110010011;      // addi x15, x1, -50
    // inst = 0b00000000100000010010011110000011;      // lw x15, 8(x2)
    // inst = 0b00000000111000010010010000100011;      // sw x14, 8(x2)
    // inst = 0b0000000000100011000000001100011;       // beq x3, x1, 2

    // Decode the instruction
    decodeInstruction(inst);

    return 0;
}
