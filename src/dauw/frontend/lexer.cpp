#include "lexer.hpp"

namespace dauw::frontend
{
  // Constructor for a lexer rule
  LexerRule::LexerRule(TokenKind kind, utils::Regex pattern, size_t group)
    : kind_(kind), pattern_(pattern), group_(std::make_optional(group))
  {

  }

  // Constructor for a lexer rule without capturing group
  LexerRule::LexerRule(TokenKind kind, utils::Regex pattern)
    : kind_(kind), pattern_(pattern), group_(std::nullopt)
  {
  }

  // Return the token kind of the rule
  TokenKind LexerRule::kind()
  {
    return kind_;
  }

  // Return the regular expression pattern of the rule
  utils::Regex LexerRule::pattern()
  {
    return pattern_;
  }

  // Return the replacement of the rule based on a match
  string_t LexerRule::replace(utils::RegexMatch match)
  {
    return group_.has_value() ? match.group(group_.value()).value() : "";
  }

  // --------------------------------------------------------------------------

  // Initialize the regex patterns for the lexer
  utils::Regex Lexer::comment_pattern_("(?:[ \\t]*--[ \\t]*(.*))|(?:\"((?:[^\\\\\"]|\\\\.)*)\")");
  utils::Regex Lexer::whitespace_pattern_("[ \\t]+");
  utils::Regex Lexer::ascii_identifier_pattern_("[A-Za-z_][A-Za-z0-9_]*");
  utils::Regex Lexer::stropped_identifier_pattern_("`((?:[^\\\\`]|\\\\.)+)`");
  utils::Regex Lexer::int_pattern_("0[Xx][0-9A-Fa-f][0-9A-Fa-f_]*|-?(?:0|[1-9][0-9_]*)");
  utils::Regex Lexer::real_pattern_("-?(?:0|[1-9][0-9_]*)(?:\\.[0-9][0-9_]*(?:[Ee][+-]?(?:0|[1-9][0-9_]*))?|[Ee][+-]?(?:0|[1-9][0-9_]*))");
  utils::Regex Lexer::rune_pattern_("'((?:[^\\\\']|\\\\.)*)'");
  utils::Regex Lexer::string_pattern_("\"((?:[^\\\\\"]|\\\\.)*)\"");
  utils::Regex Lexer::regex_pattern_("/((?:[^\\\\/]|\\\\.)*)/[A-Za-z]*");

  // Initialize the rules for the lexer
  std::vector<LexerRule> Lexer::rules_({
    // Delimiters
    LexerRule(TokenKind::PARENTHESIS_LEFT, utils::Regex("\\(")),
    LexerRule(TokenKind::PARENTHESIS_RIGHT, utils::Regex("\\)")),
    LexerRule(TokenKind::SQUARE_BRACKET_LEFT, utils::Regex("\\[")),
    LexerRule(TokenKind::SQUARE_BRACKET_RIGHT, utils::Regex("\\]")),
    LexerRule(TokenKind::CURLY_BRACKET_LEFT, utils::Regex("\\{")),
    LexerRule(TokenKind::CURLY_BRACKET_RIGHT, utils::Regex("\\}")),

    // Symbols
    LexerRule(TokenKind::SYMBOL_COLON, utils::Regex(":")),
    LexerRule(TokenKind::SYMBOL_COMMA, utils::Regex(",")),
    LexerRule(TokenKind::SYMBOL_DOT, utils::Regex("\\.")),
    LexerRule(TokenKind::SYMBOL_BACKSLASH, utils::Regex("\\\\")),

    // Operators
    LexerRule(TokenKind::OPERATOR_MAYBE, utils::Regex("\\?")),
    LexerRule(TokenKind::OPERATOR_INTERSECTION, utils::Regex("&")),
    LexerRule(TokenKind::OPERATOR_UNION, utils::Regex("\\|")),
    LexerRule(TokenKind::OPERATOR_LENGTH, utils::Regex("#")),
    LexerRule(TokenKind::OPERATOR_STRING, utils::Regex("\\$")),
    LexerRule(TokenKind::OPERATOR_MULTIPLY, utils::Regex("\\*")),
    LexerRule(TokenKind::OPERATOR_DIVIDE, utils::Regex("/")),
    LexerRule(TokenKind::OPERATOR_QUOTIENT, utils::Regex("//")),
    LexerRule(TokenKind::OPERATOR_REMAINDER, utils::Regex("%")),
    LexerRule(TokenKind::OPERATOR_ADD, utils::Regex("\\+")),
    LexerRule(TokenKind::OPERATOR_SUBTRACT, utils::Regex("-")),
    LexerRule(TokenKind::OPERATOR_RANGE, utils::Regex("\\.\\.")),
    LexerRule(TokenKind::OPERATOR_COMPARE, utils::Regex("<=>")),
    LexerRule(TokenKind::OPERATOR_LESS, utils::Regex("<")),
    LexerRule(TokenKind::OPERATOR_LESS_EQUAL, utils::Regex("<=")),
    LexerRule(TokenKind::OPERATOR_GREATER, utils::Regex(">")),
    LexerRule(TokenKind::OPERATOR_GREATER_EQUAL, utils::Regex(">=")),
    LexerRule(TokenKind::OPERATOR_MATCH, utils::Regex("=~")),
    LexerRule(TokenKind::OPERATOR_NOT_MATCH, utils::Regex("!~")),
    LexerRule(TokenKind::OPERATOR_EQUAL, utils::Regex("==")),
    LexerRule(TokenKind::OPERATOR_NOT_EQUAL, utils::Regex("!=")),
    LexerRule(TokenKind::OPERATOR_IDENTICAL, utils::Regex("===")),
    LexerRule(TokenKind::OPERATOR_NOT_IDENTICAL, utils::Regex("!==")),
    LexerRule(TokenKind::OPERATOR_LOGIC_NOT, utils::Regex("not")),
    LexerRule(TokenKind::OPERATOR_LOGIC_AND, utils::Regex("and")),
    LexerRule(TokenKind::OPERATOR_LOGIC_OR, utils::Regex("or")),
    LexerRule(TokenKind::OPERATOR_ASSIGN, utils::Regex("=")),

    // Keywords
    LexerRule(TokenKind::KEYWORD_DEF, utils::Regex("def")),
    LexerRule(TokenKind::KEYWORD_DO, utils::Regex("do")),
    LexerRule(TokenKind::KEYWORD_ECHO, utils::Regex("echo")),
    LexerRule(TokenKind::KEYWORD_ELSE, utils::Regex("else")),
    LexerRule(TokenKind::KEYWORD_FALSE, utils::Regex("false")),
    LexerRule(TokenKind::KEYWORD_FOR, utils::Regex("for")),
    LexerRule(TokenKind::KEYWORD_IF, utils::Regex("if")),
    LexerRule(TokenKind::KEYWORD_IN, utils::Regex("in")),
    LexerRule(TokenKind::KEYWORD_NOTHING, utils::Regex("nothing")),
    LexerRule(TokenKind::KEYWORD_THEN, utils::Regex("then")),
    LexerRule(TokenKind::KEYWORD_TRUE, utils::Regex("true")),
    LexerRule(TokenKind::KEYWORD_UNTIL, utils::Regex("until")),
    LexerRule(TokenKind::KEYWORD_WHILE, utils::Regex("while")),

    // Identifiers
    LexerRule(TokenKind::IDENTIFIER, ascii_identifier_pattern_, 0),
    LexerRule(TokenKind::IDENTIFIER, stropped_identifier_pattern_, 1),

    // Literals
    LexerRule(TokenKind::LITERAL_INT, int_pattern_, 0),
    LexerRule(TokenKind::LITERAL_REAL, real_pattern_, 0),
    LexerRule(TokenKind::LITERAL_RUNE, rune_pattern_, 1),
    LexerRule(TokenKind::LITERAL_STRING, string_pattern_, 1),
    LexerRule(TokenKind::LITERAL_REGEX, regex_pattern_, 0),
  });


  // Constructor for the lexer
  Lexer::Lexer(Reporter* reporter, source_ptr source)
    : ReporterAware(reporter), source_(source)
  {
  }

  // Convert a string into a deque of tokens
  Lexer::token_list_type Lexer::tokenize()
  {
    // Initialize a deque to store the tokens
    token_list_type tokens;

    // Initialize a stack to store the indentation levels
    std::deque<int> indents({0});

    // ReInitialize a location for the lexer
    Location location;

    // Iterate over the lines in the source
    for (auto line : source_->source_lines())
    {
      // Check for a shebang at the start of the source
      if (line.rfind("#!", 0) == 0)
      {
        // If this is the first line of the source, then ignore the shebang, otherwise report an error
        if (location.line() != 0)
        {
          report<SyntaxError>(location, "A shebang is only allowed at the first line of the source");
          return token_list_type({});
        }

        // Increase the position to past the shebang
        location.increase_line_();
        continue;
      }

      // Skip the line if it is empty or only contains whitespace
      if (line.empty() || line.find_first_not_of(" \t\r\n") == string_t::npos)
      {
        location.increase_line_();
        continue;
      }

      // Calculate the indentation at the start of the line and add tokens accordingly
      int indent = 0;
      for (auto indent_iterator = line.begin(); indent_iterator != line.end() && *indent_iterator == ' '; ++ indent_iterator)
        indent ++;

      if (indent > indents.back())
      {
        // Check if the indent is on the first line, because that's an Error
        if (location.line() == 0)
        {
          report<SyntaxError>(location, "The first line of the source should never be indented");
          return token_list_type({});
        }

        // If the indent is bigger than the last indent, then add an indent token
        indents.push_back(indent);
        tokens.push_back(Token(TokenKind::INDENT, location));
      }

      while (indent < indents.back())
      {
        // If the indent is smaller than the last indent, then add as much dedent tokens until the indent matches one on the stack
        indents.pop_back();
        tokens.push_back(Token(TokenKind::DEDENT, location));
      }

     // If the indent doesn't match the current indentation level, then report an error
      if (indent != indents.back())
      {
        report<SyntaxError>(location, "The indentation does not match any outer indentation level");
        return token_list_type({});
      }

      location.increase_col_(indent);

      // Iterate over the line
      while (location.col() < line.length())
      {
        // Check for comments at the current position
        auto comment_match = comment_pattern_.match(line, location.col(), utils::REGEX_MATCH_AT_BEGIN);
        if (comment_match.success() && comment_match.group(1).success())
        {
          tokens.push_back(Token(TokenKind::COMMENT, comment_match.group(1).value(), location));
          location.increase_col_(comment_match.length());
          continue;
        }

        // Check for whitespaces at the current position
        auto whitespace_match = whitespace_pattern_.match(line, location.col(), utils::REGEX_MATCH_AT_BEGIN);
        if (whitespace_match.success())
        {
          location.increase_col_(whitespace_match.length());
          continue;
        }

        // Iterate over the rules to see if they match
        std::vector<std::tuple<Token, size_t>> matched_tokens;
        for (auto rule : rules_)
        {
          auto rule_match = rule.pattern().match(line, location.col(), utils::REGEX_MATCH_AT_BEGIN);
          if (rule_match.success())
            matched_tokens.push_back(std::make_tuple(Token(rule.kind(), rule.replace(rule_match), location), rule_match.length()));
        }

        // If there are no matched tokens, then we've encountered a syntax Error
        if (matched_tokens.empty())
        {
          report<SyntaxError>(location, fmt::format("Invalid character '{}'", line.substr(location.col(), 1)));
          return token_list_type({});
        }

        // if there is more than one matched token, then sort the current matched tokens
        if (matched_tokens.size() > 1)
        {
          std::sort(matched_tokens.begin(), matched_tokens.end(), [](std::tuple<Token, size_t> a, std::tuple<Token, size_t> b)->bool {
            return std::get<0>(a) < std::get<0>(b) || std::get<1>(a) > std::get<1>(b);
          });
        }

        // Add the first token and increase the position to past the token
        auto token = *matched_tokens.begin();
        tokens.push_back(std::get<0>(token));
        location.increase_col_(std::get<1>(token));
      }

      // Add a newline at the end of the line and update the location to the next line
      tokens.push_back(Token(TokenKind::NEWLINE, location));
      location.increase_line_();
    }

    // Add dedent tokens for the remaining indents
    while (indents.back() > 0)
    {
      indents.pop_back();
      tokens.push_back(Token(TokenKind::DEDENT, location));
    }

    // Add an end token
    tokens.push_back(Token(TokenKind::END, location));

    // Return the tokens
    return tokens;
  }
}
