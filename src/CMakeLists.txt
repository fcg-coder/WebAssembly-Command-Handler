cmake_minimum_required(VERSION 3.15)

project(TerminalApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Build options
# ============================================

option(USE_TERMINAL "Build terminal version" ON)
option(USE_WEBASM "Build WebAssembly version" OFF)

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
# Platform / Global flags
# ============================================

if(USE_TERMINAL)

    message(STATUS "================================")
    message(STATUS "Building TERMINAL version")
    message(STATUS "================================")

    add_compile_definitions(USE_TERMINAL)

    # --------------------------------------------
    # Platform-specific terminal libraries
    # --------------------------------------------

    if(APPLE)

        message(STATUS "Platform: macOS")

    elseif(UNIX)

        message(STATUS "Platform: Linux")

    else()

        message(FATAL_ERROR
            "Unsupported platform for TERMINAL version"
        )

    endif()

elseif(USE_WEBASM)

    message(STATUS "================================")
    message(STATUS "Building WEBASSEMBLY version")
    message(STATUS "================================")

    if(NOT EMSCRIPTEN)
        message(FATAL_ERROR
            "USE_WEBASM requires the Emscripten toolchain"
        )
    endif()

    add_compile_definitions(USE_WEBASM)

endif()

# ============================================
# Global compiler warnings
# ============================================

add_compile_options(
    -Wall
    -Wextra
)

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

    # ncurses is required on both macOS and Linux.
    target_link_libraries(main PRIVATE
        ncurses
    )

    # pthread and rt are Linux-specific.
    # macOS provides threading functionality through its system libraries
    # and does not provide a separate librt.
    if(UNIX AND NOT APPLE)

        target_link_libraries(main PRIVATE
            pthread
            rt
        )

    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_compile_options(
            -g
            -O0
        )

    else()

        add_compile_options(
            -O2
        )

    endif()

# ============================================
# WebAssembly
# ============================================

elseif(USE_WEBASM)

    set_target_properties(main PROPERTIES
        SUFFIX ".html"
    )

    target_link_options(main PRIVATE
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sINITIAL_MEMORY=128MB
        -sMAXIMUM_MEMORY=4GB
        -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,HEAP8,HEAP16,HEAP32,HEAPU8,HEAPU16,HEAPU32,HEAPF32,HEAPF64
        -sEXPORTED_FUNCTIONS=_main,_getMode,_getScreen,_output,_input,_setSize
    )

endif()