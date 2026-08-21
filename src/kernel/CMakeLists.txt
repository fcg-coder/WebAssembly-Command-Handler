add_library(kernel)

target_sources(kernel PRIVATE
    shell_base.hpp
    kernel.hpp
    kernel.cpp
)

if(USE_TERMINAL)
    target_sources(kernel PRIVATE
        terminal_ioh.hpp
        terminal_ioh.cpp
    )

    target_compile_definitions(kernel PRIVATE USE_TERMINAL)

    target_link_libraries(kernel PRIVATE
        ncurses
        pthread
    )
endif()

# if(USE_WEBASM)
#     target_sources(kernel PRIVATE
#         web_ioh.cpp
#         web_ioh.hpp
#     )

#     target_compile_definitions(kernel PRIVATE USE_WEBASM)
# endif()

target_include_directories(kernel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
