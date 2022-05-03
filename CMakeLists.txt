cmake_minimum_required (VERSION 3.13)
project(dauw)

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the latest commit hash
execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set language standards
set(CMAKE_CXX_STANDARD 20)

# Add sources to the project
file(GLOB_RECURSE DAUW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE DAUW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${DAUW_HEADERS} ${DAUW_SOURCES})

# Add libraries to the project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/argh)
target_link_libraries(${PROJECT_NAME} argh)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-linenoise)
target_link_libraries(${PROJECT_NAME} linenoise)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt)

# Add compile definitions to the project
target_compile_definitions(${PROJECT_NAME}
  PRIVATE "DAUW_GIT_BRANCH=\"${GIT_BRANCH}\""
  PRIVATE "DAUW_GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
