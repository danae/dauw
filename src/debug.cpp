#include "debug.h"


// Format a lexer location
std::string dauw::formatLexerLocation(dauw::Location location)
{
  return fmt::format("line {}, col {}", location.line + 1, location.col + 1);
}

// Format a lexer token
std::string dauw::formatLexerToken(dauw::LexerToken token)
{
  if (!token.value.empty())
    return fmt::format("{} \"{}\" at {}", token.name, token.value, formatLexerLocation(token.location));
  else
    return fmt::format("{} at {}", token.name, formatLexerLocation(token.location));
}
