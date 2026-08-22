add_library(graphic_lib)

add_subdirectory(shapes)

target_sources(graphic_lib PRIVATE
    screen.cpp
)

target_link_libraries(graphic_lib PUBLIC
    shapes
)
