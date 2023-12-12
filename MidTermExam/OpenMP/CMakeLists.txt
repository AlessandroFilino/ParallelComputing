cmake_minimum_required(VERSION 3.23)
project(MidTermExam)

set(CMAKE_CXX_STANDARD 17)

#FOR LINUX
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#[===[ FOR APPLE
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lomp")
if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        include_directories(/opt/homebrew/opt/libomp/include)
        link_directories(/opt/homebrew/opt/libomp/lib)
    else()
        include_directories(/usr/local/opt/libomp/include)
        link_directories(/usr/local/opt/libomp/lib)
    endif()
endif(APPLE)
]===]

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/googletest)