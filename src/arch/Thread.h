// src/arch/Thread.h
#pragma once

#include <vector>
#include <iostream>
#include <string>

class Thread {
private:
    int id;
    std::vector<int> registers;
    int program_counter;

public:
    Thread(int thread_id, int num_registers)
        : id(thread_id), registers(num_registers, 0), program_counter(0) {
        if (num_registers <= 0) {
            std::cerr << "Warning (Thread " << id << "): Initializing with non-positive number of registers." << std::endl;
        }
    }

    int getID() const {
        return id;
    }

    int getRegister(int index) const {
        if (index < 0 || index >= registers.size()) {
            std::cerr << "Error (Thread " << id << "): Register index " << index
                      << " out of bounds. Max index: " << registers.size() - 1 << std::endl;
            return 0;
        }
        return registers[index];
    }

    void setRegister(int index, int value) {
        if (index < 0 || index >= registers.size()) {
            std::cerr << "Error (Thread " << id << "): Register index " << index
                      << " out of bounds. Max index: " << registers.size() - 1 << std::endl;
            return;
        }
        registers[index] = value;
    }

    int getPC() const {
        return program_counter;
    }

    void incrementPC() {
        program_counter++;
    }

    void setPC(int pc) {
        if (pc < 0) {
            std::cerr << "Warning (Thread " << id << "): Setting PC to a negative value " << pc << std::endl;
        }
        program_counter = pc;
    }

    void printRegisters() const {
        std::cout << "Thread " << id << " (PC=" << program_counter << ") Registers: ";
        for (size_t i = 0; i < registers.size(); ++i) {
            std::cout << "R" << i << "=" << registers[i] << " ";
        }
        std::cout << std::endl;
    }
};
