add_library(kernel)


if(USE_TERMINAL)
    target_sources(kernel PRIVATE
        terminal_ioh.hpp
        terminal_ioh.cpp
    )
    target_compile_definitions(kernel PRIVATE USE_TERMINAL)

    if(LINUX)
        target_link_libraries(kernel PRIVATE ncurses pthread)
    endif()
endif()

if(USE_WEBASM)
    target_sources(kernel PRIVATE
        web_ioh.cpp
        web_ioh.hpp
    )
    target_compile_definitions(kernel PRIVATE USE_WEBASM)
endif()


target_sources(kernel PRIVATE
    interface.hpp
    kernel.hpp
    kernel.cpp
)
# Общие директории для заголовков
target_include_directories(kernel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
