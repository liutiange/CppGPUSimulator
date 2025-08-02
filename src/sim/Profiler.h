// src/sim/Profiler.h
#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <iostream>

struct KernelTrace {
    std::string kernel_name;
    std::string grid_dim_str;
    std::string block_dim_str;
    double duration_ms;
};

struct MemoryTrace {
    std::string op_type;
    long long address;
    size_t size_bytes;
    double time_stamp_ms;
};

class Profiler {
private:
    std::vector<KernelTrace> kernel_traces;
    std::vector<MemoryTrace> memory_traces;
    std::chrono::high_resolution_clock::time_point start_time;
    bool profiling_active;

public:
    Profiler() : profiling_active(false) {
        std::cout << "Profiler initialized." << std::endl;
    }

    void startProfiling() {
        start_time = std::chrono::high_resolution_clock::now();
        profiling_active = true;
        std::cout << "Profiling started." << std::endl;
    }

    void stopProfiling() {
        profiling_active = false;
        std::cout << "Profiling stopped." << std::endl;
    }

    void logKernelExecution(const std::string& kernel_name,
                            const std::string& grid_dim_str,
                            const std::string& block_dim_str,
                            double duration_ms) {
        if (profiling_active) {
            kernel_traces.push_back({kernel_name, grid_dim_str, block_dim_str, duration_ms});
        }
    }

    void logMemoryOperation(const std::string& op_type, long long address, size_t size_bytes) {
        if (profiling_active) {
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = current_time - start_time;
            memory_traces.push_back({op_type, address, size_bytes, elapsed.count()});
        }
    }

    void printSummary() const {
        std::cout << "\n--- Profiling Summary ---" << std::endl;

        if (kernel_traces.empty() && memory_traces.empty()) {
            std::cout << "No profiling data collected." << std::endl;
            return;
        }

        std::cout << "\nKernel Execution Summary:" << std::endl;
        if (kernel_traces.empty()) {
            std::cout << "  No kernel executions logged." << std::endl;
        } else {
            for (const auto& trace : kernel_traces) {
                std::cout << "  - Kernel: " << trace.kernel_name
                          << ", Grid: " << trace.grid_dim_str
                          << ", Block: " << trace.block_dim_str
                          << ", Duration: " << trace.duration_ms << " ms" << std::endl;
            }
        }

        std::cout << "\nMemory Operations Summary (Total: " << memory_traces.size() << "):" << std::endl;
        if (memory_traces.empty()) {
            std::cout << "  No memory operations logged." << std::endl;
        } else {
            int reads = 0, writes = 0, allocates = 0, releases = 0;
            size_t total_read_bytes = 0, total_written_bytes = 0;

            for (const auto& trace : memory_traces) {
                if (trace.op_type == "read") {
                    reads++;
                    total_read_bytes += trace.size_bytes;
                } else if (trace.op_type == "write") {
                    writes++;
                    total_written_bytes += trace.size_bytes;
                } else if (trace.op_type == "allocate") {
                    allocates++;
                } else if (trace.op_type == "release") {
                    releases++;
                }
            }
            std::cout << "  Reads: " << reads << " (" << total_read_bytes << " bytes)" << std::endl;
            std::cout << "  Writes: " << writes << " (" << total_written_bytes << " bytes)" << std::endl;
            std::cout << "  Allocates: " << allocates << std::endl;
            std::cout << "  Releases: " << releases << std::endl;

            std::cout << "\n  Recent Memory Operations (up to 5):" << std::endl;
            int count = 0;
            for (auto it = memory_traces.rbegin(); it != memory_traces.rend() && count < 5; ++it, ++count) {
                std::cout << "    - Type: " << it->op_type
                          << ", Address: " << it->address
                          << ", Size: " << it->size_bytes << " bytes"
                          << ", Time: " << it->time_stamp_ms << " ms" << std::endl;
            }
        }

        std::cout << "-------------------------" << std::endl;
    }

    void clear() {
        kernel_traces.clear();
        memory_traces.clear();
        profiling_active = false;
        std::cout << "Profiler data cleared." << std::endl;
    }
};
