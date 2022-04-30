cmake_minimum_required (VERSION 3.13)
project(dauw)

# Set language standards
set(CMAKE_CXX_STANDARD 17)

# Add sources to the project
file(GLOB_RECURSE DAUW_HEADERS src/*.h)
file(GLOB_RECURSE DAUW_SOURCES src/*.cpp)
add_executable(dauw ${DAUW_HEADERS} ${DAUW_SOURCES})

# Add libraries to the project
add_subdirectory(include/argh)
target_link_libraries(dauw argh)

add_subdirectory(include/fmt)
target_link_libraries(dauw fmt::fmt)
