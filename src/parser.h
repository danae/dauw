#pragma once

#include <deque>
#include <functional>
#include <string>
#include <vector>

#include "ast.h"
#include "common.h"
#include "token.h"


namespace dauw
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser
  {
    private:
      Token advance();

    public:
      // Parse a deque of tokens into an expression
      Expr* parse(std::deque<Token> tokens);
  };
}
