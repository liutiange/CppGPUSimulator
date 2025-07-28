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
│   ├── arch/               # Architectural components (Instruction, Thread, Warp, ShaderCore)
│   │   ├── Instruction.h   # Defines the custom instruction set.
│   │   ├── Thread.h        # Represents a single GPU thread with registers and PC.
│   │   ├── Warp.h          # Groups threads and executes instructions in SIMD.
│   │   └── ShaderCore.h    # Manages warps, analogous to a Streaming Multiprocessor.
│   ├── memory/             # Memory system components
│   │   └── GPUMemory.h     # Simulates the GPU's global memory.
│   ├── sim/                # Overall simulation control
│   │   └── GPUSimulator.h  # Orchestrates the entire GPU simulation.
│   └── main.cpp            # The main application entry point, defines and launches a kernel.
├── CMakeLists.txt          # CMake build configuration for the project.
├── README.md               # This file.
└── LICENSE                 # Project license (e.g., MIT License).
