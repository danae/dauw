#include "source_parser.h"

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
      return std::make_shared<ExprLiteral>(value_nothing(), current_.location());
    if (match("keyword_false"))
      return std::make_shared<ExprLiteral>(value_false(), current_.location());
    if (match("keyword_true"))
      return std::make_shared<ExprLiteral>(value_true(), current_.location());
    if (match("int"))
      return parse_int();
    if (match("real"))
      return parse_real();
    if (match("rune"))
      return parse_rune();
    if (match("string"))
      return parse_string();
    if (match("regex"))
      return parse_regex();

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

    throw SyntaxError(next_.location(), "Expected atom");
  }

  // Parse an int literal
  std::shared_ptr<Expr> Parser::parse_int()
  {
    auto value = current_.value();

    try
    {
      // Sanitize the int literal
      value = regex_replace(regex_t("_"), "", value);

      // Parse the int literal
      size_t end_index;
      auto is_hex = (value.rfind("0x", 0) == 0 || value.rfind("0X", 0) == 0);
      auto int_value = (int64_t)std::stoll(value, &end_index, is_hex ? 16 : 10);
      if (end_index != value.length())
        throw SyntaxError(current_.location(), fmt::format("Invalid character '{}' in int literal", value, value.substr(end_index, 1)));

      // Return the expression
      return std::make_shared<ExprLiteral>(value_of_int(int_value), current_.location());
    }
    catch (std::invalid_argument& ex) {
      throw SyntaxError(current_.location(), ex.what());
    }
    catch (std::out_of_range& ex) {
      throw SyntaxError(current_.location(), ex.what());
    }
  }

  // Parse a real literal
  std::shared_ptr<Expr> Parser::parse_real()
  {
    auto value = current_.value();

    try
    {
      // Sanitize the int literal
      value = regex_replace(regex_t("_"), "", value);

      // Parse the real literal
      size_t end_index;
      double real_value = std::stod(value, &end_index);
      if (end_index != value.length())
        throw SyntaxError(current_.location(), fmt::format("Invalid character '{}' in real literal", value, value.substr(end_index, 1)));

      // Return the expression
      return std::make_shared<ExprLiteral>(value_of_real(real_value), current_.location());
    }
    catch (std::invalid_argument& ex) {
      throw SyntaxError(current_.location(), ex.what());
    }
    catch (std::out_of_range& ex) {
      throw SyntaxError(current_.location(), ex.what());
    }
  }

  // Parse a rune literal
  std::shared_ptr<Expr> Parser::parse_rune()
  {
    auto value = current_.value();

    try
    {
      // Parse the rune literal
      auto runes = string_to_runes(string_unescape(value));
      if (runes.size() < 1)
        throw SyntaxError(current_.location(), "The rune literal contains no code point");
      if (runes.size() > 1)
        throw SyntaxError(current_.location(), "The rune literal contains more than one code point");

      rune_t rune_value = (rune_t)runes.front();

      // Return the expression
      return std::make_shared<ExprLiteral>(value_of_rune(rune_value), current_.location());
    }
    catch (std::out_of_range& ex) {
      throw SyntaxError(current_.location(), ex.what());
    }
  }

  // Parse a string literal
  std::shared_ptr<Expr> Parser::parse_string()
  {
    auto value = string_unescape(current_.value());
    return std::make_shared<ExprLiteral>(value_ptr_from_string(value), current_.location());
  }

  // Parse a regex literal
  std::shared_ptr<Expr> Parser::parse_regex()
  {
    // TODO: Implement actual regex conversion
    auto value = string_unescape(current_.value());
    return std::make_shared<ExprLiteral>(value_ptr_from_string(value), current_.location());
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
        throw SyntaxError(current_.location(), current_.value());
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

    throw SyntaxError(next_.location(), message);
  }
}
