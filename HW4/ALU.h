#ifndef ALU_H
#define ALU_H

class ALU {

public:

    uint32_t doTheThing(uint32_t operation, uint32_t rs1, uint32_t rs2) {
        int32_t signedRS1;
        uint64_t result;
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
            default:
                return 0;
        }
    }

};

#endif //ALU_H