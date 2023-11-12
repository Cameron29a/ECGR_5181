#ifndef ALU_H
#define ALU_H

#include "instruction.cpp"

// #include <cstdint>
// #include <cstring>

class ALU {

public:

    int32_t doTheThing(Instruction& instruction, uint32_t operation, int32_t rs1, int32_t rs2, float rs1_fp, float rs2_fp) {
        int32_t signedRS1;
        int64_t result;

        float floatResult;

        switch (operation) {
            case 0b0000:
                // Perform addition
                return rs1 + rs2;
            case 0b0001:
                // Perform subtraction
                return rs1 - rs2;
            case 0b0010:
                // Perform bitwise AND
                return rs1 & rs2;
            case 0b0011:
                // Perform bitwise OR
                return rs1 | rs2;
            case 0b0100:
                // Perform bitwise XOR
                return rs1 ^ rs2;
            case 0b0101:
                // Perform logical shift left
                return rs1 << rs2;
            case 0b0110:
                // Perform logical shift right
                return rs1 >> rs2;
            case 0b0111:
                // Perform arithmetic shift right
                signedRS1 = static_cast<int32_t>(rs1);
                return static_cast<uint32_t>(signedRS1 >> rs2);
            case 0b1000:
                // Multiplication
                result = static_cast<uint64_t>(rs1) * static_cast<uint64_t>(rs2);
                return static_cast<uint32_t>(result);
            case 0b1001:
                // Division
                if (rs2 == 0) {
                    return 0;
                }
                return rs1 / rs2;
            // Floating-point operations
            case 0b1010:
                // Floating-point addition
                floatResult = rs1_fp + rs2_fp;
                return *reinterpret_cast<uint32_t*>(&floatResult);

            case 0b1011:
                // Floating-point subtraction
                floatResult = rs1_fp - rs2_fp;
                return *reinterpret_cast<uint32_t*>(&floatResult);

            case 0b1100:
                // Floating-point multiplication
                floatResult = rs1_fp * rs2_fp;
                return *reinterpret_cast<uint32_t*>(&floatResult);

            case 0b1101:
                // Floating-point division
                if (rs2_fp == 0.0f) {
                    return 0;
                }
                floatResult = rs1_fp / rs2_fp;
                return *reinterpret_cast<uint32_t*>(&floatResult);
            default:
                return 0;
        }
        // Convert the floating-point result to uint32_t
        // uint32_t uintResult;
        // std::memcpy(&uintResult, &floatResult, sizeof(uint32_t));
        // return uintResult;
    }

};

#endif //ALU_H