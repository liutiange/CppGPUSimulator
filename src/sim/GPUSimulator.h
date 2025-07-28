// src/sim/GPUSimulator.h
#pragma once 

#include "../arch/ShaderCore.h"  
#include "../memory/GPUMemory.h" 
#include "../arch/Instruction.h" 
#include <vector>              
#include <iostream>        
#include <numeric>         
#include <cmath> 

// The GPUSimulator class orchestrates the entire GPU simulation.
// It manages multiple ShaderCores and the global memory, and handles kernel launches.
class GPUSimulator {
private:
    int num_sms;
    int threads_per_warp; 
    int warps_per_sm;   

    GPUMemory global_memory;
    std::vector<ShaderCore> sms;


public:
    GPUSimulator(size_t global_memory_size, int num_sms, int threads_per_warp, int warps_per_sm)
        : global_memory(global_memory_size),
          num_sms(num_sms),
          threads_per_warp(threads_per_warp),
          warps_per_sm(warps_per_sm)
    {
        if (num_sms <= 0 || threads_per_warp <= 0 || warps_per_sm <= 0) {
            std::cerr << "Warning: GPUSimulator initialized with non-positive configuration values." << std::endl;
        }

        // Initialize ShaderCore instances
        for (int i = 0; i < num_sms; ++i) {
            sms.emplace_back(i); // Each ShaderCore gets a unique ID
        }
        std::cout << "GPUSimulator initialized with " << num_sms << " ShaderCores." << std::endl;
    }

    // Provides access to the global memory for external setup (e.g., loading input data).
    GPUMemory& getGlobalMemory() {
        return global_memory;
    }

    
    void launchKernel(const KernelProgram& kernel_program,
                      int grid_dim_x, int grid_dim_y, int grid_dim_z,
                      int block_dim_x, int block_dim_y, int block_dim_z) {

        std::cout << "\n--- Launching Kernel ---" << std::endl;
        std::cout << "  Grid Dimensions: (" << grid_dim_x << ", " << grid_dim_y << ", " << grid_dim_z << ")" << std::endl;
        std::cout << "  Block Dimensions: (" << block_dim_x << ", " << block_dim_y << ", " << block_dim_z << ")" << std::endl;

        long long total_blocks = static_cast<long long>(grid_dim_x) * grid_dim_y * grid_dim_z;
        long long threads_per_block = static_cast<long long>(block_dim_x) * block_dim_y * block_dim_z;
        long long total_threads = total_blocks * threads_per_block;

        // Calculate total warps needed. Use ceil to ensure all threads are covered.
        long long total_warps = static_cast<long long>(std::ceil(static_cast<double>(total_threads) / threads_per_warp));

        std::cout << "  Total Threads to launch: " << total_threads << std::endl;
        std::cout << "  Total Warps to launch: " << total_warps << std::endl;

        if (total_warps == 0) {
            std::cout << "No warps to launch. Kernel launch aborted." << std::endl;
            return;
        }



        std::vector<Warp> all_launched_warps;
        all_launched_warps.reserve(total_warps); // Pre-allocate memory

        for (long long i = 0; i < total_warps; ++i) {

            all_launched_warps.emplace_back(i, threads_per_warp, 8);
        }


        if (!sms.empty()) {
            for (Warp& warp : all_launched_warps) {
                // Simple round-robin assignment to SMs
                int sm_idx = warp.getID() % num_sms;
                sms[sm_idx].addWarp(std::move(warp));
            }
        } else {
            std::cerr << "Error: No ShaderCores available in GPUSimulator to launch warps." << std::endl;
            return;
        }


        bool all_sms_completed = true;
        for (ShaderCore& sm : sms) {
            std::cout << "\nExecuting warps on ShaderCore " << sm.getID() << "..." << std::endl;
            bool sm_success = sm.executeWarps(kernel_program, global_memory);
            if (!sm_success) {
                all_sms_completed = false;
            }
        }

        if (all_sms_completed) {
            std::cout << "\n--- Kernel Launch Completed Successfully ---" << std::endl;
        } else {
            std::cout << "\n--- Kernel Launch Completed with Issues ---" << std::endl;
        }
    }
};
