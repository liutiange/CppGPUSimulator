// CppGPUSimulator/src/main.cpp
// Removed #pragma once here as it's typically not used in .cpp files, only headers.

#include <iostream>
#include "arch/Instruction.h"
#include "memory/GPUMemory.h"
#include "arch/Thread.h"
#include "arch/Warp.h"
#include "arch/ShaderCore.h"
#include "sim/GPUSimulator.h"
#include "sim/Profiler.h"

// Helper function to create a simple kernel for testing
KernelProgram create_simple_add_kernel() {
    KernelProgram kernel;
    kernel.emplace_back(OpCode::LOAD, 0, -1, -1, 0);
    kernel.emplace_back(OpCode::LOAD, 1, -1, -1, 100);
    kernel.emplace_back(OpCode::ADD, 2, 0, 1);
    kernel.emplace_back(OpCode::STORE, -1, 2, -1, 200);
    kernel.emplace_back(OpCode::HALT);
    return kernel;
}

int main() {
    std::cout << "Starting CppGPUSimulator full system test with Profiler..." << std::endl;

    // --- GPU Configuration ---
    const size_t GLOBAL_MEMORY_SIZE = 1024;
    const int NUM_SMS = 2;
    const int THREADS_PER_WARP = 4;
    const int WARPS_PER_SM = 2;

    // --- Instantiate the GPUSimulator ---
    GPUSimulator gpu_simulator(GLOBAL_MEMORY_SIZE, NUM_SMS, THREADS_PER_WARP, WARPS_PER_SM);

    // --- Prepare Input Data in Global Memory ---
    const int TEST_TOTAL_THREADS = 8; // 2 warps * 4 threads/warp
    std::cout << "\n--- Preparing Global Memory with Input Data ---" << std::endl;
    GPUMemory& global_mem = gpu_simulator.getGlobalMemory();

    for (int i = 0; i < TEST_TOTAL_THREADS; ++i) {
        global_mem.write(0 + i, (i + 1) * 10);
        global_mem.write(100 + i, (i + 1) * 2);
    }

    global_mem.printMemory(0, TEST_TOTAL_THREADS);
    global_mem.printMemory(100, 100 + TEST_TOTAL_THREADS);
    global_mem.printMemory(200, 200 + TEST_TOTAL_THREADS);

    // --- Define the Kernel Program ---
    KernelProgram add_kernel = create_simple_add_kernel();
    std::cout << "\n--- Kernel Program to be Launched ---" << std::endl;
    for (const auto& instr : add_kernel) {
        std::cout << "  - " << instr.toString() << std::endl;
    }

    // --- Launch the Kernel ---
    gpu_simulator.launchKernel(add_kernel, 1, 1, 1, TEST_TOTAL_THREADS, 1, 1);

    // --- Verify Final Output in Global Memory ---
    std::cout << "\n--- Final Global Memory State (Output Region) ---" << std::endl;
    global_mem.printMemory(200, 200 + TEST_TOTAL_THREADS);

    // --- Print Profiling Summary ---
    gpu_simulator.getProfiler().printSummary();

    std::cout << "\nCppGPUSimulator full system test finished." << std::endl;

    return 0;
}
