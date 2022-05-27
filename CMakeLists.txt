cmake_minimum_required (VERSION 3.13)

# Create the project
project(dauw)

# Get the current working branch
execute_process(COMMAND git rev-parse --abbrev-ref HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "${PROJECT_NAME}: Current Git branch is ${GIT_BRANCH}")

# Get the latest commit hash
execute_process(COMMAND git rev-parse --short HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GIT_COMMIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "${PROJECT_NAME}: Current Git commit hash is ${GIT_COMMIT_HASH}")

# Set language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add sources to the project
file(GLOB_RECURSE DAUW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE DAUW_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${DAUW_HEADERS} ${DAUW_SOURCES})

# Add libraries to the project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/argh ${CMAKE_CURRENT_BINARY_DIR}/lib/argh)
target_link_libraries(${PROJECT_NAME} PRIVATE argh)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cpp-linenoise  ${CMAKE_CURRENT_BINARY_DIR}/lib/cpp-linenoise)
target_link_libraries(${PROJECT_NAME} PRIVATE linenoise)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fmt ${CMAKE_CURRENT_BINARY_DIR}/lib/fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)

set(PCRE2_BUILD_PCRE2GREP OFF)
set(PCRE2_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/pcre2 ${CMAKE_CURRENT_BINARY_DIR}/lib/pcre2)
target_link_libraries(${PROJECT_NAME} PRIVATE pcre2-8)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/lib/pcre2)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/utfcpp ${CMAKE_CURRENT_BINARY_DIR}/lib/utfcpp)
target_link_libraries(${PROJECT_NAME} PRIVATE utf8cpp)

# Add compile definitions to the project
target_compile_definitions(${PROJECT_NAME}
  PRIVATE "PCRE2_CODE_UNIT_WIDTH=32"
  PRIVATE "DAUW_GIT_BRANCH=\"${GIT_BRANCH}\""
  PRIVATE "DAUW_GIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")

# Add include directories to the project
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
