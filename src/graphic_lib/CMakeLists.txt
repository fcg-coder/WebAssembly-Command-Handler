add_library(graphic_lib)

target_sources(graphic_lib PRIVATE screen.cpp)



add_subdirectory(shapes)
target_link_libraries(main PRIVATE shapes)