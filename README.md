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

- **Basic Performance Profiling**
  Includes a built-in profiler to collect and report key simulation metrics such as kernel execution duration and memory operation counts (reads, writes, allocations, releases).

## Project Structure

The project is organized to reflect a modular approach to hardware modeling:


CppGPUSimulator/
├── src/
│   ├── arch/               # Architectural components (Instruction, Thread, Warp, ShaderCore)
│   │   ├── Instruction.h   # Defines the custom instruction set.
│   │   ├── Thread.h        # Represents a single GPU thread with registers and PC.
│   │   ├── Warp.h          # Groups threads and executes instructions in SIMD.
│   │   └── ShaderCore.h    # Manages warps, analogous to a Streaming Multiprocessor.
│   ├── memory/             # Memory system components
│   │   └── GPUMemory.h     # Simulates the GPU's global memory.
│   ├── sim/                # Overall simulation control and utilities
│   │   ├── GPUSimulator.h  # Orchestrates the entire GPU simulation.
│   │   └── Profiler.h      # Collects and reports simulation performance metrics.
│   └── main.cpp            # The main application entry point, defines and launches a kernel.
├── CMakeLists.txt          # CMake build configuration for the project.
├── README.md               # This file.
└── LICENSE                 # Project license.


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
Starting CppGPUSimulator full system test with Profiler...
GPUMemory initialized with 1024 units.
Profiler initialized.
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
  Grid Dimensions: (1,1,1), Block Dimensions: (8,1,1)
  Total Threads to launch: 8, Total Warps to launch: 2
Profiling started.
... (detailed execution trace for ShaderCore 0 and 1) ...
--- Kernel Launch Completed Successfully ---

--- Final Global Memory State (Output Region) ---
--- Global Memory Contents (200 to 208) ---
Mem[200]: 12
Mem[201]: 24
Mem[202]: 36
Mem[203]: 48
Mem[204]: 60
Mem[205]: 72
Mem[206]: 84
Mem[207]: 96
----------------------------------------------------

--- Profiling Summary ---
Kernel Execution Summary:
  - Kernel: SimpleAddKernel, Grid: (1,1,1), Block: (8,1,1), Duration: X.XXX ms (actual duration will vary)

Memory Operations Summary (Total: Y):
  Reads: Z (A bytes)
  Writes: W (B bytes)
  Allocates: C
  Releases: D

  Recent Memory Operations (up to 5):
    - Type: write, Address: ..., Size: ... bytes, Time: ... ms
    ...
-------------------------

CppGPUSimulator full system test finished.

```
