#include "parser.hpp"

namespace dauw::frontend
{
  // Constructor for the parser
  Parser::Parser(Reporter* reporter, Lexer::token_list_type tokens)
    : ReporterAware(reporter), tokens_(tokens)
  {
    index_ = 0;
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

  // --------------------------------------------------------------------------
  // BASIC PARSER FUNCTIONALITY
  // --------------------------------------------------------------------------

  // Return the current token
  Token Parser::current()
  {
    return tokens_[index_ - 1];
  }

  // Return the token past the current token
  Token Parser::next()
  {
    return tokens_[index_];
  }

  // Return if the parser reached the end of the tokens
  bool Parser::at_end()
  {
    return next().kind() == TokenKind::END;
  }

  // Advance to the next token and return that token
  Token Parser::advance()
  {
    if (!at_end())
      index_ ++;
    return current();
  }

  // Check if the next token has the specified kind
  bool Parser::check(TokenKind kind)
  {
    return at_end() ? false : next().kind() == kind;
  }

  // Check if the next token has the specified name and advance if so
  bool Parser::match(TokenKind kind)
  {
    if (check(kind))
    {
      advance();
      return true;
    }

    return false;
  }

  // Check if the next token has one of the specified names and advance if so
  bool Parser::match(const std::initializer_list<TokenKind> kinds)
  {
    for (auto kind : kinds)
    {
      if (check(kind))
      {
        advance();
        return true;
      }
    }

    return false;
  }

  // Consume the next token or report an error if it doesn't have the specified name
  Token Parser::consume(TokenKind kind, string_t context)
  {
    if (match(kind))
      return current();
    else if (!context.empty())
      throw report<SyntaxError>(next().location(), fmt::format("Expected {} {}, but found {}", kind, context, next().kind()));
    else
      throw report<SyntaxError>(next().location(), fmt::format("Expected {}, but found {}", kind, next().kind()));
  }

  // Synchronize the parser after an error
  void Parser::synchronize()
  {
    // Advance the parser until it reaches an expression boundary
    advance();
    while (!at_end())
    {
      // Check for a newline token
      if (current().kind() == TokenKind::NEWLINE)
        break;

      // Advance the parser
      advance();
    }
  }

  // --------------------------------------------------------------------------
  // PARSER HELPER FUNCTIONS
  // --------------------------------------------------------------------------

  // Parse an infix operation
  expr_ptr Parser::parse_infix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand)
  {
    auto left = parse_operand(this);
    while (match(op_kinds))
    {
      auto op = current();
      auto right = parse_operand(this);
      left = std::make_shared<ExprBinary>(left, op, right);
    }
    return left;
  }

  // Parse an infix operation that is not allowed to chain
  expr_ptr Parser::parse_infix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand)
  {
    auto left = parse_operand(this);
    if (match(op_kinds))
    {
      auto op = current();
      auto right = parse_operand(this);
      return std::make_shared<ExprBinary>(left, op, right);
    }
    return left;
  }

  // Parse a prefix operation
  expr_ptr Parser::parse_prefix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand)
  {
    if (match(op_kinds))
    {
      auto op = current();
      auto right = parse_prefix_op(op_kinds, parse_operand);
      return std::make_shared<ExprUnary>(op, right);
    }
    return parse_operand(this);
  }

  // Parse a prefix operation that is not allowed to chain
  expr_ptr Parser::parse_prefix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand)
  {
    if (match(op_kinds))
    {
      auto op = current();
      auto right = parse_operand(this);
      return std::make_shared<ExprUnary>(op, right);
    }
    return parse_operand(this);
  }

  // --------------------------------------------------------------------------
  // PARSERS FOR EXPRESSIONS
  // --------------------------------------------------------------------------

  // Parse a script
  // script → line* EOF
  expr_ptr Parser::parse_script()
  {
    // Create a vector to store expressions
    std::vector<expr_ptr> exprs;

    // Parse lines until we reach an END token
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
      // Check comments before the line
      line_comment_ = "";
      while (match(TokenKind::COMMENT))
      {
        if (!line_comment_.empty())
          line_comment_.append("\n");
        line_comment_.append(current().value());

        consume(TokenKind::NEWLINE, "after comment");
      }

      // Parse the line as an expression
      auto expr = parse_expression();

      // Check for a comment at the end of the line, and ignore it if so
      if (check(TokenKind::COMMENT))
        advance();

      // Consume a newline token or skip if the current token is a dedent token
      if (current().kind() != TokenKind::DEDENT)
        consume(TokenKind::NEWLINE, "after expression");

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
    if (match(TokenKind::KEYWORD_DEF))
      return parse_def();

    // Parse an assignment expression
    return parse_assignment();
  }

  // Parse a def declaration
  // def → 'def' IDENTIFIER ('(' parameters ')')? (':' type)? '=' assignment
  expr_ptr Parser::parse_def()
  {
    // Set the keyword
    auto keyword = current();

    // Parse the identifier
    auto name = consume(TokenKind::IDENTIFIER, "in def declaration");

    // Check for the parameters
    if (match(TokenKind::PARENTHESIS_LEFT))
    {
      // The def declaration is a function declaration

      // Set the token
      auto token = current();

      // Parse the parameters
      auto parameters = parse_parameters();

      // Parse the return type
      std::optional<type_expr_ptr> return_type = std::nullopt;
      if (match(TokenKind::SYMBOL_COLON))
        return_type = std::make_optional(parse_type());

      // Parse the body
      consume(TokenKind::OPERATOR_ASSIGN, "in def declaration");
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
      std::optional<type_expr_ptr> type = std::nullopt;
      if (match(TokenKind::SYMBOL_COLON))
        type = std::make_optional(parse_type());

      // Parse the value
      consume(TokenKind::OPERATOR_ASSIGN, "in def declaration");
      auto value = parse_assignment();

      // Return the declaration
      return std::make_shared<ExprDef>(name, type, value);
    }
  }

  // Parse an assignment expression
  // assignment → (call '.')? IDENTIFIER '=' assignment | control
  expr_ptr Parser::parse_assignment()
  {
    // TODO: Parse an assignment expression

    // Parse a control expression
    return parse_control();
  }

  // Parse a control flow expression
  // control → echo | if | for | while | until | block | operation
  expr_ptr Parser::parse_control()
  {
    // Check for an echo expression
    if (match(TokenKind::KEYWORD_ECHO))
      return parse_echo();

    // Check for an if expression
    if (match(TokenKind::KEYWORD_IF))
      return parse_if();

    // Check for an if expression
    if (match(TokenKind::KEYWORD_FOR))
      return parse_for();

    // Check for an if expression
    if (match(TokenKind::KEYWORD_WHILE))
      return parse_while();

    // Check for an if expression
    if (match(TokenKind::KEYWORD_UNTIL))
      return parse_until();

    // Check for a block expression
    if (match(TokenKind::NEWLINE))
      return parse_block();

    // Parse a operation expression
    return parse_operation();
  }

  // Parse an echo expression
  // echo → 'echo' operation
  expr_ptr Parser::parse_echo()
  {
    // Set the keyword
    auto keyword = current();

    // Parse a operation expression
    auto expr = parse_operation();

    // Return the expression
    return std::make_shared<ExprEcho>(keyword, expr);
  }

  // Parse an if expression
  // if → 'if' operation 'then' expression ('else' expression)?
  expr_ptr Parser::parse_if()
  {
    // Set the keyword
    auto keyword = current();

    // Parse the condition
    auto condition = parse_operation();

    // Parse the true branch
    consume(TokenKind::KEYWORD_THEN, "in if expression");
    auto true_branch = parse_expression();

    // Parse the false branch
    std::optional<expr_ptr> false_branch = std::nullopt;
    if (match(TokenKind::KEYWORD_ELSE))
      false_branch = std::make_optional(parse_expression());

    // Return the expression
    return std::make_shared<ExprIf>(keyword, condition, true_branch, false_branch);
  }

  // Parse a for expression
  // for → 'for' IDENTIFIER 'in' operation 'do' expression
  expr_ptr Parser::parse_for()
  {
    // Set the keyword
    auto keyword = current();

    // Parse the name
    auto name = consume(TokenKind::IDENTIFIER, "in for expression");

    // Parse the iterable
    consume(TokenKind::KEYWORD_IN, "in for expression");
    auto iterable = parse_operation();

    // Parse the body
    consume(TokenKind::KEYWORD_DO, "in for expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprFor>(keyword, name, iterable, body);
  }

  // Parse a while expression
  // while → 'while' operation 'do' expression
  expr_ptr Parser::parse_while()
  {
    // Set the keyword
    auto keyword = current();

    // Parse the condition
    auto condition = parse_operation();

    // Parse the body
    consume(TokenKind::KEYWORD_DO, "in while expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprWhile>(keyword, condition, body);
  }

  // Parse an until expression
  // until → 'until' operation 'do' expression
  expr_ptr Parser::parse_until()
  {
    // Set the keyword
    auto keyword = current();

    // Parse the condition
    auto condition = parse_operation();

    // Parse the body
    consume(TokenKind::KEYWORD_DO, "in until expression");
    auto body = parse_expression();

    // Return the expression
    return std::make_shared<ExprUntil>(keyword, condition, body);
  }

  // Parse a block expression
  // block → NEWLINE INDENT line+ DEDENT
  expr_ptr Parser::parse_block()
  {
    // Consume an INDENT token
    consume(TokenKind::INDENT, "in block");

    // Create a vector to store expressions
    std::vector<expr_ptr> exprs;

    // Parse lines until we reach a DEDENT token
    do
    {
      // Parse a line
      exprs.push_back(parse_line());
    } while (!match(TokenKind::DEDENT));

    // Return a block expresssion containing the expressions
    return std::make_shared<ExprBlock>(exprs);
  }

  // Parse an operation expression
  // operation → logic_or
  expr_ptr Parser::parse_operation()
  {
    return parse_logic_or();
  }

  // Parse a logic or expression
  // logic_or → logic_and ('or' logic_and)*
  expr_ptr Parser::parse_logic_or()
  {
    return parse_infix_op({
      TokenKind::OPERATOR_LOGIC_OR,
    }, &Parser::parse_logic_and);
  }

  // Parse a logic and expression
  // logic_and → logic_not ('and' logic_not)*
  expr_ptr Parser::parse_logic_and()
  {
    return parse_infix_op({
      TokenKind::OPERATOR_LOGIC_AND,
    }, &Parser::parse_logic_not);
  }

  // Parse a logic not expression
  // logic_not → 'not' logic_not | equality
  expr_ptr Parser::parse_logic_not()
  {
    return parse_prefix_op({
      TokenKind::OPERATOR_LOGIC_NOT,
    }, &Parser::parse_equality);
  }

  // Parse an equality expression
  // equality → comparison (('==' | '!=' | '===' | '!==') comparison)?
  expr_ptr Parser::parse_equality()
  {
    return parse_infix_op_single({
      TokenKind::OPERATOR_EQUAL,
      TokenKind::OPERATOR_NOT_EQUAL,
      TokenKind::OPERATOR_IDENTICAL,
      TokenKind::OPERATOR_NOT_IDENTICAL,
    }, &Parser::parse_comparison);
  }

  // Parse a comparison expression
  // comparison → threeway (('<' | '<=' | '>' | '>=' | '=~' | '!~') threeway)?
  expr_ptr Parser::parse_comparison()
  {
    return parse_infix_op_single({
      TokenKind::OPERATOR_LESS,
      TokenKind::OPERATOR_LESS_EQUAL,
      TokenKind::OPERATOR_GREATER,
      TokenKind::OPERATOR_GREATER_EQUAL,
      TokenKind::OPERATOR_MATCH,
      TokenKind::OPERATOR_NOT_MATCH,
    }, &Parser::parse_threeway);
  }

  // Parse a threeway expression
  // threeway → range ('<=>' range)?
  expr_ptr Parser::parse_threeway()
  {
    return parse_infix_op_single({
      TokenKind::OPERATOR_COMPARE,
    }, &Parser::parse_range);
  }

  // Parse a range expression
  // range → term ('..' term)?
  expr_ptr Parser::parse_range()
  {
    return parse_infix_op_single({
      TokenKind::OPERATOR_RANGE,
    }, &Parser::parse_term);
  }

  // Parse a term expression
  // term → factor (('+' | '-') factor)*
  expr_ptr Parser::parse_term()
  {
    return parse_infix_op({
      TokenKind::OPERATOR_ADD,
      TokenKind::OPERATOR_SUBTRACT,
    }, &Parser::parse_factor);
  }

  // Parse a factor expression
  // factor → unary (('*' | '/' | '//' | '%') unary)*
  expr_ptr Parser::parse_factor()
  {
    return parse_infix_op({
      TokenKind::OPERATOR_MULTIPLY,
      TokenKind::OPERATOR_DIVIDE,
      TokenKind::OPERATOR_QUOTIENT,
      TokenKind::OPERATOR_REMAINDER,
    }, &Parser::parse_unary);
  }

  // Parse an unary expression
  // unary → ('-' | '#' | '$') unary | primary
  expr_ptr Parser::parse_unary()
  {
    return parse_prefix_op({TokenKind::OPERATOR_SUBTRACT, TokenKind::OPERATOR_LENGTH, TokenKind::OPERATOR_STRING}, &Parser::parse_primary);
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
      if (match(TokenKind::PARENTHESIS_LEFT))
      {
        // Set the token
        auto token = current();

        // Parse the arguments
        auto arguments = std::make_shared<ExprSequence>(token, parse_arguments());

        // Create the call expression
        expr = std::make_shared<ExprCall>(expr, token, arguments);
      }

      // Check for a get postfix
      else if (match(TokenKind::SYMBOL_DOT))
      {
        // Parse the name
        auto name = consume(TokenKind::IDENTIFIER, "in get expression");

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
    // Check for a literal expression
    if (match(TokenKind::KEYWORD_NOTHING))
      return std::make_shared<ExprLiteral>(internals::Value::value_nothing, current().location());
    if (match(TokenKind::KEYWORD_FALSE))
      return std::make_shared<ExprLiteral>(internals::Value::value_false, current().location());
    if (match(TokenKind::KEYWORD_TRUE))
      return std::make_shared<ExprLiteral>(internals::Value::value_true, current().location());
    if (match(TokenKind::LITERAL_INT))
      return parse_int();
    if (match(TokenKind::LITERAL_REAL))
      return parse_real();
    if (match(TokenKind::LITERAL_RUNE))
      return parse_rune();
    if (match(TokenKind::LITERAL_STRING))
      return parse_string();
    if (match(TokenKind::LITERAL_REGEX))
      return parse_regex();

    // Check for a name expression
    if (match(TokenKind::IDENTIFIER))
      return std::make_shared<ExprName>(current());

    // Check for a sequence expression
    if (match(TokenKind::SQUARE_BRACKET_LEFT))
      return parse_sequence();

    // Check for a record expression
    if (match(TokenKind::CURLY_BRACKET_LEFT))
      return parse_record();

    // Check for a lambda expression
    if (match(TokenKind::SYMBOL_BACKSLASH))
      return parse_lambda();

    // Check for a grouped expression
    if (match(TokenKind::PARENTHESIS_LEFT))
      return parse_grouped();

    // No suitable expression found, so throw an error
    throw report<SyntaxError>(next().location(), fmt::format("Expected atom, but found {}", next().kind()));
  }

  // Parse an int literal
  expr_ptr Parser::parse_int()
  {
    try
    {
      auto int_value = utils::parse_int(current().value());
      auto value = internals::Value::of_int(int_value);
      return std::make_shared<ExprLiteral>(value, current().location());
    }
    catch (internals::ValueMismatchException& ex)
    {
      report<ValueMismatchError>(current().location(), ex.message());
      return nullptr;
    }
    catch (internals::ValueOverflowException& ex)
    {
      report<ValueOverflowError>(current().location(), ex.message());
      return nullptr;
    }
  }

  // Parse a real literal
  expr_ptr Parser::parse_real()
  {
    try
    {
      auto real_value = utils::parse_real(current().value());
      auto value = internals::Value::of_real(real_value);
      return std::make_shared<ExprLiteral>(value, current().location());
    }
    catch (internals::ValueMismatchException& ex)
    {
      throw report<ValueMismatchError>(current().location(), ex.message());
    }
    catch (internals::ValueOverflowException& ex)
    {
      throw report<ValueOverflowError>(current().location(), ex.message());
    }
  }

  // Parse a rune literal
  expr_ptr Parser::parse_rune()
  {
    try
    {
      auto rune_value = utils::parse_rune(current().value());
      auto value = internals::Value::of_rune(rune_value);
      return std::make_shared<ExprLiteral>(value, current().location());
    }
    catch (internals::ValueMismatchException& ex)
    {
      report<ValueMismatchError>(current().location(), ex.message());
      return nullptr;
    }
    catch (internals::ValueOverflowException& ex)
    {
      report<ValueOverflowError>(current().location(), ex.message());
      return nullptr;
    }
  }

  // Parse a string literal
  expr_ptr Parser::parse_string()
  {
    try
    {
      auto string_value = utils::parse_string(current().value());
      auto value = internals::Value::of_obj(std::make_shared<internals::String>(string_value).get());
      return std::make_shared<ExprLiteral>(value, current().location());
    }
    catch (...)
    {
      // TODO: Add proper catch
      return nullptr;
    }
  }

  // Parse a regex literal
  expr_ptr Parser::parse_regex()
  {
    try
    {
      // TODO: Properly parse the regex literal instead of making a literal string
      auto string_value = utils::parse_string(current().value());
      auto value = internals::Value::of_obj(std::make_shared<internals::String>(string_value).get());
      return std::make_shared<ExprLiteral>(value, current().location());
    }
    catch (...)
    {
      // TODO: Add proper catch
      return nullptr;
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
    auto token = current();

    // Parse expressions until we reach the closing token
    do
    {
      // Parse the value of the item
      items.push_back(parse_expression());
    } while (match(TokenKind::SYMBOL_COMMA));

    // Consume the closing token
    consume(TokenKind::SQUARE_BRACKET_RIGHT, "in sequence atom");

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
    auto token = current();

    // Parse expressions until we reach the closing token
    do
    {
      // Parse the name of the item
      auto name = consume(TokenKind::IDENTIFIER, "in record atom").value();
      consume(TokenKind::SYMBOL_COLON, "in record atom");

      // Parse the value of the item
      items.insert(std::make_pair(name, parse_expression()));
    } while (match(TokenKind::SYMBOL_COMMA));

    // Consume the closing token
    consume(TokenKind::CURLY_BRACKET_RIGHT, "in record atom");

    // Return a record expression containing the items
    return std::make_shared<ExprRecord>(token, items);
  }

  // Parse a lambda expression
  // lambda → '\' '(' parameters? ')' (':' type)? '=' assignment
  expr_ptr Parser::parse_lambda()
  {
    // Set the token
    auto token = current();

    // Parse the parameters
    consume(TokenKind::PARENTHESIS_LEFT, "in lambda atom");
    auto parameters = parse_parameters();

    // Parse the return type
    std::optional<type_expr_ptr> return_type = std::nullopt;
    if (match(TokenKind::SYMBOL_COLON))
      return_type = std::make_optional(parse_type());

    // Parse the body
    consume(TokenKind::OPERATOR_ASSIGN, "in lambda atom");
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
    consume(TokenKind::PARENTHESIS_RIGHT, "in grouped atom");

    // Return the grouped expression
    return std::make_shared<ExprGrouped>(expr);
  }

  // --------------------------------------------------------------------------
  // PARSERS FOR TYPE EXPRESSIONS
  // --------------------------------------------------------------------------

  // Parse a type expression
  // type → type_union
  type_expr_ptr Parser::parse_type()
  {
    // Parse an union type expression
    return parse_type_union();
  }

  // Parse an union type expression
  // type_union → type_intersection ('&' type_intersection)*
  type_expr_ptr Parser::parse_type_union()
  {
    // Parse the left operand
    auto left = parse_type_intersection();

    // Match the operator
    while (match(TokenKind::OPERATOR_UNION))
    {
      // Set the operator
      auto op = current();

      // Parse the right operand
      auto right = parse_type_intersection();

      // Create the union type expression
      left = std::make_shared<TypeExprUnion>(left, op, right);
    }

    // Return the type expression
    return left;
  }

  // Parse an intersection type expression
  // type_intersection → type_maybe ('|' type_maybe)*
  type_expr_ptr Parser::parse_type_intersection()
  {
    // Parse the left operand
    auto left = parse_type_maybe();

    // Match the operator
    while (match(TokenKind::OPERATOR_INTERSECTION))
    {
      // Set the operator
      auto op = current();

      // Parse the right operand
      auto right = parse_type_maybe();

      // Create the union type expression
      left = std::make_shared<TypeExprUnion>(left, op, right);
    }

    // Return the type expression
    return left;
  }

  // Parse a maybe type expression
  // type_maybe → type_generic ('?')?
  type_expr_ptr Parser::parse_type_maybe()
  {
    // Parse a generic type
    auto expr = parse_type_generic();

    // Match the operator
    if (match(TokenKind::OPERATOR_MAYBE))
    {
      // Set the operator
      auto op = current();

      // Create the maybe expression
      return std::make_shared<TypeExprMaybe>(expr, op);
    }

    // Return the expression
    return expr;
  }

  // Parse a generic type expression
  // type_generic → IDENTIFIER ('[' type_arguments ']')? | type_grouped
  type_expr_ptr Parser::parse_type_generic()
  {
    // Check for a grouped type expression
    if (match(TokenKind::PARENTHESIS_LEFT))
      return parse_type_grouped();

    // Parse the name
    auto name = consume(TokenKind::IDENTIFIER, "in type");
    auto expr = std::make_shared<TypeExprName>(name);

    // Check for generic arguments
    if (match(TokenKind::SQUARE_BRACKET_LEFT))
    {
      // Set the token
      auto token = current();

      // Parse the generic arguments
      auto arguments = parse_type_arguments();

      // Create the generic expression
      return std::make_shared<TypeExprGeneric>(expr, token, arguments);
    }

    // Return the expression
    return expr;
  }

  // Parse a grouped type expression
  // type_grouped → '(' type ')'
  type_expr_ptr Parser::parse_type_grouped()
  {
    // Parse the grouped expression
    auto expr = parse_type();

    // Consume the closing parenthesis
    consume(TokenKind::PARENTHESIS_RIGHT, "in grouped type");

    // Return the grouped expression
    return std::make_shared<TypeExprGrouped>(expr);
  }

  // --------------------------------------------------------------------------
  // PARSERS FOR SPECIALIZED TYPES
  // --------------------------------------------------------------------------

  // Parse parameters
  // parameters → (IDENTIFIER ':' type) (',' IDENTIFIER ':' type)*)?
  ExprFunction::parameters_type Parser::parse_parameters()
  {
    // Create a vector to store the parameters
    ExprFunction::parameters_type parameters;

    // Loop until we encounter a closing parenthesis
    if (!check(TokenKind::PARENTHESIS_RIGHT))
    {
      do
      {
        // Parse the name
        auto name = consume(TokenKind::IDENTIFIER, "in parameters");

        // Parse the type
        consume(TokenKind::SYMBOL_COLON, "in parameters");
        auto type = parse_type();

        // Create the expression and add it to the parameters
        parameters.push_back(std::make_shared<ExprFunctionParameter>(name, type));
      } while (match(TokenKind::SYMBOL_COMMA));
    }

    // Consume the closing parenthesis
    consume(TokenKind::PARENTHESIS_RIGHT, "in parameters");

    // Return the parameters
    return parameters;
  }

  // Parse arguments
  // arguments → assignment (',' assignment)*
  ExprSequence::sequence_type Parser::parse_arguments()
  {
    // Create a vector to store the arguments
    ExprSequence::sequence_type arguments;

    // Loop until we encounter a closing parenthesis
    if (!check(TokenKind::PARENTHESIS_RIGHT))
    {
      do
      {
        // Parse the expression and add it to the arguments
        arguments.push_back(parse_assignment());
      } while (match(TokenKind::SYMBOL_COMMA));
    }

    // Consume the closing parenthesis
    consume(TokenKind::PARENTHESIS_RIGHT, "in arguments");

    // Return the arguments
    return arguments;
  }

  // Parse type arguments
  // type_arguments → type (',' type)*
  std::vector<type_expr_ptr> Parser::parse_type_arguments()
  {
    // Create a vector to store the arguments
    std::vector<type_expr_ptr> arguments;

    // Loop until we encounter a closing square bracket
    if (check(TokenKind::SQUARE_BRACKET_RIGHT))
      report<SyntaxError>(next().location(), "Expected type, but found right square bracket");

    do
    {
      // Parse the expression and add it to the arguments
      arguments.push_back(parse_type());
    } while (match(TokenKind::SYMBOL_COMMA));

    // Consume the closing parenthesis
    consume(TokenKind::SQUARE_BRACKET_RIGHT, "in type arguments");

    // Return the arguments
    return arguments;
  }
}
