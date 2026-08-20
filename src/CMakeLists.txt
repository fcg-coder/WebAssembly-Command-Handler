cmake_minimum_required(VERSION 3.15)
project(WebAsm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)

if(EMSCRIPTEN)
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

endif()


add_subdirectory(kernel)
target_link_libraries(main PRIVATE kernel)

add_subdirectory(command_handler)
target_link_libraries(main PRIVATE command_handler)

add_subdirectory(graphic_lib)
target_link_libraries(main PRIVATE graphic_lib)
