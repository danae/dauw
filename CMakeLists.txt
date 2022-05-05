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
file(GLOB_RECURSE DAUW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE DAUW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${DAUW_HEADERS} ${DAUW_SOURCES})

# Add libraries to the project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/argh)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-linenoise)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/utfcpp)

target_link_libraries(${PROJECT_NAME}
  PRIVATE argh
  PRIVATE fmt::fmt
  PRIVATE linenoise
  PRIVATE utf8cpp)

# Add compile definitions to the project
target_compile_definitions(${PROJECT_NAME}
  PRIVATE "DAUW_GIT_BRANCH=\"${GIT_BRANCH}\""
  PRIVATE "DAUW_GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")

# Add include directories to the project
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
