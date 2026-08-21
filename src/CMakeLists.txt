cmake_minimum_required(VERSION 3.15)

project(TerminalApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Build options
# ============================================

option(USE_TERMINAL "Build terminal version" ON)
option(USE_WEBASM "Build WebAssembly version" OFF)

if(USE_WEBASM)
    add_compile_definitions(USE_WEBASM)
endif()


if(USE_TERMINAL AND USE_WEBASM)
    message(FATAL_ERROR
        "USE_TERMINAL and USE_WEBASM cannot be enabled together"
    )
endif()

if(NOT USE_TERMINAL AND NOT USE_WEBASM)
    message(FATAL_ERROR
        "Either USE_TERMINAL or USE_WEBASM must be enabled"
    )
endif()

# ============================================
# Platform
# ============================================

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# ============================================
# Main
# ============================================

add_executable(main main.cpp)

# ============================================
# Libraries
# ============================================

add_subdirectory(kernel)
add_subdirectory(graphic_lib)
add_subdirectory(command_handler)

target_link_libraries(main PRIVATE
    kernel
    graphic_lib
    command_handler
)

# ============================================
# Terminal
# ============================================

if(USE_TERMINAL)

    message(STATUS "================================")
    message(STATUS "Building TERMINAL version")
    message(STATUS "================================")

    target_compile_definitions(main PRIVATE USE_TERMINAL)
    target_compile_definitions(kernel PRIVATE USE_TERMINAL)

    target_link_libraries(main PRIVATE
        ncurses
        pthread
        rt
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(main PRIVATE
            -g
            -O0
            -Wall
            -Wextra
        )
    else()
        target_compile_options(main PRIVATE
            -O2
            -Wall
            -Wextra
        )
    endif()

    # ============================================
    # WebAssembly
    # ============================================

elseif(USE_WEBASM)

    message(STATUS "================================")
    message(STATUS "Building WEBASSEMBLY version")
    message(STATUS "================================")

    target_compile_definitions(main PRIVATE USE_WEBASM)
    target_compile_definitions(kernel PRIVATE USE_WEBASM)

    set_target_properties(main PROPERTIES
        SUFFIX ".html"
    )

    target_compile_options(main PRIVATE
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sINITIAL_MEMORY=128MB
        -sMAXIMUM_MEMORY=4GB
        -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAP8,HEAP16,HEAP32,HEAPU8,HEAPU16,HEAPU32,HEAPF32,HEAPF64
        -sEXPORTED_FUNCTIONS=_main,_getMode,_getScreen,_processInput,_setSize,_pressButton,_initScreen,_printToJS
    )

    target_link_options(main PRIVATE
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sINITIAL_MEMORY=128MB
        -sMAXIMUM_MEMORY=4GB
        -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAP8,HEAP16,HEAP32,HEAPU8,HEAPU16,HEAPU32,HEAPF32,HEAPF64
        -sEXPORTED_FUNCTIONS=_main,_getMode,_getScreen,_processInput,_setSize,_pressButton,_initScreen,_printToJS
    )

endif()
