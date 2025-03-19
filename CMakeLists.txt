cmake_minimum_required(VERSION 3.15)
project(WebAsm)

 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

 
add_executable(main src/main.cpp)

 
if (EMSCRIPTEN)
    set_target_properties(main PROPERTIES SUFFIX ".html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s  EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'")
endif()

add_subdirectory(src/command_handler)
target_link_libraries(main PRIVATE command_handler)