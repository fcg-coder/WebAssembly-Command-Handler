cmake_minimum_required(VERSION 3.15)
project(WebAsm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)

if (EMSCRIPTEN)
    set_target_properties(main PROPERTIES SUFFIX ".html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sWASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s INITIAL_MEMORY=128MB") #RAM 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s MAXIMUM_MEMORY=4GB")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msimd128") # Enable SIMD
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSIMD") # Define SIMD macro
 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]")
    
 
endif()

add_subdirectory(command_handler)
target_link_libraries(main PRIVATE command_handler)

add_subdirectory(graphic_lib)
target_link_libraries(main PRIVATE graphic_lib)