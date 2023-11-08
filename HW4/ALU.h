#ifndef ALU_H
#define ALU_H

class ALU {

    
public:

    uint32_t doTheThing(uint32_t operation, uint32_t rs1, uint32_t rs2) {
        switch (operation) {
            case 1:
                // Perform addition
                return rs1 + rs2;
            case 2:
                // Perform subtraction
                return rs1 - rs2;
            case 3:
                // Perform bitwise AND
                return rs1 & rs2;
            case 4:
                // Perform bitwise OR
                return rs1 | rs2;
            case 5:
                // Perform bitwise XOR
                return rs1 ^ rs2;
            case 6:
                // Perform logical shift left
                return rs1 << rs2;
            case 7:
                // Perform logical shift right
                return rs1 >> rs2;
            case 8:
                // Perform arithmetic shift right
                int32_t signedRS1 = static_cast<int32_t>(rs1);
                return static_cast<uint32_t>(signedRS1 >> rs2);
            case 9:
                // Multiplication
                uint64_t result = static_cast<uint64_t>(rs1) * static_cast<uint64_t>(rs2);
                return static_cast<uint32_t>(result);
            case 10:
                // Division
                if (rs2 == 0) {
                    return 0;
                }
                return rs1 / rs2;
            default:

        }
    }

};

#endif //ALU_H