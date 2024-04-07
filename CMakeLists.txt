cmake_minimum_required(VERSION 3.0)
project(AnimeTop)

add_executable(animetop main.cpp renderingengine.cpp)

target_include_directories(animetop PRIVATE "/usr/include/irrlicht")
target_link_libraries(animetop Irrlicht X11 m)
