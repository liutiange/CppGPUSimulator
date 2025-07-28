    # CppGPUSimulator/CMakeLists.txt
    cmake_minimum_required(VERSION 3.10) 
    project(CppGPUSimulator CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


    include_directories(src)

    add_executable(gpu_sim src/main.cpp)

