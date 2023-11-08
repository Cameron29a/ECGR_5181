#ifndef BRANCHCTL_H
#define BRANCHCTL_H

#include <stdint.h>

class BranchCtl {
public:
    bool shouldTakeBranch(uint32_t funct3, int32_t rs1Value, int32_t rs2Value) {
        // Implement branch control logic
        bool takeBranch = false;
        switch (funct3) {
            case 0x63: // BEQ
                takeBranch = (rs1Value == rs2Value);
                break;

            case 0x64: // BNE
                takeBranch = (rs1Value != rs2Value);
                break;

            case 0x69: // BLT
                takeBranch = (rs1Value < rs2Value);
                break;

            case 0x67: // BGE
                takeBranch = (rs1Value >= rs2Value);
                break;

            case 0x65: // BLTU
                takeBranch = ((uint32_t)rs1Value < (uint32_t)rs2Value);
                break;
            
            case 0x66: // BGEU
                takeBranch = ((uint32_t)rs1Value >= (uint32_t)rs2Value);
                break;

            default:
                takeBranch = false; // Default behavior
                break;
        }

        return takeBranch;
    }
};

#endif //BRANCHCTL_H