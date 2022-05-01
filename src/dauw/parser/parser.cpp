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
  std::shared_ptr<Expr> Parser::parse()
  {
    // script → (expression 'newline')* 'eof'
    std::vector<std::shared_ptr<Expr>> exprs;
    while (!at_end())
    {
      exprs.push_back(parse_expression());
      consume("newline", "Expected newline after expression");
    }
    return std::make_shared<ExprBlock>(exprs);
  }

  // Parse an expression
  std::shared_ptr<Expr> Parser::parse_expression()
  {
    // expression → operation 'newline'
    return parse_operation();
  }

  // Parse an operation expression
  std::shared_ptr<Expr> Parser::parse_operation()
  {
    // operation → logic_or
    return parse_logic_or();
  }

  // Parse a logic or expression
  std::shared_ptr<Expr> Parser::parse_logic_or()
  {
    // logic_or → logic_and ('or' logic_and)*
    auto expr = parse_logic_and();
    while (match("operator_logical_or"))
    {
      auto op_token = current_;
      auto right = parse_logic_and();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a logic and expression
  std::shared_ptr<Expr> Parser::parse_logic_and()
  {
    // logic_and → logic_not ('and' logic_not)*
    auto expr = parse_logic_not();
    while (match("operator_logical_and"))
    {
      auto op_token = current_;
      auto right = parse_logic_not();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a logic not expression
  std::shared_ptr<Expr> Parser::parse_logic_not()
  {
    // logic_not → 'not' logic_not | equality
    if (match("operator_logical_not"))
    {
      auto op_token = current_;
      auto right = parse_logic_not();
      return std::make_shared<ExprUnary>(op_token, right);
    }

    return parse_equality();
  }

  // Parse an equality expression
  std::shared_ptr<Expr> Parser::parse_equality()
  {

    // equality → comparison (('==' | '<>' | '~') comparison)*
    auto expr = parse_comparison();
    while (match({"operator_equal", "operator_not_equal", "operator_match"}))
    {
      auto op_token = current_;
      auto right = parse_comparison();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a comparison expression
  std::shared_ptr<Expr> Parser::parse_comparison()
  {
    // comparison → threeway (('<' | '<=' | '>' | '>=' | '%%') threeway)*
    auto expr = parse_threeway();
    while (match({"operator_less", "operator_less_equal", "operator_greater", "operator_greater_equal", "operator_divisible"}))
    {
      auto op_token = current_;
      auto right = parse_threeway();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a threeway expression
  std::shared_ptr<Expr> Parser::parse_threeway()
  {
    // threeway → range ('<=>' range)*
    auto expr = parse_range();
    while (match("operator_threeway"))
    {
      auto op_token = current_;
      auto right = parse_range();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a range expression
  std::shared_ptr<Expr> Parser::parse_range()
  {
    // range → term ('..' term)?
    auto expr = parse_term();
    while (match("operator_range"))
    {
      auto op_token = current_;
      auto right = parse_term();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a term expression
  std::shared_ptr<Expr> Parser::parse_term()
  {
    // term → factor (('+' | '-') factor)*
    auto expr = parse_factor();
    while (match({"operator_add", "operator_subtract"}))
    {
      auto op_token = current_;
      auto right = parse_factor();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse a factor expression
  std::shared_ptr<Expr> Parser::parse_factor()
  {
    // factor → unary (('*' | '/' | '//' | '%') unary)*
    auto expr = parse_unary();
    while (match({"operator_multiply", "operator_divide", "operator_floor_divide", "operator_modulo"}))
    {
      auto op_token = current_;
      auto right = parse_unary();
      expr = std::make_shared<ExprBinary>(expr, op_token, right);
    }
    return expr;
  }

  // Parse an unary expression
  std::shared_ptr<Expr> Parser::parse_unary()
  {
    // unary → ('-' | '#' | '$') unary | primary
    if (match({"operator_subtract", "operator_length", "operator_string"}))
    {
      auto op_token = current_;
      auto right = parse_primary();
      return std::make_shared<ExprUnary>(op_token, right);
    }

    return parse_primary();
  }

  // Parse a primary expression
  std::shared_ptr<Expr> Parser::parse_primary()
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
  std::shared_ptr<Expr> Parser::parse_call_postfix(std::shared_ptr<Expr> callee)
  {
    // call_postfix → '(' arguments? ')'
    // arguments → expression (',' expression)*
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check("parenthesis_right"))
    {
      do {
        arguments.push_back(parse_expression());
      } while (match("symbol_comma"));
    }

    auto end_token = consume("parenthesis_right", "Expected ')' after call arguments");
    return std::make_shared<ExprCall>(callee, end_token, arguments);
  }

  // Parse a subscript postfix
  std::shared_ptr<Expr> Parser::parse_subscript_postfix(std::shared_ptr<Expr> callee)
  {
    // subscript_postfix → '[' arguments? ']'
    // arguments → expression (',' expression)*
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check("square_bracket_right"))
    {
      do {
        arguments.push_back(parse_expression());
      } while (match("symbol_comma"));
    }

    auto end_token = consume("square_bracket_right", "Expected ']' after subscript arguments");
    return std::make_shared<ExprSubscript>(callee, end_token, arguments);
  }

  // Parse a access postfix
  std::shared_ptr<Expr> Parser::parse_access_postfix(std::shared_ptr<Expr> callee)
  {
    // access_postfix → '.' IDENTIFIER
    auto name_token = consume("identifier", "Expected identifier after accessor");
    return std::make_shared<ExprGet>(callee, name_token);
  }

  // Parse an atom expression
  std::shared_ptr<Expr> Parser::parse_atom()
  {
    // atom → literal | name | list | record | lambda | parenthesized

    // literal → 'nothing' | 'false' | 'true' | INT | REAL | STRING
    if (match("keyword_nothing"))
      return std::make_shared<ExprLiteral>("nothing", current_.location());
    if (match("keyword_false"))
      return std::make_shared<ExprLiteral>("false", current_.location());
    if (match("keyword_true"))
      return std::make_shared<ExprLiteral>("true", current_.location());
    if (match({"int", "real", "rune", "string", "regex"}))
      return std::make_shared<ExprLiteral>(current_.value(), current_.location());

    // name → IDENTIFIER
    if (match("identifier"))
      return std::make_shared<ExprName>(current_);

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
      return std::make_shared<ExprParenthesized>(expr);
    }

    throw Error("Expected expression", next_.location());
  }


  // Advance to the next token and return that token
  Token Parser::advance()
  {
    if (!at_end())
    {
      current_ = next_;
      next_ = tokens_.front();
      tokens_.pop_front();

      if (current_.name() == "error")
        throw Error(current_.value(), current_.location());
    }

    return current_;
  }

  // Return if the parser has reaced the end of the tokens
  bool Parser::at_end()
  {
    return next_.name() == "eof";
  }

  // Check if the next token has the specified name
  bool Parser::check(string_t name)
  {
    return at_end() ? false : next_.name() == name;
  }

  // Check if the next token has the specified name and advance if so
  bool Parser::match(string_t name)
  {
    if (!check(name))
      return false;

    advance();
    return true;
  }

  // Check if the next token has one of the specified names and advance if so
  bool Parser::match(const std::initializer_list<string_t> names)
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

  // Consume the next token and fail if it doesn't have the specified name
  Token Parser::consume(string_t name, string_t message)
  {
    if (check(name))
      return advance();

    throw Error(message, next_.location());
  }
}
