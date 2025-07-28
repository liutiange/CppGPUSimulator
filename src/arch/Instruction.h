// src/arch/Instruction.h
#pragma once

#include <string>
#include <vector>
#include <stdexcept>

enum class OpCode {
    ADD,
    MUL,
    LOAD,
    STORE,
    HALT
};

struct Instruction {
    OpCode opcode;
    int dest_reg;
    int src1_reg;
    int src2_reg;
    int mem_address;

    Instruction(OpCode op, int dr = -1, int s1r = -1, int s2r = -1, int mem_addr = -1)
        : opcode(op), dest_reg(dr), src1_reg(s1r), src2_reg(s2r), mem_address(mem_addr) {}

    std::string opcodeToString() const {
        switch (opcode) {
            case OpCode::ADD:   return "ADD";
            case OpCode::MUL:   return "MUL";
            case OpCode::LOAD:  return "LOAD";
            case OpCode::STORE: return "STORE";
            case OpCode::HALT:  return "HALT";
            default:            return "UNKNOWN_OPCODE";
        }
    }

    std::string toString() const {
        std::string s = opcodeToString();
        switch (opcode) {
            case OpCode::ADD:
            case OpCode::MUL:
                s += " R" + std::to_string(dest_reg) + ", R" + std::to_string(src1_reg) + ", R" + std::to_string(src2_reg);
                break;
            case OpCode::LOAD:
                s += " R" + std::to_string(dest_reg) + ", Mem[" + std::to_string(mem_address) + "]";
                break;
            case OpCode::STORE:
                s += " Mem[" + std::to_string(mem_address) + "], R" + std::to_string(src1_reg);
                break;
            case OpCode::HALT:
                break;
            default:
                s += " (Invalid Instruction)";
                break;
        }
        return s;
    }
};

using KernelProgram = std::vector<Instruction>;
