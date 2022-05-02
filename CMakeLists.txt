cmake_minimum_required (VERSION 3.13)
project(dauw)

# Set language standards
set(CMAKE_CXX_STANDARD 20)

# Add sources to the project
file(GLOB_RECURSE DAUW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE DAUW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(dauw ${DAUW_HEADERS} ${DAUW_SOURCES})

# Add libraries to the project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/argh)
target_link_libraries(dauw argh)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/fmt)
target_link_libraries(dauw fmt::fmt)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-linenoise)
target_link_libraries(dauw linenoise)
