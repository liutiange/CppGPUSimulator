// CppGPUSimulator/src/main.cpp
// This is the main entry point for the simplified CppGPUSimulator.

#include <iostream> 

#include "arch/Instruction.h"
#include "memory/GPUMemory.h" 
#include "arch/Thread.h"
#include "arch/Warp.h" 
#include "arch/ShaderCore.h" 
#include "sim/GPUSimulator.h" 

// Helper function to create a simple kernel program.
// This kernel performs element-wise addition: C[i] = A[i] + B[i]
// A is loaded from Mem[0 + global_thread_id] into R0
// B is loaded from Mem[100 + global_thread_id] into R1
// R2 = R0 + R1
// C is stored to Mem[200 + global_thread_id] from R2
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
    std::cout << "Starting CppGPUSimulator (Simplified Version) full system test..." << std::endl;

    // --- GPU Configuration ---
    const size_t GLOBAL_MEMORY_SIZE = 1024; 
    const int NUM_SMS = 2;   
    const int THREADS_PER_WARP = 4; 
    const int WARPS_PER_SM = 2;

    // --- Instantiate the GPUSimulator ---
    GPUSimulator gpu_simulator(GLOBAL_MEMORY_SIZE, NUM_SMS, THREADS_PER_WARP, WARPS_PER_SM);

    // --- Prepare Input Data in Global Memory ---
    const int TEST_TOTAL_THREADS = 8;

    std::cout << "\n--- Preparing Global Memory with Input Data ---" << std::endl;
    GPUMemory& global_mem = gpu_simulator.getGlobalMemory(); // Get reference to the simulator's memory

    // Initialize input data for 8 threads:
    // Input A: 10, 20, 30, 40, 50, 60, 70, 80 (at addresses 0-7)
    // Input B: 2, 4, 6, 8, 10, 12, 14, 16 (at addresses 100-107)
    for (int i = 0; i < TEST_TOTAL_THREADS; ++i) {
        global_mem.write(0 + i, (i + 1) * 10); 
        global_mem.write(100 + i, (i + 1) * 2); 
    }

    // Print initial state of relevant memory regions for verification.
    global_mem.printMemory(0, TEST_TOTAL_THREADS);
    global_mem.printMemory(100, 100 + TEST_TOTAL_THREADS);
    global_mem.printMemory(200, 200 + TEST_TOTAL_THREADS); 

    // --- Define the Kernel Program ---
    // In this simplified version, the kernel is hardcoded.
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

    std::cout << "\nCppGPUSimulator (Simplified Version) full system test finished." << std::endl;

    return 0; 
}
