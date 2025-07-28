// src/memory/GPUMemory.h
#pragma once 

#include <vector>   
#include <iostream> 
#include <stdexcept> 

// The GPUMemory class simulates the main global memory of the GPU.
// For simplicity, it's modeled as a contiguous block of integers.
class GPUMemory {
private:
    std::vector<int> data;

public:
    // Constructor: Initializes the memory with a specified size.
    // All memory locations are initialized to 0.
    explicit GPUMemory(size_t size) : data(size, 0) {
        if (size == 0) {
            std::cerr << "Warning: Initializing GPUMemory with size 0. No memory will be available." << std::endl;
        }
        std::cout << "GPUMemory initialized with " << size << " units." << std::endl;
    }

    // Returns the total size of the memory in units (e.g., number of integers).
    size_t getSize() const {
        return data.size();
    }

    // Reads an integer value from the specified memory address.
    // Includes basic bounds checking to prevent invalid memory access.
    int read(int address) const {
        if (address < 0 || address >= data.size()) {
            std::cerr << "Error: Memory read out of bounds at address " << address
                      << ". Memory size: " << data.size() << std::endl;
            return 0; 
        }
        return data[address];
    }

    // Writes an integer value to the specified memory address.
    // Includes basic bounds checking.
    void write(int address, int value) {
        if (address < 0 || address >= data.size()) {
            std::cerr << "Error: Memory write out of bounds at address " << address
                      << ". Memory size: " << data.size() << std::endl;
            return; 
        }
        data[address] = value;
    }

    // Prints a portion of the memory for debugging purposes.
    // Allows specifying a start and end address to view a range.
    void printMemory(int start_addr = 0, int end_addr = 10) const {
        std::cout << "\n--- Global Memory Contents (" << start_addr << " to " << end_addr << ") ---" << std::endl;
        for (int i = start_addr; i < end_addr && i < data.size(); ++i) {
            std::cout << "Mem[" << i << "]: " << data[i] << std::endl;
        }
        if (end_addr > data.size()) {
            std::cout << "(Display truncated as end_addr exceeds memory size)" << std::endl;
        }
        std::cout << "----------------------------------------------------" << std::endl;
    }
};
