#include "debug.hpp"

namespace dauw::debug
{
  // Print an indentation string
  void print_indent(size_t indent)
  {
    fmt::print("{}", utils::repeat("  ", indent));
  }

  // Print a list of tokens
  void print_tokens(frontend::Lexer::token_list_type tokens)
  {
    int indent = 0;
    for (auto token : tokens)
    {
      // Check the kind of the token
      switch (token.kind())
      {
        // Print a NEWLINE token
        case frontend::TokenKind::NEWLINE:
          print_indent(indent);
          fmt::print(fmt::fg(fmt::color::light_green), "{}\n", token);
          break;

        // Print an INDENT token
        case frontend::TokenKind::INDENT:
          print_indent(indent);
          fmt::print(fmt::fg(fmt::color::light_green), "{}{}\n", token);
          indent ++;
          break;

        // Print a DEDENT token
        case frontend::TokenKind::DEDENT:
          indent --;
          print_indent(indent);
          fmt::print(fmt::fg(fmt::color::light_green), "{}\n", token);
          break;

        // Print an END token
        case frontend::TokenKind::END:
          print_indent(indent);
          fmt::print(fmt::fg(fmt::color::crimson), "{}\n", token);
          break;

        // Print a COMMENT token
        case frontend::TokenKind::COMMENT:
          print_indent(indent);
          fmt::print(fmt::fg(fmt::color::gray), "{}\n", token);
          break;

        // Print another kind of token
        default:
          print_indent(indent);
          fmt::print("{}\n", token);
          break;
      }
    }
  }
}
