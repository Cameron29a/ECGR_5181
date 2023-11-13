#ifndef ALU_H
#define ALU_H

#include <variant>

class ALU {

public:
    std::variant<int32_t, float> doTheThing(uint32_t operation, int32_t rs1, int32_t rs2, float rs1_fp, float rs2_fp) {
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
};

#endif //ALU_H