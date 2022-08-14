#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/utils/string.hpp>


namespace dauw::debug
{
  // Print an indentation string
  void print_indent(size_t indent);

  // Print a list of tokens
  void print_tokens(Lexer::token_list_type tokens);
}
