// src/arch/ShaderCore.h
#pragma once

#include "Warp.h"
#include "Instruction.h"
#include "../memory/GPUMemory.h" // Corrected path

#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>

class ShaderCore {
private:
    int id;
    std::vector<Warp> warp_queue;

public:
    explicit ShaderCore(int sm_id) : id(sm_id) {
        std::cout << "ShaderCore " << id << " initialized." << std::endl;
    }

    int getID() const {
        return id;
    }

    void addWarp(Warp warp) {
        warp_queue.push_back(std::move(warp));
        std::cout << "Warp " << warp.getID() << " added to ShaderCore " << id << "." << std::endl;
    }

    bool executeWarps(const KernelProgram& kernel, GPUMemory& global_memory) {
        if (warp_queue.empty()) {
            std::cout << "ShaderCore " << id << ": No warps to execute." << std::endl;
            return false;
        }

        std::cout << "\n--- ShaderCore " << id << " Starting Warp Execution ---" << std::endl;

        std::vector<bool> warp_active_status(warp_queue.size(), true);
        int active_warps_count = warp_queue.size();
        int simulated_cycles = 0;

        while (active_warps_count > 0 && simulated_cycles < kernel.size() * 2) {
            bool any_warp_executed_this_cycle = false;

            for (size_t i = 0; i < warp_queue.size(); ++i) {
                if (warp_active_status[i]) {
                    Warp& current_warp = warp_queue[i];

                    if (current_warp.getCurrentInstructionIndex() < kernel.size()) {
                        const Instruction& current_instr = kernel[current_warp.getCurrentInstructionIndex()];

                        std::cout << "\n  ShaderCore " << id << " Cycle " << simulated_cycles
                                  << ": Executing Warp " << current_warp.getID()
                                  << " instruction: " << current_instr.toString() << std::endl;

                        bool still_active = current_warp.executeInstruction(current_instr, global_memory);
                        any_warp_executed_this_cycle = true;

                        if (!still_active) {
                            warp_active_status[i] = false;
                            active_warps_count--;
                            std::cout << "  Warp " << current_warp.getID() << " has completed its execution." << std::endl;
                        }
                        current_warp.printWarpState();
                    } else {
                        if (warp_active_status[i]) {
                             warp_active_status[i] = false;
                             active_warps_count--;
                             std::cout << "  Warp " << current_warp.getID() << " finished all instructions." << std::endl;
                        }
                    }
                }
            }

            if (!any_warp_executed_this_cycle && active_warps_count > 0) {
                std::cout << "  ShaderCore " << id << ": All active warps have finished their instructions. Forcing end." << std::endl;
                break;
            }
            if (active_warps_count > 0) {
                simulated_cycles++;
            } else {
                break;
            }
        }

        std::cout << "\n--- ShaderCore " << id << " Finished Warp Execution in "
                  << simulated_cycles << " cycles. Remaining active warps: "
                  << active_warps_count << " ---" << std::endl;
        return true;
    }

    const Warp& getWarp(size_t index) const {
        if (index >= warp_queue.size()) {
            throw std::out_of_range("Warp index out of bounds in ShaderCore::getWarp");
        }
        return warp_queue[index];
    }
};
