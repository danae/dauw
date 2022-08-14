#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/location.hpp>


namespace dauw
{
  // Enum that defines the kind of a token in a source string
  enum class TokenKind : uint8_t
  {
    // Lexer-specific
    END,
    NEWLINE,
    INDENT,
    DEDENT,
    COMMENT,

    // Delimiters
    PARENTHESIS_LEFT,
    PARENTHESIS_RIGHT,
    SQUARE_BRACKET_LEFT,
    SQUARE_BRACKET_RIGHT,
    CURLY_BRACKET_LEFT,
    CURLY_BRACKET_RIGHT,

    // Synbols
    SYMBOL_COLON,
    SYMBOL_COMMA,
    SYMBOL_DOT,
    SYMBOL_BACKSLASH,

    // Operators
    OPERATOR_MAYBE,
    OPERATOR_INTERSECTION,
    OPERATOR_UNION,
    OPERATOR_LENGTH,
    OPERATOR_STRING,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_QUOTIENT,
    OPERATOR_REMAINDER,
    OPERATOR_ADD,
    OPERATOR_SUBTRACT,
    OPERATOR_RANGE,
    OPERATOR_COMPARE,
    OPERATOR_LESS,
    OPERATOR_LESS_EQUAL,
    OPERATOR_GREATER,
    OPERATOR_GREATER_EQUAL,
    OPERATOR_MATCH,
    OPERATOR_NOT_MATCH,
    OPERATOR_EQUAL,
    OPERATOR_NOT_EQUAL,
    OPERATOR_IDENTICAL,
    OPERATOR_NOT_IDENTICAL,
    OPERATOR_LOGIC_NOT,
    OPERATOR_LOGIC_AND,
    OPERATOR_LOGIC_OR,
    OPERATOR_ASSIGN,

    // Keywords
    KEYWORD_DEF,
    KEYWORD_DO,
    KEYWORD_ECHO,
    KEYWORD_ELSE,
    KEYWORD_FALSE,
    KEYWORD_FOR,
    KEYWORD_IF,
    KEYWORD_IN,
    KEYWORD_NOTHING,
    KEYWORD_THEN,
    KEYWORD_TRUE,
    KEYWORD_UNTIL,
    KEYWORD_WHILE,

    // Identifiers
    IDENTIFIER,

    // Literals
    LITERAL_INT,
    LITERAL_REAL,
    LITERAL_RUNE,
    LITERAL_STRING,
    LITERAL_REGEX,
  };


  // Class that defines a token in a source string
  class Token
  {
    private:
      // The kind of the token
      TokenKind kind_;

      // The value of the token
      string_t value_;

      // The location of the token
      Location location_;


    public:
      // Constructor
      Token(TokenKind kind, string_t value, Location location);
      Token(TokenKind kind, Location location);

      // Return the kind of the token
      TokenKind& kind();

      // Return the value of the token
      string_t& value();

      // Return the location of the token
      Location& location();

      // Operator overloads
      bool operator==(const Token& other);
      bool operator<(const Token& other);
  };
}


namespace fmt
{
  using namespace dauw;

  // Class that defines a formatter for a token kind
  template <>
  struct formatter<TokenKind> : formatter<string_view_t>
  {
    inline string_t stringify(TokenKind kind)
    {
      switch (kind)
      {
        // Lexer-specific
        case TokenKind::END: return "end of tokens";
        case TokenKind::NEWLINE: return "newline";
        case TokenKind::INDENT: return "indent";
        case TokenKind::DEDENT: return "dedent";
        case TokenKind::COMMENT: return "comment";
        case TokenKind::PARENTHESIS_LEFT: return "'('";
        case TokenKind::PARENTHESIS_RIGHT: return "')'";
        case TokenKind::SQUARE_BRACKET_LEFT: return "'['";
        case TokenKind::SQUARE_BRACKET_RIGHT: return "']'";
        case TokenKind::CURLY_BRACKET_LEFT: return "'{'";
        case TokenKind::CURLY_BRACKET_RIGHT: return "'}'";
        case TokenKind::SYMBOL_COLON: return "':'";
        case TokenKind::SYMBOL_COMMA: return "','";
        case TokenKind::SYMBOL_DOT: return "'.'";
        case TokenKind::SYMBOL_BACKSLASH: return "'\\'";
        case TokenKind::OPERATOR_MAYBE: return "maybe operator";
        case TokenKind::OPERATOR_INTERSECTION: return "intersection operator";
        case TokenKind::OPERATOR_UNION: return "union operator";
        case TokenKind::OPERATOR_LENGTH: return "length operator";
        case TokenKind::OPERATOR_STRING: return "string operator";
        case TokenKind::OPERATOR_MULTIPLY: return "multiply operator";
        case TokenKind::OPERATOR_DIVIDE: return "divide operator";
        case TokenKind::OPERATOR_QUOTIENT: return "quotient operator";
        case TokenKind::OPERATOR_REMAINDER: return "remainder operator";
        case TokenKind::OPERATOR_ADD: return "add operator";
        case TokenKind::OPERATOR_SUBTRACT: return "subtract operator";
        case TokenKind::OPERATOR_RANGE: return "range operator";
        case TokenKind::OPERATOR_COMPARE: return "compare operator";
        case TokenKind::OPERATOR_LESS: return "less than operator";
        case TokenKind::OPERATOR_LESS_EQUAL: return "less than or equal operator";
        case TokenKind::OPERATOR_GREATER: return "greater than operator";
        case TokenKind::OPERATOR_GREATER_EQUAL: return "greater than or equal operator";
        case TokenKind::OPERATOR_MATCH: return "match operator";
        case TokenKind::OPERATOR_NOT_MATCH: return "not match operator";
        case TokenKind::OPERATOR_EQUAL: return "equal operator";
        case TokenKind::OPERATOR_NOT_EQUAL: return "not equal operator";
        case TokenKind::OPERATOR_IDENTICAL: return "identical operator";
        case TokenKind::OPERATOR_NOT_IDENTICAL: return "not identical operator";
        case TokenKind::OPERATOR_LOGIC_NOT: return "logic not operator";
        case TokenKind::OPERATOR_LOGIC_AND: return "logic and operator";
        case TokenKind::OPERATOR_LOGIC_OR: return "logic or operator";
        case TokenKind::OPERATOR_ASSIGN: return "assign operator";
        case TokenKind::KEYWORD_DEF: return "'def'";
        case TokenKind::KEYWORD_DO: return "'do'";
        case TokenKind::KEYWORD_ECHO: return "'echo'";
        case TokenKind::KEYWORD_ELSE: return "'else'";
        case TokenKind::KEYWORD_FALSE: return "'false'";
        case TokenKind::KEYWORD_FOR: return "'for'";
        case TokenKind::KEYWORD_IF: return "'of'";
        case TokenKind::KEYWORD_IN: return "'in'";
        case TokenKind::KEYWORD_NOTHING: return "'nothing'";
        case TokenKind::KEYWORD_THEN: return "'then'";
        case TokenKind::KEYWORD_TRUE: return "'true'";
        case TokenKind::KEYWORD_UNTIL: return "'until'";
        case TokenKind::KEYWORD_WHILE: return "'while'";
        case TokenKind::IDENTIFIER: return "identifier";
        case TokenKind::LITERAL_INT: return "int literal";
        case TokenKind::LITERAL_REAL: return "real literal";
        case TokenKind::LITERAL_RUNE: return "rune literal";
        case TokenKind::LITERAL_STRING: return "string literal";
        case TokenKind::LITERAL_REGEX: return "regex literal";
        default: return "<undefined>";
      }
    }

    template <typename FormatContext>
    auto format(TokenKind kind, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(kind), ctx);
    }
  };

  // Class that defines a formatter for a token
  template <>
  struct formatter<Token> : formatter<string_view_t>
  {
    inline string_t stringify(Token token)
    {
      if (!token.value().empty())
        return fmt::format("{} \"{}\" at {}", token.kind(), token.value(), token.location());
      else
        return fmt::format("{} at {}", token.kind(), token.location());
    }

    template <typename FormatContext>
    auto format(Token token, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(token), ctx);
    }
  };
}
