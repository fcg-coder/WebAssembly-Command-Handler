add_library(graphic_lib)
add_subdirectory(shapes)
target_link_libraries(main PRIVATE shapes)

target_sources(graphic_lib PRIVATE screen.cpp)


