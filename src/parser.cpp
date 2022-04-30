#include "parser.h"


namespace dauw
{
  // Parse a deque of tokens into an expression
  Expr* Parser::parse(std::deque<Token> tokens)
  {
    Expr* expr = new LiteralExpr(tokens[0]);
    for (auto i = 1; i < tokens.size(); i ++)
      expr = new NestedExpr(tokens[i].name(), expr, new LiteralExpr(tokens[i]));
    return expr;
  }
}
