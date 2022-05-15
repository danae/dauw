#include "parser.hpp"

namespace dauw
{
  // Constructor for the parser
  Parser::Parser(VM* vm, ErrorReporter* error_reporter, std::deque<Token> tokens)
    : vm_(vm), error_reporter_(error_reporter), tokens_(tokens)
  {
    // Advance to the first token
    advance();
  }

  // Parse a deque of tokens into an expression
  expr_ptr Parser::parse()
  {
    // Parse the script
    try
    {
      return parse_script();
    }
    catch (SyntaxError& ex)
    {
      return nullptr;
    }
  }

  // Parse a script
  // script → line* EOF
  expr_ptr Parser::parse_script()
  {
    // Create a vector to store expressions
    expr_ptr_vector exprs;

    // Parse lines until we reach an EOF token
    while (!at_end())
      exprs.push_back(parse_line());

    // Return a block expresssion containing the expressions
    return std::make_shared<ExprBlock>(exprs);
  }

  // Parse a line
  // line → (COMMENT | expression COMMENT?) NEWLINE
  expr_ptr Parser::parse_line()
  {
    try
    {
      // Check comments at the start of the line
      last_line_comment_ = "";
      while (match("comment"))
      {
        if (!last_line_comment_.empty())
          last_line_comment_ += "\n";
        last_line_comment_ += current_.value();

        consume("newline", "after comment");
      }

      // Parse the line as an expression
      auto expr = parse_expression();

      // Check for a comment at the end of the line, and ignore it if so
      if (check("comment"))
        advance();

      // Consume a newline token
      consume("newline", "after expression");

      // Return the expression
      return expr;
    }
    catch (SyntaxError& ex)
    {
      synchronize();
      return nullptr;
    }
  }

  // Parse an expression
  // expression → def | assignment
  expr_ptr Parser::parse_expression()
  {
    // Check for a def declaration
    if (match("keyword_def"))
      return parse_def();

    // Parse an assignment expression
    return parse_assignment();
  }

  // Parse a def declaration
  // def → 'def' IDENTIFIER ('(' parameters ')')? (':' simple)? '=' assignment
  expr_ptr Parser::parse_def()
  {
    // Set the keyword
    auto keyword = current_;

    // Parse the identifier
    auto name = consume("identifier", "in def declaration");

    // Check for the parameters
    if (match("parenthesis_left"))
    {
      // The def declaration is a function declaration

      // Set the token
      auto token = current_;

      // Parse the parameters
      auto parameters = parse_parameters();

      // Parse the return type
      std::optional<expr_ptr> return_type = std::nullopt;
      if (match("symbol_colon"))
        return_type = std::make_optional(parse_simple());

      // Parse the body
      consume("operator_assign", "in def declaration");
      auto body = parse_assignment();

      // Return the declaration
      // TODO: Add the proper function type
      auto function = std::make_shared<ExprFunction>(token, parameters, return_type, body);
      return std::make_shared<ExprDef>(name, std::nullopt, function);
    }
    else
    {
      // The def declaration is a name declaration

      // Parse the type
      std::optional<expr_ptr> type = std::nullopt;
      if (match("symbol_colon"))
        type = std::make_optional(parse_simple());

      // Parse the value
      consume("operator_assign", "in def declaration");
      auto value = parse_assignment();

      // Return the declaration
      return std::make_shared<ExprDef>(name, type, value);
    }
  }

  // Parse an assignment expression
  // assignment → (call '.')? IDENTIFIER (':' simple)? '=' assignment | control
  expr_ptr Parser::parse_assignment()
  {
    // TODO: Parse an assignment expression

    // Parse a control expression
    return parse_control();
  }

  // Parse a control flow expression
  // control → if | for | while | until | block | simple
  expr_ptr Parser::parse_control()
  {
    // Check for an if expression
    if (match("keyword_if"))
      return parse_if();

    // Check for an if expression
    if (match("keyword_for"))
      return parse_for();

    // Check for an if expression
    if (match("keyword_while"))
      return parse_while();

    // Check for an if expression
    if (match("keyword_until"))
      return parse_until();

    // Check for a block expression
    if (match("newline"))
      return parse_block();

    // Parse a simple expression
    return parse_simple();
  }

  // Parse an if expression
  // if → 'if' simple 'then' expression ('else' expression)?
  expr_ptr Parser::parse_if()
  {
    // Set the keyword
    auto keyword = current_;

    // Parse the condition
    auto condition = parse_simple();

    // Parse the true branch
    consume("keyword_then", "in if expression");
    auto true_branch = parse_expression();

    // Parse the false branch
    std::optional<expr_ptr> false_branch = std::nullopt;
    if (match("keyword_else"))
      false_branch = std::make_optional(parse_expression());

    // Return the expression
    return std::make_shared<ExprIf>(keyword, condition, true_branch, false_branch);
  }

  // Parse a for expression
  // for → 'for' IDENTIFIER 'in' simple 'do' expression
  expr_ptr Parser::parse_for()
  {
    // Set the keyword
    auto keyword = current_;

    // Parse the name
    auto name = consume("identifier", "in for expression");

    // Parse the iterable
    consume("keyword_in", "in for expression");
    auto iterable = parse_simple();

    // Parse the body
    consume("keyword_do", "in for expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprFor>(keyword, name, iterable, body);
  }

  // Parse a while expression
  // while → 'while' assignment 'do' expression
  expr_ptr Parser::parse_while()
  {
    // Set the keyword
    auto keyword = current_;

    // Parse the condition
    auto condition = parse_simple();

    // Parse the body
    consume("keyword_do", "in while expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprWhile>(keyword, condition, body);
  }

  // Parse an until expression
  // until → 'until' assignment 'do' expression
  expr_ptr Parser::parse_until()
  {
    // Set the keyword
    auto keyword = current_;

    // Parse the condition
    auto condition = parse_simple();

    // Parse the body
    consume("keyword_do", "in until expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprUntil>(keyword, condition, body);
  }

  // Parse a block expression
  // block → NEWLINE INDENT line+ DEDENT
  expr_ptr Parser::parse_block()
  {
    // Set the token to the current NEWLINE token
    auto token = current_;

    // Consume an INDENT token
    consume("indent", "in block");

    // Create a vector to store expressions
    expr_ptr_vector exprs;

    // Parse lines until we reach a DEDENT token
    do
    {
      // Parse a line
      exprs.push_back(parse_line());
    } while (!match("dedent"));

    // Return a block expresssion containing the expressions
    return std::make_shared<ExprBlock>(exprs);
  }

  // Parse an simple expression
  // simple → logic_or
  expr_ptr Parser::parse_simple()
  {
    return parse_logic_or();
  }

  // Parse a logic or expression
  // logic_or → logic_and ('or' logic_and)*
  expr_ptr Parser::parse_logic_or()
  {
    return parse_as_binary({"operator_logical_or"}, &Parser::parse_logic_and);
  }

  // Parse a logic and expression
  // logic_and → logic_not ('and' logic_not)*
  expr_ptr Parser::parse_logic_and()
  {
    return parse_as_binary({"operator_logical_and"}, &Parser::parse_logic_not);
  }

  // Parse a logic not expression
  // logic_not → 'not' logic_not | equality
  expr_ptr Parser::parse_logic_not()
  {
    return parse_as_unary({"operator_logical_not"}, &Parser::parse_equality);
  }

  // Parse an equality expression
  // equality → comparison (('==' | '<>' | '~') comparison)*
  expr_ptr Parser::parse_equality()
  {
    return parse_as_binary({"operator_equal", "operator_not_equal", "operator_match"}, &Parser::parse_comparison);
  }

  // Parse a comparison expression
  // comparison → threeway (('<' | '<=' | '>' | '>=' | '%%') threeway)*
  expr_ptr Parser::parse_comparison()
  {
    return parse_as_binary({"operator_less", "operator_less_equal", "operator_greater", "operator_greater_equal", "operator_divisible"}, &Parser::parse_threeway);
  }

  // Parse a threeway expression
  // threeway → range ('<=>' range)*
  expr_ptr Parser::parse_threeway()
  {
    return parse_as_binary({"operator_threeway"}, &Parser::parse_range);
  }

  // Parse a range expression
  // range → term ('..' term)?
  expr_ptr Parser::parse_range()
  {
    return parse_as_single_binary({"operator_range"}, &Parser::parse_term);
  }

  // Parse a term expression
  // term → factor (('+' | '-') factor)*
  expr_ptr Parser::parse_term()
  {
    return parse_as_binary({"operator_add", "operator_subtract"}, &Parser::parse_factor);
  }

  // Parse a factor expression
  // factor → unary (('*' | '/' | '//' | '%') unary)*
  expr_ptr Parser::parse_factor()
  {
    return parse_as_binary({"operator_multiply", "operator_divide", "operator_floor_divide", "operator_modulo"}, &Parser::parse_unary);
  }

  // Parse an unary expression
  // unary → ('-' | '#' | '$') unary | primary
  expr_ptr Parser::parse_unary()
  {
    return parse_as_unary({"operator_subtract", "operator_length", "operator_string"}, &Parser::parse_primary);
  }

  // Parse a primary expression
  // primary → atom ('(' arguments? ')' | '.' IDENTIFIER)*
  expr_ptr Parser::parse_primary()
  {
    // Parse an atom
    auto expr = parse_atom();

    // Loop until the postfix checks fail
    while (true)
    {
      // Check for a call postfix
      if (match("parenthesis_left"))
      {
        // Set the token
        auto token = current_;

        // Parse the arguments
        auto arguments = std::make_shared<ExprSequence>(token, parse_arguments());

        // Create the call expression
        expr = std::make_shared<ExprCall>(expr, token, arguments);
      }

      // Check for a get postfix
      else if (match("symbol_dot"))
      {
        // Parse the name
        auto name = consume("identifier", "in get expression");

        // Return the expression
        expr = std::make_shared<ExprGet>(expr, name);
      }

      // Otherwise break the loop
      else
        break;
    }

    // Return the expression
    return expr;
  }

  // Parse an atom expression
  // atom → literal | name | list | record | lambda | grouped
  // literal → 'nothing' | 'false' | 'true' | INT | REAL | STRING
  // name → IDENTIFIER
  expr_ptr Parser::parse_atom()
  {
    try
    {
      // Check for a literal expression
      if (match("keyword_nothing"))
        return std::make_shared<ExprLiteral>(value_nothing, current_.location());
      if (match("keyword_false"))
        return std::make_shared<ExprLiteral>(value_false, current_.location());
      if (match("keyword_true"))
        return std::make_shared<ExprLiteral>(value_true, current_.location());
      if (match("int"))
        return std::make_shared<ExprLiteral>(value_int(current_.value()), current_.location());
      if (match("real"))
        return std::make_shared<ExprLiteral>(value_real(current_.value()), current_.location());
      if (match("rune"))
        return std::make_shared<ExprLiteral>(value_rune(current_.value()), current_.location());
      if (match("string"))
        return std::make_shared<ExprLiteral>(value_string(current_.value()), current_.location());
      if (match("regex"))
        return std::make_shared<ExprLiteral>(value_regex(current_.value()), current_.location());

      // Check for a name expression
      if (match("identifier"))
        return std::make_shared<ExprName>(current_);

      // Check for a sequence expression
      if (match("square_bracket_left"))
        return parse_sequence();

      // Check for a record expression
      if (match("curly_bracket_left"))
        return parse_record();

      // Check for a lambda expression
      if (match("symbol_lambda"))
        return parse_lambda();

      // Check for a grouped expression
      if (match("parenthesis_left"))
        return parse_grouped();

      // No suitable expression found, so throw an error
      throw error(next_, fmt::format("Expected atom, but found {}", next_.name()));
    }
    catch (std::invalid_argument& ex)
    {
      // Found a literal that is malformed
      throw error(current_, ex.what());
    }
    catch (std::out_of_range& ex)
    {
      // Found a literal whose value is out of range
      throw error(current_, ex.what());
    }
  }

  // Parse a sequence expression
  // sequence → '[' (sequence_item (',' sequence_item)*)? ']'
  // sequence_item → expression
  expr_ptr Parser::parse_sequence()
  {
    // Create a container to store the items
    ExprSequence::sequence_type items;

    // Set the opening token
    auto token = current_;

    // Parse expressions until we reach the closing token
    do
    {
      // Parse the value of the item
      items.push_back(parse_expression());
    } while (match("symbol_comma"));

    // Consume the closing token
    consume("square_bracket_right", "in sequence atom");

    // Return a sequence expression containing the items
    return std::make_shared<ExprSequence>(token, items);
  }

  // Parse a record expression
  // record → '{' (record_item (',' record_item)*)? '}'
  // record_item → IDENTIFIER ':' expression
  expr_ptr Parser::parse_record()
  {
    // Create a container to store the items
    ExprRecord::record_type items;

    // Set the opening token
    auto token = current_;

    // Parse expressions until we reach the closing token
    do
    {
      // Parse the name of the item
      auto name = consume("identifier", "in record atom").value();
      consume("symbol_colon", "in record atom");

      // Parse the value of the item
      items.insert(std::make_pair(name, parse_expression()));
    } while (match("symbol_comma"));

    // Consume the closing token
    consume("curly_bracket_right", "in record atom");

    // Return a record expression containing the items
    return std::make_shared<ExprRecord>(token, items);
  }

  // Parse a lambda expression
  // lambda → '\' '(' parameters? ')' '=' assignment
  expr_ptr Parser::parse_lambda()
  {
    // Set the token
    auto token = current_;

    // Parse the parameters
    consume("parenthesis_left", "in lambda atom");
    auto parameters = parse_parameters();

    // Parse the return type
    std::optional<expr_ptr> return_type = std::nullopt;
    if (match("symbol_colon"))
      return_type = std::make_optional(parse_simple());

    // Parse the body
    consume("operator_assign", "in lambda atom");
    auto body = parse_assignment();

    // Return the function expression
    return std::make_shared<ExprFunction>(token, parameters, return_type, body);
  }

  // Parse a grouped expression
  // grouped → '(' expression ')'
  expr_ptr Parser::parse_grouped()
  {
    // Parse the grouped expression
    auto expr = parse_expression();

    // Consume the closing parenthesis
    consume("parenthesis_right", "in grouped atom");

    // Return the grouped expression
    return std::make_shared<ExprGrouped>(expr);
  }

  // Parse parameters
  // parameters → (IDENTIFIER ':' simple) (',' IDENTIFIER ':' simple)*)?
  expr_name_ptr_vector Parser::parse_parameters()
  {
    // Create a vector to store the parameters
    expr_name_ptr_vector parameters;

    // Loop until we encounter a closing parenthesis
    if (!check("parenthesis_right"))
    {
      do
      {
        // Parse the name
        auto name = consume("identifier", "in parameters");
        consume("symbol_colon", "in parameters");

        // Parse the type
        auto type = parse_simple();

        // Create the expression and add it to the parameters
        parameters.push_back(std::make_shared<ExprName>(name, type));
      } while (match("symbol_comma"));
    }

    // Consume the closing parenthesis
    consume("parenthesis_right", "in parameters");

    // Return the parameters
    return parameters;
  }

  // Parse arguments
  // arguments → simple (',' simple)*
  expr_ptr_vector Parser::parse_arguments()
  {
    // Create a vector to store the arguments
    expr_ptr_vector arguments;

    // Loop until we encounter a closing parenthesis
    if (!check("parenthesis_right"))
    {
      do
      {
        // Parse the expression and add it to the arguments
        arguments.push_back(parse_simple());
      } while (match("symbol_comma"));
    }

    // Consume the closing parenthesis
    consume("parenthesis_right", "in arguments");

    // Return the arguments
    return arguments;
  }


  // Parse a binary expression
  expr_ptr Parser::parse_as_binary(std::initializer_list<string_t> op_names, parser_function_type parse_operand)
  {
    // Parse the left operand
    auto left = parse_operand(this);

    // Check if we match the binary operator
    while (match(op_names))
    {
      // Set the operator token
      auto op = current_;

      // Parse the right operand
      auto right = parse_operand(this);

      // Create the binary expression
      left = std::make_shared<ExprBinary>(left, op, right);
    }

    // Return the expression
    return left;
  }

  // Parse a single binary expression
  expr_ptr Parser::parse_as_single_binary(std::initializer_list<string_t> op_names, parser_function_type parse_operand)
  {
    // Parse the left operand
    auto left = parse_operand(this);

    // Check if we match the binary operator
    if (match(op_names))
    {
      // Set the operator token
      auto op = current_;

      // Parse the right operand
      auto right = parse_operand(this);

      // Return the binary expression
      return std::make_shared<ExprBinary>(left, op, right);
    }

    // Return the expression
    return left;
  }

  // Parse an unary expression
  expr_ptr Parser::parse_as_unary(std::initializer_list<string_t> op_names, parser_function_type parse_operand)
  {
    // Check if we match the unary operator
    if (match(op_names))
    {
      // Set the operand token
      auto op = current_;

      // Parse the operand as another unary expression
      auto right = parse_as_unary(op_names, parse_operand);

      // Return the unary expression
      return std::make_shared<ExprUnary>(op, right);
    }

    // Parse the operand
    return parse_operand(this);
  }


  // Parse an int literal
  Value Parser::value_int(string_t value)
  {
    // Remove thousand separators from the int literal
    value = regex_utils::replace(regex_t("_"), "", value);

    // Parse the int literal
    size_t end_index;
    auto int_is_hex = (value.rfind("0x", 0) == 0 || value.rfind("0X", 0) == 0);
    auto int_value = (int64_t)std::stoll(value, &end_index, int_is_hex ? 16 : 10);
    if (end_index != value.length())
      throw std::invalid_argument(fmt::format("Invalid character '{}' in int literal", value, value.substr(end_index, 1)));

    // Return the int value
    return Value::of_int(int_value);
  }

  // Parse a real literal
  Value Parser::value_real(string_t value)
  {
    // Remove thousand separators from the real literal
    value = regex_utils::replace(regex_t("_"), "", current_.value());

    // Parse the real literal
    size_t end_index;
    double real_value = std::stod(value, &end_index);
    if (end_index != value.length())
      throw std::invalid_argument(fmt::format("Invalid character '{}' in real literal", value, value.substr(end_index, 1)));

    // Return the value
    return Value::of_real(real_value);
  }

  // Parse a rune literal
  Value Parser::value_rune(string_t value)
  {
    // Unescape the rune literal
    value = string_utils::unescape(current_.value(), string_utils::EscapeType::RUNE);

    // Parse the rune literal
    auto runes = string_utils::to_runes(value);
    if (runes.size() < 1)
      throw std::invalid_argument("Invalid rune literal without code point");
    if (runes.size() > 1)
      throw std::invalid_argument("Invalid rune literal with more than one code point");
    auto rune_value = runes.front();

    // Return the value
    return Value::of_rune(rune_value);
  }

  // Parse a string literal
  Value Parser::value_string(string_t value)
  {
      // Unescape the string literal
      value = string_utils::unescape(value, string_utils::EscapeType::STRING);

      // Allocate the string value
      auto string_value = vm_->allocate_string(value.c_str());

      // Return the value
      return Value::of_obj(string_value, type_string);
  }

  // Parse a regex literal
  Value Parser::value_regex(string_t value)
  {
    // TODO: Properly parse the regex literal instead of making a literal string

    // Unescape the regex literal
    value = string_utils::unescape(value, string_utils::EscapeType::REGEX);

    // Allocate the string value
    auto string_value = vm_->allocate_string(value.c_str());

    // Return the value
    return Value::of_obj(string_value, type_string);
  }


  // Create and report a syntax error
  SyntaxError Parser::error(Token token, string_t message)
  {
    // Report the error
    error_reporter_->report(token.location(), fmt::format("ParserError: {}", message));

    // Return the error
    return SyntaxError(token.location(), message);
  }

  // Synchronize the parser after an error
  void Parser::synchronize()
  {
    // Advance the parser until it reaches an expression boundary
    advance();
    while (!at_end())
    {
      // Check for a newline token
      if (current_.name() == "newline")
        break;

      // Advance the parser
      advance();
    }
  }


  // Return if the parser has reaced the end of the tokens
  bool Parser::at_end()
  {
    return next_.name() == "eof";
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
        throw error(current_, current_.value());
    }

    return current_;
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

  // Consume the next token or report an error if it doesn't have the specified name
  Token Parser::consume(string_t name, string_t context)
  {
    if (check(name))
      return advance();
    else if (!context.empty())
      throw error(next_, fmt::format("Expected {} {}, but found {}", name, context, next_.name()));
    else
      throw error(next_, fmt::format("Expected {}, but found {}", name, next_.name()));
  }
}
