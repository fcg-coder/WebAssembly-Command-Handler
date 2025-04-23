add_library(command_handler)

target_sources(command_handler PRIVATE command_handler.cpp)

add_subdirectory(commands)
target_link_libraries(main PRIVATE commands)

add_subdirectory(history)
target_link_libraries(main PRIVATE history)

add_subdirectory(menu)
target_link_libraries(main PRIVATE menu)