#ifndef BRANCHCTL_H
#define BRANCHCTL_H

#include <stdint.h>

class BranchCtl {
public:
    bool shouldTakeBranch(uint32_t funct3, int32_t rs1Value, int32_t rs2Value) {
        // Implement branch control logic
        bool takeBranch = false;
        switch (funct3) {
            case 0b000: // BEQ
                takeBranch = (rs1Value == rs2Value);
                break;

            case 0b001: // BNE
                takeBranch = (rs1Value != rs2Value);
                break;

            case 0b100: // BLT
                takeBranch = (rs1Value < rs2Value);
                break;

            case 0b101: // BGE
                takeBranch = (rs1Value >= rs2Value);
                break;

            case 0b110: // BLTU
                takeBranch = ((uint32_t)rs1Value < (uint32_t)rs2Value);
                break;
            
            case 0b111: // BGEU
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