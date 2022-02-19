#pragma once

#include <string>
#include <fmt/core.h>

#include "parser/lexer.h"


namespace dauw
{
  // Format a lexer location
  std::string formatLexerLocation(Location location);

  // Format a lexer token
  std::string formatLexerToken(LexerToken token);
}
