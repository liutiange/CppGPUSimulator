// src/memory/GPUMemory.h
#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>
#include "../sim/Profiler.h" // Corrected path

class GPUMemory {
private:
    std::vector<int> data;
    Profiler* profiler;

public:
    explicit GPUMemory(size_t size, Profiler* p = nullptr) : data(size, 0), profiler(p) {
        if (size == 0) {
            std::cerr << "Warning: Initializing GPUMemory with size 0. No memory will be available." << std::endl;
        }
        std::cout << "GPUMemory initialized with " << size << " units." << std::endl;
        if (profiler) {
            profiler->logMemoryOperation("allocate", 0, size * sizeof(int));
        }
    }

    ~GPUMemory() {
        if (profiler) {
            profiler->logMemoryOperation("release", 0, data.size() * sizeof(int));
        }
    }

    size_t getSize() const {
        return data.size();
    }

    int read(int address) const {
        if (address < 0 || address >= data.size()) {
            std::cerr << "Error: Memory read out of bounds at address " << address
                      << ". Memory size: " << data.size() << std::endl;
            return 0;
        }
        if (profiler) {
            profiler->logMemoryOperation("read", address, sizeof(int));
        }
        return data[address];
    }

    void write(int address, int value) {
        if (address < 0 || address >= data.size()) {
            std::cerr << "Error: Memory write out of bounds at address " << address
                      << ". Memory size: " << data.size() << std::endl;
            return;
        }
        if (profiler) {
            profiler->logMemoryOperation("write", address, sizeof(int));
        }
        data[address] = value;
    }

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
