# CppGPUSimulator: A Simplified GPU Architectural Simulator (C++)

## Overview

CppGPUSimulator is a foundational project designed to simulate the core architectural components and execution principles of a Graphics Processing Unit (GPU) using C++. This simulator provides a clear, simplified model to understand how GPUs achieve massive parallelism, particularly through the Single Instruction, Multiple Data (SIMD) execution model.

## Key Features

- **Core GPU Components**  
  - **ShaderCores (Streaming Multiprocessors):** Manage and execute warps.  
  - **Warps:** Groups of threads that execute instructions in SIMD fashion.  
  - **Threads:** Individual execution units with their own registers and program counters.  
  - **Global Memory:** A shared memory space accessible by all threads.

- **Simplified Instruction Set**  
  - `ADD`: Element-wise addition.  
  - `MUL`: Element-wise multiplication.  
  - `LOAD`: Load data from global memory into a thread's register.  
  - `STORE`: Store data from a thread's register into global memory.  
  - `HALT`: Terminates kernel execution.

- **Parallel Execution Demonstration**  
  Illustrates how multiple threads within a warp, and multiple warps across ShaderCores, execute instructions concurrently on distinct data.

- **Functional Correctness**  
  Focuses on ensuring that the simulated instructions produce accurate results.

## Project Structure

The project is organized to reflect a modular approach to hardware modeling:

CppGPUSimulator/
├── src/
│   ├── arch/
│   │   ├── Instruction.h    # Defines the custom instruction set.
│   │   ├── Thread.h         # Represents a single GPU thread with registers and PC.
│   │   ├── Warp.h           # Groups threads and executes instructions in SIMD.
│   │   └── ShaderCore.h     # Manages warps, analogous to a Streaming Multiprocessor.
│   ├── memory/
│   │   └── GPUMemory.h      # Simulates the GPU's global memory.
│   ├── sim/
│   │   └── GPUSimulator.h   # Orchestrates the entire GPU simulation.
│   └── main.cpp             # The main application entry point, defines and launches a kernel.
├── CMakeLists.txt           # CMake build configuration for the project.
├── README.md                # This file.
└── LICENSE                  # Project license (MIT License).


## How to Build

This project uses CMake for its build system.

**Prerequisites:**
- A C++ compiler with C++17 support (e.g., `g++`, `clang++`)  
- CMake (version 3.10 or higher)

**Steps:**

```bash
# Clone the repository
git clone https://github.com/liutiange/CppGPUSimulator.git
cd CppGPUSimulator

# Create and enter a build directory
mkdir build
cd build

# Configure and generate build files
cmake ..

# Build the project
cmake --build . 
```

This will compile the source code and create the `gpu_sim` executable in the `build/` directory.

## How to Run

After successfully building the project, you can run the simulator from the `build/` directory by executing `./gpu_sim`.

```bash
./gpu_sim
```

The output will display:

- Initialization of GPU components

- Preparation of input data in global memory

- The hardcoded kernel program

- A detailed trace of the simulated execution on the ShaderCores and Warps

- The computed results in global memory

## Example Output Snippet

```
Starting CppGPUSimulator (Simplified Version) full system test...
GPUMemory initialized with 1024 units.
GPUSimulator initialized with 2 ShaderCores.

--- Preparing Global Memory with Input Data ---
... (initial memory state) ...

--- Kernel Program to be Launched ---
  - LOAD R0, Mem[0]
  - LOAD R1, Mem[100]
  - ADD R2, R0, R1
  - STORE Mem[200], R2
  - HALT

--- Launching Kernel ---
  Grid Dimensions: (1, 1, 1)
  Block Dimensions: (8, 1, 1)
  Total Threads to launch: 8
  Total Warps to launch: 2

Executing warps on ShaderCore 0...
--- ShaderCore 0 Starting Warp Execution ---
  ShaderCore 0 Cycle 0: Executing Warp 0 instruction: LOAD R0, Mem[0]
--- Warp 0 State (Current Instruction Index: 1) ---
Thread 0 (PC=1) Registers: R0=10 R1=0 R2=0 R3=0 
Thread 1 (PC=1) Registers: R0=20 R1=0 R2=0 R3=0 
... (more execution cycles and Warp states) ...

--- ShaderCore 0 Finished Warp Execution in 4 cycles. Remaining active warps: 0 ---

Executing warps on ShaderCore 1...
... (similar execution trace for ShaderCore 1) ...

--- Kernel Launch Completed Successfully ---

--- Final Global Memory State (Output Region) ---
Mem[200]: 12
Mem[201]: 24
Mem[202]: 36
Mem[203]: 48
Mem[204]: 60
Mem[205]: 72
Mem[206]: 84
Mem[207]: 96
----------------------------------------------------

CppGPUSimulator (Simplified Version) full system test finished.

```

## Future Enhancements

- **External Kernel Loading:**  
  Implement a `KernelLoader` to load kernel programs from external assembly files (`.asm`), adding a `kernels/` directory.

- **Performance Profiling:**  
  Integrate a profiler to collect and report metrics like cycle counts, instruction counts, and memory access patterns.

- **Comprehensive Unit Testing:**  
  Add a `tests/` directory with a C++ unit testing framework (e.g., Google Test) to verify individual components.

- **More Complex ISA:**  
  Support additional instruction types (branches, logical operations, floating‑point arithmetic).

- **Thread Divergence:**  
  Simulate warp execution masks for control‑flow divergence (`if/else`).

- **Memory Hierarchy:**  
  Introduce caches (L1, L2) and shared memory with realistic latency models.

- **Multiple ShaderCores:**  
  Enhance the simulator to truly parallelize execution across multiple ShaderCores.

- **Flexible I/O:**  
  Implement flexible mechanisms to load input data and retrieve results.
