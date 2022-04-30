#include "parser.h"


namespace dauw
{
  // Constructor for the parser
  Parser::Parser(std::deque<Token> tokens)
    : tokens_(tokens)
  {
    // Advance to the first token
    advance();
  }

  // Parse a deque of tokens into an expression
  Expr* Parser::parse()
  {
    return parse_expression();
  }

  // Parse an expression
  Expr* Parser::parse_expression()
  {
    return parse_operation();
  }

  // Parse an operation expression
  Expr* Parser::parse_operation()
  {
    // operation → logic_or
    return parse_logic_or();
  }

  // Parse a logic or expression
  Expr* Parser::parse_logic_or()
  {
    // logic_or → logic_and ('or' logic_and)*
    auto expr = parse_logic_and();
    while (match("keyword_or"))
    {
      auto op_token = previous();
      auto right = parse_logic_and();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a logic and expression
  Expr* Parser::parse_logic_and()
  {
    // logic_and → logic_not ('and' logic_not)*
    auto expr = parse_logic_not();
    while (match("keyword_and"))
    {
      auto op_token = previous();
      auto right = parse_logic_not();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a logic not expression
  Expr* Parser::parse_logic_not()
  {
    // logic_not → 'not' logic_not | equality
    if (match("keyword_not"))
    {
      auto op_token = previous();
      auto right = parse_logic_not();
      return new UnaryExpr(op_token, right);
    }

    return parse_equality();
  }

  // Parse an equality expression
  Expr* Parser::parse_equality()
  {
    // equality → comparison (('==' | '<>' | '~') comparison)*
    auto expr = parse_comparison();
    while (match({"operator_equal", "operator_not_equal", "operator_match"}))
    {
      auto op_token = previous();
      auto right = parse_comparison();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a comparison expression
  Expr* Parser::parse_comparison()
  {
    // comparison → threeway (('<' | '<=' | '>' | '>=' | '%%') threeway)*
    auto expr = parse_threeway();
    while (match({"operator_less", "operator_less_equal", "operator_greater", "operator_greater_equal", "operator_divisible"}))
    {
      auto op_token = previous();
      auto right = parse_threeway();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a threeway expression
  Expr* Parser::parse_threeway()
  {
    // threeway → range ('<=>' range)*
    auto expr = parse_range();
    while (match("operator_threeway"))
    {
      auto op_token = previous();
      auto right = parse_range();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a range expression
  Expr* Parser::parse_range()
  {
    // range → term ('..' term)?
    auto expr = parse_term();
    while (match("operator_range"))
    {
      auto op_token = previous();
      auto right = parse_term();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a term expression
  Expr* Parser::parse_term()
  {
    // term → factor (('+' | '-') factor)*
    auto expr = parse_factor();
    while (match({"operator_add", "operator_subtract"}))
    {
      auto op_token = previous();
      auto right = parse_factor();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse a factor expression
  Expr* Parser::parse_factor()
  {
    // factor → unary (('*' | '/' | '//' | '%') unary)*
    auto expr = parse_unary();
    while (match({"operator_multiply", "operator_divide", "operator_floor_divide", "operator_modulo"}))
    {
      auto op_token = previous();
      auto right = parse_unary();
      expr = new BinaryExpr(expr, op_token, right);
    }
    return expr;
  }

  // Parse an unary expression
  Expr* Parser::parse_unary()
  {
    // unary → ('-' | '#' | '$') unary | primary
    if (match({"operator_subtract", "operator_length", "operator_string"}))
    {
      auto op_token = previous();
      auto right = parse_primary();
      return new UnaryExpr(op_token, right);
    }

    return parse_primary();
  }

  // Parse a primary expression
  Expr* Parser::parse_primary()
  {
    // primary → atom (call_postfix | subscript_postfix | access_postfix)*
    auto expr = parse_atom();
    while (true)
    {
      if (match("parenthesis_left"))
        expr = parse_call_postfix(expr);
      else if(match("square_bracket_left"))
        expr = parse_subscript_postfix(expr);
      else if(match("symbol_dot"))
        expr = parse_access_postfix(expr);
      else
        break;
    }
    return expr;
  }

  // Parse a call postfix
  Expr* Parser::parse_call_postfix(Expr* callee)
  {
    // call_postfix → '(' arguments? ')'
    // arguments → expression (',' expression)*
    std::vector<Expr*> arguments;
    if (!check("parenthesis_right"))
    {
      do {
        arguments.push_back(parse_expression());
      } while (match("symbol_comma"));
    }

    auto end_token = consume("parenthesis_right", "Expected ')' after call arguments");
    return new CallExpr(callee, end_token, arguments);
  }

  // Parse a subscript postfix
  Expr* Parser::parse_subscript_postfix(Expr* callee)
  {
    // subscript_postfix → '[' arguments? ']'
    // arguments → expression (',' expression)*
    std::vector<Expr*> arguments;
    if (!check("square_bracket_right"))
    {
      do {
        arguments.push_back(parse_expression());
      } while (match("symbol_comma"));
    }

    auto end_token = consume("square_bracket_right", "Expected ']' after subscript arguments");
    return new SubscriptExpr(callee, end_token, arguments);
  }

  // Parse a access postfix
  Expr* Parser::parse_access_postfix(Expr* callee)
  {
    // access_postfix → '.' IDENTIFIER
    auto name_token = consume("identifier", "Expected identifier after accessor");
    return new GetExpr(callee, name_token);
  }

  // Parse an atom expression
  Expr* Parser::parse_atom()
  {
    // atom → literal | name | list | record | lambda | parenthesized

    // literal → 'nothing' | 'false' | 'true' | INT | REAL | STRING
    if (match("keyword_nothing"))
      return new LiteralExpr("nothing", previous().location());
    if (match("keyword_false"))
      return new LiteralExpr("false", previous().location());
    if (match("keyword_true"))
      return new LiteralExpr("true", previous().location());
    if (match({"int", "real", "rune", "string", "regex"}))
      return new LiteralExpr(previous().value(), previous().location());

    // name → IDENTIFIER
    if (match("identifier"))
      return new NameExpr(previous());

    // list → '[' (list_item (',' list_item)*)? ']'
    // list_item → expression

    // record → '{' (record_item (',' record_item)*)? '}'
    // record_item → IDENTIFIER ':' expression

    // lambda → '\' '(' parameters? ')' '=' expression

    // parenthesized → '(' expression ')'
    if (match("parenthesis_left"))
    {
      auto expr = parse_expression();
      consume("parenthesis_right", "Expected ')' after expression");
      return new ParenthesizedExpr(expr);
    }

    throw Error("Expected expression", current().location());
  }

  // Advance to the next token and return the previous token
  Token Parser::advance()
  {
    if (!at_end())
      index_ ++;
    return previous();
  }

  // Return the current token
  Token Parser::current()
  {
    return tokens_[index_];
  }

  // Return the previous token
  Token Parser::previous()
  {
    return tokens_[index_ - 1];
  }

  // Return if the parser has reaced the end of the tokens
  bool Parser::at_end()
  {
    return current().name() != "eof";
  }

  // Check if the current token has the specified name
  bool Parser::check(std::string name)
  {
    return at_end() ? false : current().name() == name;
  }

  // Check if the current token has the specified name and advance if so
  bool Parser::match(std::string name)
  {
    if (!check(name))
      return false;

    advance();
    return true;
  }
  bool Parser::match(const std::initializer_list<std::string> names)
  {
    for (auto name : names)
    {
      if (check(name))
      {
        advance();
        return true;
      }
    }

    return false;
  }

  Token Parser::consume(std::string name, std::string message)
  {
    if (check(name))
      return advance();

    throw Error(message, current().location());
  }
}
