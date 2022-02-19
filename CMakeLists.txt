cmake_minimum_required (VERSION 3.13)
project ("dauw")

# Add sources to the project
add_executable (dauw
  "src/main.cpp"
  "src/debug.h"
  "src/debug.cpp"
  "src/utils.h"
  "src/utils.cpp"

  "src/core/errors.h"
  "src/core/errors.cpp"

  "src/parser/lexer.h"
  "src/parser/lexer.cpp"
  "src/parser/location.h"
  "src/parser/location.cpp"
)

# Add libraries to the project
find_package(fmt)
target_link_libraries(dauw PRIVATE fmt::fmt-header-only)
