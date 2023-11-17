#include "CPU.h"

// the register functions are only used outside of the cpu class (i.e. set up x1 for running lab 2)
void CPU::writeIntRegister(uint32_t address, uint32_t data) {
    if (address >= 0 && address < 32)
        registers.int_regs[address] = data;
}

void CPU::writeFPRegister(uint32_t address, float data) {
    if (address >= 0 && address < 32)
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
    MemoryRequest request{MemoryRequestType::READ, address};
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