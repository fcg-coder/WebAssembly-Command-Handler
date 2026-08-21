add_library(command_handler)

target_sources(command_handler PRIVATE
    command_handler.cpp
    command_handler.hpp
)

# ============================================================
# Commands
# ============================================================

add_subdirectory(commands)

# command_handler использует команды
target_link_libraries(command_handler PRIVATE
    commands
)

# ============================================================
# History
# ============================================================

add_subdirectory(history)

# command_handler использует историю
target_link_libraries(command_handler PRIVATE
    history
)

# ============================================================
# Include directories
# ============================================================

target_include_directories(command_handler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
