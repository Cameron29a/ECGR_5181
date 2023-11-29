#include "CPU.h"

// the register functions are only used outside of the cpu class (i.e. set up x1 for running lab 2)
void CPU::writeIntRegister(uint32_t address, uint32_t data) {
    if (address < 32)
        registers.int_regs[address] = data;
}

void CPU::writeFPRegister(uint32_t address, float data) {
    if (address < 32)
        registers.fp_regs[address] = data;
}

int32_t CPU::readIntRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < 32) {
        return registers.int_regs[regIndex];
    } else {
        std::cerr << "Error: Integer register index out of bounds." << std::endl;
        return 0;
    }
}

float CPU::readFloatRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < 32) {
        return registers.fp_regs[regIndex];
    } else {
        std::cerr << "Error: Float register index out of bounds." << std::endl;
        return 0;
    }
}

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

// Method to make a memory read request
void CPU::Read(uint32_t address) {
    MemoryRequest request{MemoryRequestType::READ, address, 0};
    memBus.addMemoryRequest(cpuId, request);
}

// Method to make a memory write request
void CPU::Write(uint32_t address, uint32_t data) {
    MemoryRequest request{MemoryRequestType::WRITE, address, data};
    memBus.addMemoryRequest(cpuId, request);
}

// // Read float from memory at the specified address
// void CPU::ReadFloat(uint32_t address) {
//     uint32_t floatData = Read(address);
//     float result;
//     std::memcpy(&result, &floatData, sizeof(float));
// }

// 

// // Method to make a memory read request
// uint32_t CPU::Read(uint32_t address) {
//     MemoryRequest request{MemoryRequestType::READ, address};
//     memoryBus.addMemoryRequest(cpuId, request);
//     // For now, return a placeholder value (you might want to change this based on your logic)
//     return 0;
// }

// // Method to make a memory write request
// void CPU::Write(uint32_t address, uint32_t data) {
//     MemoryRequest request{MemoryRequestType::WRITE, address, data};
//     memoryBus.addMemoryRequest(cpuId, request);
// }

// // Read float from memory at the specified address
// float CPU::ReadFloat(uint32_t address) {
//     uint32_t intData = Read(address);
//     float result;
//     std::memcpy(&result, &intData, sizeof(float));
//     return result;
// }

// // Write float to memory at the specified address
// void CPU::WriteFloat(uint32_t address, float data) {
//     uint32_t intData;
//     std::memcpy(&intData, &data, sizeof(uint32_t));
//     Write(address, intData);
// }

std::variant<int32_t, float> CPU::doTheThing(uint32_t operation, int32_t rs1, int32_t rs2, float rs1_fp, float rs2_fp) {
        int32_t signedRS1;
        int64_t product;

        int32_t intResult = 0;
        float floatResult = 0;

        switch (operation) {
            case ADD:
                // Perform addition
                intResult = rs1 + rs2;
                break;
            case SUB:
                // Perform subtraction
                intResult = rs1 - rs2;
                break;
            case AND:
                // Perform bitwise AND
                intResult = rs1 & rs2;
                break;
            case OR:
                // Perform bitwise OR
                intResult = rs1 | rs2;
                break;
            case XOR:
                // Perform bitwise XOR
                intResult = rs1 ^ rs2;
                break;
            case SLL:
                // Perform logical shift left
                intResult = rs1 << rs2;
                break;
            case SRL:
                // Perform logical shift right
                intResult = rs1 >> rs2;
                break;
            case SRA:
                // Perform arithmetic shift right
                signedRS1 = static_cast<int32_t>(rs1);
                intResult = static_cast<uint32_t>(signedRS1 >> rs2);
                break;
            case SLT:
                // Set Less Than
                intResult = (rs1 < rs2) ? 1 : 0;
                break;
            case SLTU:
                // Set Less Than Unsigned
                intResult = (static_cast<uint32_t>(rs1) < static_cast<uint32_t>(rs2)) ? 1 : 0;
                break;
            case MUL:
                // Multiplication
                product = static_cast<uint64_t>(rs1) * static_cast<uint64_t>(rs2);
                intResult = static_cast<uint32_t>(product);
                break;
            case MULH:
                // Multiply Upper Half (Unsigned)
                intResult = static_cast<uint64_t>(rs1) * static_cast<uint64_t>(rs2) >> 32;
                break;
            case MULHSU:
                // Multiply Upper Half (Signed-Unsigned)
                intResult = (static_cast<int64_t>(rs1) * static_cast<int64_t>(rs2)) >> 32;
                break;
            case MULHU:
                // Multiply Upper Half (Unsigned)
                intResult = static_cast<uint64_t>(static_cast<uint32_t>(rs1) * static_cast<uint32_t>(rs2)) >> 32;
                break;
            case DIV:
                // Division
                if (rs2 == 0) intResult = 0;
                else intResult = rs1 / rs2;
                break;
            case DIVU:
                // Divide (Unsigned)
                if (rs2 == 0) intResult = 0;
                else intResult = static_cast<uint32_t>(rs1) / static_cast<uint32_t>(rs2);
                break;
            case REM:
                // Remainder
                if (rs2 == 0) intResult = 0;
                else intResult = rs1 % rs2;
                break;
            case REMU:
                // Remainder (Unsigned)
                if (rs2 == 0) intResult = 0;
                else intResult = static_cast<uint32_t>(rs1) % static_cast<uint32_t>(rs2);
                break;

            // Floating-point operations
            case ADD_FP:
                // Floating-point addition
                floatResult = rs1_fp + rs2_fp;
                break;
            case SUB_FP:
                // Floating-point subtraction
                floatResult = rs1_fp - rs2_fp;
                break;
            case MUL_FP:
                // Floating-point multiplication
                floatResult = rs1_fp * rs2_fp;
                break;
            case DIV_FP:
                // Floating-point division
                if (rs2_fp == 0.0f) floatResult = 0;
                else floatResult = rs1_fp / rs2_fp;
                break;
        }
        return (operation >= ADD_FP) ? std::variant<int32_t, float>(floatResult) : std::variant<int32_t, float>(intResult);
    }