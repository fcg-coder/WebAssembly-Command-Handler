cmake_minimum_required(VERSION 3.15)
project(TerminalApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Опция для сборки под WebAssembly
option(BUILD_WEBASM "Build for WebAssembly" OFF)

# Определяем платформу
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Если не WebAssembly, то собираем для терминала
if(NOT BUILD_WEBASM)
    set(BUILD_TERMINAL TRUE)
endif()

# Основной исполняемый файл
add_executable(main main.cpp)

# Подключаем поддиректории
add_subdirectory(kernel)
target_link_libraries(main PRIVATE kernel)

add_subdirectory(graphic_lib)
target_link_libraries(main PRIVATE graphic_lib)

add_subdirectory(command_handler)
target_link_libraries(main PRIVATE command_handler)


# Настройки для разных платформ
if(BUILD_TERMINAL)
    message(STATUS "Building for terminal (Linux)")

    target_link_libraries(main PRIVATE
        ncurses
        pthread
        rt
    )

    target_compile_definitions(main PRIVATE USE_TERMINAL)
    target_compile_definitions(kernel PRIVATE USE_TERMINAL)

    # Опции компиляции для Linux
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(main PRIVATE -g -O0 -Wall -Wextra)
    else()
        target_compile_options(main PRIVATE -O2 -Wall -Wextra)
    endif()

elseif(BUILD_WEBASM)
    message(STATUS "Building for WebAssembly")

    set_target_properties(main PROPERTIES SUFFIX ".html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

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

    target_compile_definitions(main PRIVATE USE_WEBASM)
    target_compile_definitions(kernel PRIVATE USE_WEBASM)
endif()
