// src/arch/Warp.h
#pragma once

#include "Thread.h"
#include "Instruction.h"
#include "../memory/GPUMemory.h" // Corrected path

#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

class Warp {
private:
    int id;
    std::vector<Thread> threads;
    int current_instruction_index;

public:
    Warp(int warp_id, int num_threads_per_warp, int num_registers_per_thread)
        : id(warp_id), current_instruction_index(0) {
        if (num_threads_per_warp <= 0) {
            std::cerr << "Warning (Warp " << id << "): Initializing with non-positive number of threads." << std::endl;
        }
        for (int i = 0; i < num_threads_per_warp; ++i) {
            threads.emplace_back(warp_id * num_threads_per_warp + i, num_registers_per_thread);
        }
        std::cout << "Warp " << id << " initialized with " << num_threads_per_warp << " threads." << std::endl;
    }

    int getID() const {
        return id;
    }

    size_t getNumThreads() const {
        return threads.size();
    }

    int getCurrentInstructionIndex() const {
        return current_instruction_index;
    }

    bool executeInstruction(const Instruction& instr, GPUMemory& global_memory) {
        bool any_thread_active = false;

        for (Thread& thread : threads) {
            switch (instr.opcode) {
                case OpCode::ADD: {
                    int val1 = thread.getRegister(instr.src1_reg);
                    int val2 = thread.getRegister(instr.src2_reg);
                    thread.setRegister(instr.dest_reg, val1 + val2);
                    break;
                }
                case OpCode::MUL: {
                    int val1 = thread.getRegister(instr.src1_reg);
                    int val2 = thread.getRegister(instr.src2_reg);
                    thread.setRegister(instr.dest_reg, val1 * val2);
                    break;
                }
                case OpCode::LOAD: {
                    int effective_address = instr.mem_address + thread.getID();
                    int value = global_memory.read(effective_address);
                    thread.setRegister(instr.dest_reg, value);
                    break;
                }
                case OpCode::STORE: {
                    int effective_address = instr.mem_address + thread.getID();
                    int value_to_store = thread.getRegister(instr.src1_reg);
                    global_memory.write(effective_address, value_to_store);
                    break;
                }
                case OpCode::HALT:
                    break;
                default:
                    std::cerr << "Error (Warp " << id << "): Unknown opcode encountered: "
                              << instr.opcodeToString() << std::endl;
                    break;
            }
            thread.incrementPC();
            any_thread_active = true;
        }

        current_instruction_index++;

        return instr.opcode != OpCode::HALT;
    }

    void initializeThreadRegisters(int reg_idx, int initial_value_base) {
        for (int i = 0; i < threads.size(); ++i) {
            threads[i].setRegister(reg_idx, initial_value_base + threads[i].getID());
        }
    }

    void setThreadsRegister(int reg_idx, int value) {
        for (Thread& thread : threads) {
            thread.setRegister(reg_idx, value);
        }
    }

    void printWarpState() const {
        std::cout << "\n--- Warp " << id << " State (Current Instruction Index: "
                  << current_instruction_index << ") ---" << std::endl;
        for (const Thread& thread : threads) {
            thread.printRegisters();
        }
        std::cout << "--------------------------------------------------------" << std::endl;
    }

    const Thread& getThread(size_t index) const {
        if (index >= threads.size()) {
            throw std::out_of_range("Thread index out of bounds in Warp::getThread");
        }
        return threads[index];
    }
};
