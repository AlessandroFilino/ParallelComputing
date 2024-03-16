cmake_minimum_required(VERSION 3.18)

project(MidTermExam LANGUAGES CUDA CXX)

# Set executable name
add_executable(MidTermExam src/main.cu)

# Set include directories
target_include_directories(MidTermExam PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

# Set compiler options for CUDA
target_compile_options(MidTermExam PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>)

# Set compiler features
target_compile_features(MidTermExam PUBLIC cxx_std_11)

# Set NVCC flags
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math")
set_target_properties(MidTermExam PROPERTIES CUDA_SEPARABLE_COMPILATION ON)